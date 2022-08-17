CFLAGS=-O0 -g

TEST_CFLAGS=\
 -fsyntax-only\
 -Wno-implicit-function-declaration\
 -Wno-overflow

.PHONY: all clean

all: parse exec

parse: parse.c util.c defs.h
	gcc parse.c util.c ${CFLAGS} -o $@

exec: exec.c defs.h
	gcc exec.c ${CFLAGS} -o $@

# note: the @ prefix stops echoing
test: parse
	@for FILE in tests/*.c; do \
		echo "Testing $$FILE"; \
		gcc ${TEST_CFLAGS} $$FILE; \
		./parse $$FILE; \
	done

clean:
	rm -rf parse exec
