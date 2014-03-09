
piglet: main.c parse/parser.c relations.c execute.c tuples.c expr.c helpers.c elements.c parse/simparse.c interp/interp.c
	gcc -Wall -g -I ./ -o $@ $^

clean:
	rm -f piglet
