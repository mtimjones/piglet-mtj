# Simple Makefile for Piglet

piglet: main.c parse/parser.c types/relations.c exec/execute.c types/tuples.c types/expr.c misc/helpers.c types/elements.c parse/simparse.c interp/interp.c
	gcc -Wall -g -I ./include/ -o $@ $^

clean:
	rm -f piglet
