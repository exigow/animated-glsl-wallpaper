CC = gcc
CFLAGS = -ggdb3 -Wall -lX11 -lGL -lGLEW -lSDL2 -lSDL2_image

all:
	$(CC) $(CFLAGS) -o bg main.c

run:
	./bg -v quad.vert -f blob.frag -t green.png

leak-check:
	valgrind --leak-check=full --log-file=leak.txt ./bg -v quad.vert -f blob.frag -t green.png

clean:
	rm leak.txt & rm bg
