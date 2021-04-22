# Animated GLSL shader background for X11 in single C file

Uniforms have the same names and properties as in ShaderToy.

## Compilation

```shell
gcc -lX11 -lGL -lGLEW -lSDL2 -lSDL2_image -o main main.c
```

## Usage

```shell
./main -v quad.vert -f blob.frag -t green.png -s 1
```
