CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC = menu.c index_first_even.c index_last_odd.c sum_between_even_odd.c sum_before_even_and_after_odd.c
OBJ = $(SRC:.c=.o)

TARGET = menu

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
$(CC) $(CFLAGS) $^ -o $@

%.o: %.c %.h
$(CC) $(CFLAGS) -c $< -o $@

clean:
rm -f $(OBJ) $(TARGET)
