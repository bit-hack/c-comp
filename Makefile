expr: expr.c defs.h
	gcc expr.c -O0 -g -o expr

test: expr
	# note: the @ prefix stops echoing
	@for FILE in tests/*.c; do \
		echo "Testing $$FILE"; \
		gcc -o /dev/null $$FILE; \
		./expr $$FILE; \
	done

clean:
	rm -rf expr
