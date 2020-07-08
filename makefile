CC = gcc
CFLAGS  = -g -Wall
TARGET = main
LDFLAGS = -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp -ldl

all:
	@clear
	@echo "Building..."
	@echo "."
	@$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LDFLAGS)
	@echo "."
	@echo "...done!"
	@./main

clean:
	$(RM) $(TARGET)