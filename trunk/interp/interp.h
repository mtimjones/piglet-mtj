// Interpreter.h

#ifndef __INTERP_H__
#define __INTERP_H__

#define OP_ADD	push(pop() + pop())
#define OP_SUB	push(pop() - pop())
#define OP_MUL	push(pop() * pop())
#define OP_DIV	push(pop() / pop())
#define OP_GT	push(pop() > pop())
#define OP_GTE	push(pop() >= pop())
#define OP_LT	push(pop() < pop())
#define OP_LTE	push(pop() <= pop())
#define OP_EQ	push(pop() == pop())

void interp_init( void );
int  interpret_go( char* instr );

#endif // __INTERP_H__
