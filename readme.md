# Animated GLSL shader wallpaper for X11 in single C file

This repository contains minimal glue required to render animated wallpaper on Linux (X11). SDL2 is used for OpenGL context creation and image loading. Shader uniforms have the same names and properties as in ShaderToy.

## Compilation

```shell
gcc -lX11 -lGL -lGLEW -lSDL2 -lSDL2_image -o main main.c
```

## Usage

```shell
./main -v quad.vert -f blob.frag -t green.png -s 1
```

Where: 
* `-v` is vertex shader file
* `-f` is fragment shader file
* `-t` is texture file
* `-s` is time speed multiplier (optional)

![Screenshot](https://user-images.githubusercontent.com/5013708/115758743-c4188180-a39f-11eb-92f7-133fec4d2eb4.png)
