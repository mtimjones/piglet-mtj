
piglet: main.c parse/parser.c relations.c execute.c tuples.c helpers.c elements.c parse/simparse.c interp/interp.c
	gcc -Wall -g -I ./ -o $@ $^

clean:
	rm -f piglet
