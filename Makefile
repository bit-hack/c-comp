expr: expr.c
	gcc expr.c -O0 -g -o expr

clean:
	rm -rf expr
