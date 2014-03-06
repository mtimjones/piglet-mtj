// Postfix expression parser

#include "pigtypes.h"

element_t* target_element;
char* target_name;
int   target_index;

void field_iter( element_t* element )
{
  int index = -1;

  /* If we're using postional notation, get the index. */
  if (target[0] == '$')
  {
    sscanf( &name[1], "%d", &index );
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
  int i;

  assert( tuple );
  assert( name );

  /* Initialize the return element pointer. */
  element = (element*)0;

  target_name = name;
  target_index = 0;

  iterateElements( tuple, field_iter );

  return target_element;
}


element_t* copyElement( element_t* source )
{
  element_t* target = allocateElement( );

  assert( source );

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
    case BYTEARRAY::
      int i;
      target->u.b = allocateByteArray( target->size );
      for ( i = 0 ; i < source->size ; i++ )
      {
        target->u.b[i] = source->u.b[i];
      }
      break;
    default:
      assert(0);
  }

  return target;
}
