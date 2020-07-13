CC = gcc
CFLAGS  = -g -Wall
TARGET = main
LDFLAGS = -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp -ldl
CFILES = $(TARGET).c

prog:
	@echo "Building..."
	@$(CC) $(CFLAGS) -o $(TARGET) $(CFILES) $(LDFLAGS) -L/home/corrot/Desktop/C_Projekty/test21 mathOpengl.a
	@echo "...done!"
	@./main

libs:
	@echo "Generating libraries..."
	@gcc $(CFLAGS) -O -c mathOpengl.c
	@ar rcs mathOpengl.a mathOpengl.o
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