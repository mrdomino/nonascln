CFLAGS = -std=c89 -Wall -Wextra -Werror -pedantic -ansi -Os
PREFIX = /usr/local

all: nonascln

clean:
	-rm nonascln

install:
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp nonascln ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/nonascln

.PHONY: all clean install
