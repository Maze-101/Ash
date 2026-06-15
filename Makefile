CC = gcc
CFLAGS = -Wall -Wextra -g
OBJ = main.o parser.o executor.o
TARGET = ash

$(TARGET): $(OBJ)
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c ash.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f *.o $(TARGET)