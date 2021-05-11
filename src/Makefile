CC=gcc
CFLAGS=-g -std=c11 -Wall -Werror
TARGET=./nbody
.PHONY: clean
all: $(TARGET)

nbody-gui: 
	$(CC) $^ -O0 $(TARGET)gui.c -o ./nbody-gui -Wall -Werror -Wvla -lpthread -lm -g -lSDL2 -lSDL2_gfx

nbody-cl:
	$(CC) $^ -O0 $(TARGET)cl.c -o ./nbody-cl -Wall -Werror -Wvla -lpthread -lm -g 

test: nbodytest.c
	$(CC) $(CFLAGS) $^ -o $@ -lpthread -lcmocka

clean:
	rm -f ./nbody-gui
	rm -f ./nbody-cl

