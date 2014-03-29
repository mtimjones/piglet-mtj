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


static void push( element_t* elem_ptr )
{
  assert( elem_ptr );
  stack[stack_index++] = elem_ptr;;
  return;
}

void ext_push( element_t* elem_ptr )
{
  push( elem_ptr );
  return;
}

static element_t* pop( void )
{
  assert(stack_index != 0);

  return stack[--stack_index];
}


static int isNumber( char *str )
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


static int isDouble( char *str )
{
  int len = 0;
  assert(str);

  if (*str == '-')
    ++str;

  while (*str)
  {
    // Need to fix (no more than one '.'
    if (!isdigit(*str) || (*str != '.')) return 0;
    else ++str;
    len++;
  }

  if (len == 0) return 0;
  else return 1;
}


static element_t* convertStringToLongElement( char* string )
{
  element_t* element = allocateElement( );

  element->type = LONG;
  element->u.l = atoi( string );

  return element;
}


static element_t* convertStringToDoubleElement( char* string )
{
  element_t* element = allocateElement( );

  element->type = DOUBLE;
  sscanf( string, "%lg", &element->u.g );

  return element;
}

// string -> LONG or DOUBLE
// LONG DOUBLE -> DOUBLE DOUBLE
// DOUBLE LONG -> DOUBLE DOUBLE


element_t* convertStringToElement( char* string )
{
  element_t* element;
  int i=1;

  element = allocateElement( );
  element->type = CHARARRAY;
  element->u.s = malloc( MAX_NAME+1 );

  assert( string[0] == '\"' );

  while ( ( i < MAX_NAME ) && ( string[i] != '\"' ) )
  {
    element->u.s[i-1] = string[i];
    i++;
  }
  element->u.s[i-1] = 0;

  element->size = strlen( element->u.s );

  return element;
}


static void convertStringToScalar( element_t* element )
{
  long larg;
  double darg;
  assert( element );

  if        ( element->type == CHARARRAY )
  {
    if ( isNumber( (char *)element->u.s ) )
    {
      element->type = LONG;
      larg = atoi( element->u.s );
      free( element->u.s );
      element->u.l = larg;
    } else if ( isDouble( (char *)element->u.s ) )
    {
      sscanf( element->u.s, "%lg", &darg );
      element->type = DOUBLE;
      free( element->u.s );
      element->u.g = darg;
    }
  } else if ( element->type == BYTEARRAY )
  {
    if ( isNumber( (char *)element->u.b ) )
    {
      element->type = LONG;
      larg = atoi( (const char *)element->u.b );
      free( element->u.b );
      element->u.l = larg;
    } else if ( isDouble( (char *)element->u.b ) )
    {
      sscanf( (const char *)element->u.b, "%lg", &darg );
      element->type = DOUBLE;
      free( element->u.b );
      element->u.g = darg;
    }
  }
  return;
}


static void normalize_types( element_t* op1, element_t* op2 )
{
  assert( op1 );
  assert( op2 );

  convertStringToScalar( op1 );
  convertStringToScalar( op2 );

  if        ( ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    return;
  } else if ( ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    return;
  } else if ( ( op1->type == LONG ) && ( op2->type == DOUBLE ) )
  {
    op1->u.g = (double)op1->u.l;
    op1->type = DOUBLE;
  } else if ( ( op1->type == DOUBLE ) && ( op2->type == LONG ) )
  {
    op2->u.g = (double)op2->u.l;
    op2->type = DOUBLE;
  }

  return;
}



static void op_add( void )
{
  element_t *op1, *op2;

  op1 = pop(); op2 = pop();
  assert( op1 ); assert( op2 );

  normalize_types( op1, op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = op1->u.l + op2->u.l;
    op1->type = LONG;
    freeElement( op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.g = op1->u.g + op2->u.g;
    op1->type = DOUBLE;
    freeElement( op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}

static void op_sub( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  normalize_types( op1, op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = op1->u.l - op2->u.l;
    op1->type = LONG;
    freeElement( op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.g = op1->u.g - op2->u.g;
    op1->type = DOUBLE;
    freeElement( op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}

static void op_mul( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  normalize_types( op1, op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = op1->u.l * op2->u.l;
    op1->type = LONG;
    freeElement( op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.g = op1->u.g * op2->u.g;
    op1->type = DOUBLE;
    freeElement( op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}

static void op_div( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  normalize_types( op1, op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = op1->u.l / op2->u.l;
    op1->type = LONG;
    freeElement( op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.g = op1->u.g / op2->u.g;
    op1->type = DOUBLE;
    freeElement( op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}

static void op_gt( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  normalize_types( op1, op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = ( op1->u.l > op2->u.l );
    op1->type = BOOLEAN;
    freeElement( op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.l = ( op1->u.g > op2->u.g );
    op1->type = BOOLEAN;
    freeElement( op2 ); push( op1 );
  }
  else if ( (  op1->type == NUL) || ( op2->type == NUL ) )
  {
    op1->u.l = 0;
    op1->type = BOOLEAN;
    freeElement( op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}
  
static void op_gte( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  normalize_types( op1, op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = ( op1->u.l >= op2->u.l );
    op1->type = BOOLEAN;
    freeElement( op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.l = ( op1->u.g >= op2->u.g );
    op1->type = BOOLEAN;
    freeElement( op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}
  
static void op_lt( void )
{
  element_t *op1, *op2;

  op2 = pop(); op1 = pop();
  assert( op1 ); assert( op2 );

  normalize_types( op1, op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = ( op1->u.l < op2->u.l );
    op1->type = BOOLEAN;
    freeElement( op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.l = ( op1->u.g < op2->u.g );
    op1->type = BOOLEAN;
    freeElement( op2 ); push( op1 );
  }
  else assert( 0 );

  return;
}
  
static void op_lte( void )
{
  element_t *op1, *op2;

  op2 = pop( ); op1 = pop( );
  assert( op1 ); assert( op2 );

  normalize_types( op1, op2 );

  if      (  ( op1->type == LONG ) && ( op2->type == LONG ) )
  {
    op1->u.l = ( op1->u.l <= op2->u.l );
    op1->type = BOOLEAN;
    freeElement( op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.l = ( op1->u.g <= op2->u.g );
    op1->type = BOOLEAN;
    freeElement( op2 ); push( op1 );
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
    freeElement( op2 ); push( op1 );
  }
  else if (  ( op1->type == DOUBLE ) && ( op2->type == DOUBLE ) )
  {
    op1->u.l = ( op1->u.g == op2->u.g );
    op1->type = BOOLEAN;
    freeElement( op2 ); push( op1 );
  }
  else if ( ( ( op1->type == BYTEARRAY ) || ( op1->type == CHARARRAY ) ) &&
            ( ( op2->type == CHARARRAY ) || ( op2->type == BYTEARRAY ) ) )
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
  else if ( ( op1->type == NUL ) && ( op2->type == NUL ) )
  {
    op1->u.l = 1;
    op1->type = BOOLEAN;
    push( op1 );
  }
  else if ( ( op1->type == NUL ) || ( op2->type == NUL ) )
  {
    op1->u.l = 0;
    op1->type = BOOLEAN;
    push( op1 );
  }
  else assert( 0 );

  return;
}


static void op_and( void )
{
  element_t *op1, *op2;

  op1 = pop( ); op2 = pop( );
  assert( op1 ); assert( op2 );

  assert( op1->type == BOOLEAN );
  assert( op2->type == BOOLEAN );

  op1->u.l = ( op1->u.l && op2->u.l );
  push( op1 );
  free( (void *)op2 );

  return;
}


static void op_or( void )
{
  element_t *op1, *op2;

  op1 = pop( ); op2 = pop( );
  assert( op1 ); assert( op2 );

  assert( op1->type == BOOLEAN );
  assert( op2->type == BOOLEAN );

  op1->u.l = ( op1->u.l || op2->u.l );
  push( op1 );
  free( (void *)op2 );

  return;
}


static void op_strlen( void )
{
  element_t *op1;

  op1 = pop();
  assert( op1 );

  if ( ( op1->type == CHARARRAY ) || ( op1->type == BYTEARRAY ) )
  {
    int len;
    len = strlen( op1->u.s );

    free( op1->u.s );
    op1->type = LONG;
    op1->u.l = (long)len;
    push( op1 );

  }
  else
  {
    // Load a -1 -- error?!?
    free( op1->u.s );
    op1->type = LONG;
    op1->u.l = -1;
    push( op1 );
  }

  return;
}


static void op_count( void )
{
  element_t *op1;
  relation_t *relation;
  link_t* link;
  int count = 0;

  op1 = pop();
  assert(op1);

  relation = (relation_t*)op1->u.r;

  assert( op1->type == RELATION );

  for (link = relation->tuple_list.first ; link ; link = link->next)
  {
    count++;
  }

  free(op1);

  op1 = allocateElement( );
  assert(op1);
  op1->type = LONG;
  op1->u.l = count;

  push( op1 );

  return;
}


void interpret_init( void )
{
  stack_index = 0;
  return;
}


element_t* interpret_go( char* instr )
{
  char *local;
  char *token;

  assert( instr );

  local = malloc( strlen( instr ) );
  strcpy( local, instr );

  token = strtok( local, " " );

  while (token)
  {
    if      ( token[0] == '\"' ) push( convertStringToElement( token ) );
    else if ( isDouble(token) ) push( convertStringToDoubleElement( token ) );
    else if ( isNumber(token) ) push( convertStringToLongElement( token ) );
    else if ( !strncmp(token, ">=", 2 ) )  op_gte();
    else if ( !strncmp(token, "<=", 2 ) )  op_lte();
    else if ( !strncmp(token, "==", 2 ) )  op_eq();
    else if ( token[0] == '<' )            op_lt();
    else if ( token[0] == '>' )            op_gt();
    else if ( token[0] == '*' )            op_mul();
    else if ( token[0] == '/' )            op_div();
    else if ( token[0] == '+' )            op_add();
    else if ( token[0] == '-' )            op_sub();
    else if ( !strncmp(token, "STRLEN", 6 ) ) op_strlen();
    else if ( !strncmp(token, "COUNT", 5 ) )  op_count();
    else if ( !strncmp(token, "&&", 2 ) )  op_and();
    else if ( !strncmp(token, "||", 2 ) )  op_or();
    else if ( ( token[0] == '$' ) || ( isalpha( token[0] ) ) )
    {
      element_t* element;
      // Push a copy of the element
      element = copyElement( retrieveElement( getCurrentTuple(), token ) );
      push( element );
    }

    else 
    {
      assert(0);
    }

    // Get next token
    token = strtok( NULL, " " );
  }

  free( local );

  assert( stack_index == 1 );

  // Return top element
  return pop();
}

