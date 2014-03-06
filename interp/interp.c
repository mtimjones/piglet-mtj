// Interpreter.c

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include "pigtypes.h"

#define MAX_STACK	40

static element_t*  stack[MAX_STACK];
static int  stack_index;


void interp_init( void )
{
  stack_index = 0;
  return;
}

element_t* createLongConst( int value )
{
  element_t* elem_ptr = malloc( sizeof(element_t) );

  // TODO: Need to handle double values...
  elem_ptr->type = LONG;
  elem_ptr->u.l = value;

  return elem_ptr;
}

static void push( element_t* elem_ptr )
{
  assert( elem_ptr );
  stack[stack_index++] = elem_ptr;;
  return;
}

static element_t* pop( void )
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


static void op_add( void )
{
  element_t *op1, *op2;

  op1 = pop(); op2 = pop();
  assert( op1 ); assert( op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = op1->u.l + op2->u.l;
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.g = op1->u.g + op2->u.g;
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}

static void op_sub( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = op1->u.l - op2->u.l;
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.g = op1->u.g - op2->u.g;
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}

static void op_mul( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = op1->u.l * op2->u.l;
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.g = op1->u.g * op2->u.g;
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}

static void op_div( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = op1->u.l / op2->u.l;
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.g = op1->u.g / op2->u.g;
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}

static void op_gt( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = ( op1->u.l > op2->u.l );
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.l = ( op1->u.g > op2->u.g );
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}
  
static void op_gte( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = ( op1->u.l >= op2->u.l );
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.l = ( op1->u.g >= op2->u.g );
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}
  
static void op_lt( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = ( op1->u.l < op2->u.l );
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.l = ( op1->u.g < op2->u.g );
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}
  
static void op_lte( void )
{
  element_t *op1, *op2;

  op2 = pop( ); op1 = pop( );
  assert( op1 ); assert( op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = ( op1->u.l <= op2->u.l );
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.l = ( op1->u.g <= op2->u.g );
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}

static void op_eq( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = ( op1->u.l == op2->u.l );
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.l = ( op1->u.g == op2->u.g );
    op1->type = BOOLEAN;
    free( (void*)op2 ); push( op1 );
  }
  else if ( ( op1->type == BYTEARRAY ) && ( op2->type == BYTEARRAY ) )
  {
    int i;
    int result = TRUE;

    if ( op1->size != op2->size ) result = 0;
    else
    {
      for ( i = 0 ; i < op1->size ; i++ )
      {
        if ( op1->u.b[i] != op2->u.b[i])
        {
          result = FALSE; break;
        }
      }
    }

    free( (void*)op2 );

    op1->u.l = result;
    op1->type = BOOLEAN;
    push( op1 );
  }
  else if ( ( op1->type == CHARARRAY ) && ( op2->type == CHARARRAY ) )
  {
    int result = FALSE;

    if (!strcmp( op1->u.s, op2->u.s )) result = TRUE;

    free( op2 );

    op1->u.l = result;
    op1->type = LONG;
    push( op1 );
  }
  else assert( 0 );

  return;
}


element_t* interpret_go( char* instr )
{
  char *token;

  assert( instr );

  token = strtok( instr, " " );

  while (token)
  {

    // Push element value
    if (token[0] == '$')
    {
      element_t* element;
      // Push a copy of the element
      element = copyElement( retrieveElement( token ) );
      push( element );
    }
    else if (isNumber(token))           push( createLongConst( atoi(token) ) );
    else if (!strncmp(token, ">=", 2))  op_gte();
    else if (!strncmp(token, "<=", 2))  op_lte();
    else if (!strncmp(token, "==", 2))  op_eq();
    else if (token[0] == '<')           op_lt();
    else if (token[0] == '>')           op_gt();
    else if (token[0] == '*')           op_mul();
    else if (token[0] == '/')           op_div();
    else if (token[0] == '+')           op_add();
    else if (token[0] == '-')           op_sub();
    else assert(0);

    // Get next token
    token = strtok( NULL, " " );
  }

  // Return top element
  return pop();
}

