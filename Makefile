CFLAGS=-O0 -g

TEST_CFLAGS=\
 -fsyntax-only\
 -Wno-implicit-function-declaration\
 -Wno-overflow

parse: parse.c util.c defs.h
	# note: $@ is the rule name
	gcc parse.c util.c ${CFLAGS} -o $@

test: parse
	# note: the @ prefix stops echoing
	@for FILE in tests/*.c; do \
		echo "Testing $$FILE"; \
		gcc ${TEST_CFLAGS} $$FILE; \
		./parse $$FILE; \
	done

.PHONY: clean
clean:
	rm -rf parse
