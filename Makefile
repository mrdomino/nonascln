CFLAGS=-std=c89 -Wall -Wextra -Werror -pedantic -ansi -Os

all: nonascln

clean:
	-rm nonascln

.PHONY: all clean
