#include <GL/glew.h>
#include <X11/Xlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

typedef struct Config {
    char *fragmentFile;
    char *vertexFile;
    char *textureFile;
    float speed;
} Config;

static char* readFile(char *path) {
    FILE *file = fopen(path, "r");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    char *string = malloc(size + 1);
    fread(string, 1, size, file);
    fclose(file);
    return string;
}

static Config parseConfig(int argc, char **argv) {
    char *prev = *argv;
    Config config = {
        NULL,
        NULL,
        NULL,
        1.0
    };
    while (argc--) {
        char *word = *argv++;
        if (!strcmp(prev, "-f"))
            config.fragmentFile = word;
        else if (!strcmp(prev, "-v"))
            config.vertexFile = word;
        else if (!strcmp(prev, "-t"))
            config.textureFile = word;
        else if (!strcmp(prev, "-s"))
            config.speed = atof(word);
        prev = word;
    }
    return config;
}

static int compileShader(const char *code, int shaderType) {
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);
    int compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        int maxLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        char errorLog[0];
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        printf(errorLog);
        exit(EXIT_FAILURE);
    }
    return shader;
}

int main(int argc, char **argv) {
    Config config = parseConfig(argc, argv);
    char *vertexText = readFile(config.vertexFile);
    char *fragmentText = readFile(config.fragmentFile);
    Display *display = XOpenDisplay(NULL);
	SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindowFrom((void*) RootWindow(display, DefaultScreen(display)));
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);
    SDL_GL_MakeCurrent(window, SDL_GL_CreateContext(window));
    SDL_Surface* imageSurface = IMG_Load(config.textureFile);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, imageSurface->w, imageSurface->h, 24, 0xff000000, 0x00ff0000, 0x0000ff00, 0);
    SDL_BlitSurface(imageSurface, 0, surface, 0);
    SDL_FreeSurface(imageSurface);
    glewInit();
    int program = glCreateProgram();
    int vertexShader = compileShader(vertexText, GL_VERTEX_SHADER);
    int fragmentShader = compileShader(fragmentText, GL_FRAGMENT_SHADER);
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_FreeSurface(surface);
                SDL_DestroyWindow(window);
                exit(EXIT_SUCCESS);
            }
        }
        glUseProgram(program);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1f(glGetUniformLocation(program, "iTime"), SDL_GetTicks() / 1000.0 * config.speed);
        glUniform2f(glGetUniformLocation(program, "iResolution"), (float) width, (float) height);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        SDL_GL_SwapWindow(window);
        SDL_Delay(1000 / 60);
    }
}
