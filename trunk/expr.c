// Postfix expression parser

#include <stdlib.h>
#include "pigtypes.h"

element_t* target_element;
char* target_name;
int   target_index;

void field_iter( element_t* element )
{
  int index = -1;

  /* If we're using postional notation, get the index. */
  if (target_name[0] == '$')
  {
    sscanf( &target_name[1], "%d", &index );
    assert( index >= 0 );
    assert( index < 20 );
    if (target_index == index) target_element = element;
  }
  else /* Using named notation */
  {
    if (!strcmp(target_name, element->name))
    {
      target_element = element;
    }
  }

  target_index++;

  return;
}


element_t* retrieveElement( tuple_t* tuple, char* name )
{
  assert( tuple );
  assert( name );

  /* Initialize the return element pointer. */
  target_element = (element_t*)0;

  target_name = name;
  target_index = 0;

  iterateElements( tuple, field_iter );

  return target_element;
}


element_t* copyElement( element_t* source )
{
  element_t* target = allocateElement( );

  if (!source)
  {
    target->type = NUL;
    return target;
  }

  strcpy( target->name, source->name );
  target->type = source->type;
  target->size = source->size;

  switch( source->type )
  {
    case LONG:
      target->u.l = source->u.l;
      break;
    case DOUBLE:
      target->u.g = source->u.g;
      break;
    case BYTEARRAY:
      {
        int i;
        target->u.b = allocateByteArray( target->size+1 );
        for ( i = 0 ; i < source->size ; i++ )
        {
          target->u.b[i] = source->u.b[i];
        }
        target->u.b[i] = 0;
      }
      break;
    case CHARARRAY:
      {
        int i;
        target->u.s = malloc( target->size+1 );
        for ( i = 0 ; i < source->size ; i++ )
        {
          target->u.s[i] = source->u.b[i];
        }
        target->u.s[i] = 0;
      }
      break;
    case BOOLEAN:
      target->u.l = source->u.l;
      break;
    case TUPLE:
      assert(0);
    case NUL:
      break;
    default:
      assert(0);
  }

  return target;
}


void convertElement( element_t* element, int type )
{
  long larg;
  double darg;

  if ( element->type == type ) return;

  if        ( element->type == BYTEARRAY )
  {
    switch( type )
    {
      case CHARARRAY:
        element->type = CHARARRAY;
        break;
      case LONG:
        larg = atoi( (char *)element->u.b );
        element->u.l = larg;
        element->type = LONG;
        break;
      case DOUBLE:
        sscanf( (char *)element->u.b, "%lg", &darg );
        element->u.g = darg;
        element->type = DOUBLE;
        break;
      case BOOLEAN:
        assert(0);
        break;
    }
  } else if ( element->type == CHARARRAY )
  {
    switch( type )
    {
      case BYTEARRAY:
        element->type = BYTEARRAY;
        break;
      case LONG:
        larg = atoi( (char *)element->u.s );
        element->u.l = larg;
        element->type = LONG;
        break;
      case DOUBLE:
        sscanf( (char *)element->u.s, "%lg", &darg );
        element->u.g = darg;
        element->type = DOUBLE;
        break;
      case BOOLEAN:
        assert(0);
        break;
    }
  } else if ( element->type == LONG )
  {
    switch( type )
    {
      case CHARARRAY:
      case BYTEARRAY:
        larg = element->u.l;
        element->u.s = malloc( MAX_LONG_STRING+1 );
        snprintf( element->u.s, MAX_LONG_STRING, "%ld", larg );
        element->type = type;
        break;
      case DOUBLE:
        element->u.g = (double)element->u.l;
        element->type = DOUBLE;
        break;
      case BOOLEAN:
        assert(0);
        break;
    }
  } else if ( element->type == DOUBLE )
  {
    switch( type )
    {
      case CHARARRAY:
      case BYTEARRAY:
        darg = element->u.g;
        element->u.s = malloc( MAX_DOUBLE_STRING+1 );
        snprintf( element->u.s, MAX_DOUBLE_STRING, "%lg", darg );
        element->type = type;
        break;
      case LONG:
        element->u.l = (long)element->u.g;
        element->type = LONG;
        break;
      case BOOLEAN:
        assert(0);
        break;
    }

  } else if ( element->type == BOOLEAN )
  {
    assert(0);
  }

  return;
}

