parse: parse.c util.c defs.h
	# note: $@ is the rule name
	gcc parse.c util.c -O0 -g -o $@

test: parse
	# note: the @ prefix stops echoing
	@for FILE in tests/*.c; do \
		echo "Testing $$FILE"; \
		gcc -fsyntax-only $$FILE; \
		./parse $$FILE; \
	done

clean:
	rm -rf parse
