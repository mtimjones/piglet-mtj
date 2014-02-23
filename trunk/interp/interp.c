// Interpreter.c

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include "interp.h"

#define MAX_STACK	40

static int  stack[MAX_STACK];
static int  stack_index;


void interp_init( void )
{
  stack_index = 0;
  return;
}

static void push( int value )
{
  stack[stack_index++] = value;
  return;
}

static int pop( void )
{
  assert(stack_index != 0);

  return stack[--stack_index];
}


int isNumber(const char *str)
{
  int len = 0;
  assert(str);

  if (*str == '-')
    ++str;

  while (*str)
  {
    if (!isdigit(*str)) return 0;
    else ++str;
    len++;
  }

  if (len == 0) return 0;
  else return 1;
}


int interpret_go( char* instr )
{
  char *token;

  token = strtok( instr, " " );

  while (token)
  {

    // Push element value
    if (token[0] == '$')
    {
      // temporary
      // !!!! This will push the contents of the field
      push( (int)token[1] );

      // if chararray or bytearray, push pointer...
    }
    else if (isNumber(token))           push( atoi(token) );
    else if (!strncmp(token, ">=", 2))  OP_GTE;
    else if (!strncmp(token, "<=", 2))  OP_LTE;
    else if (!strncmp(token, "==", 2))  OP_EQ;
    else if (token[0] == '<')           OP_LT;
    else if (token[0] == '>')           OP_GT;
    else if (token[0] == '*')           OP_MUL;
    else if (token[0] == '/')           OP_DIV;
    else if (token[0] == '+')           OP_ADD;
    else if (token[0] == '-')           OP_SUB;
    else assert(0);

    token = strtok( NULL, " " );

  }

  return pop();
}

