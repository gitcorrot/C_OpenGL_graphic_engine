CC = gcc
CFLAGS  = -g -Wall
TARGET = main
LDFLAGS = -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp
CFILES = $(TARGET).c

prog:
	@echo "Building..."
	@$(CC) $(CFLAGS) -o $(TARGET) $(CFILES) $(LDFLAGS) -L/home/corrot/Desktop/C_Projekty/opengl_test mathOpengl.a cube.a utils.a input.a camera.a
	@echo "...done!"
	@./main

libs:
	@echo "Generating libraries..."
	@gcc $(CFLAGS) -O -c mathOpengl.c cube.c utils.c input.c camera.c
	@ar rcs mathOpengl.a mathOpengl.o utils.o input.o camera.o
	@ar rcs cube.a cube.o
	@ar rcs utils.a utils.o
	@ar rcs input.a input.o
	@ar rcs camera.a camera.o
	@echo "...done!"

clean:
	@echo "Cleanning..."
	@$(RM) $(TARGET) 
	@echo "...done"

all: 
	@clear
	@make libs
	@make prog
	@make clean