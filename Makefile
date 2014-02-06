
piglet: main.c parser.c
	gcc -Wall -g -o $@ $^

clean:
	rm -f piglet
