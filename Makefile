
piglet: main.c parser.c relations.c execute.c tuples.c helpers.c elements.c
	gcc -Wall -g -o $@ $^

clean:
	rm -f piglet
