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
  else
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

  // No element was found, allocate a NUL and return.
  if (!target_element)
  {
    target_element = allocateElement( );
    target_element->type = NUL;
    strcpy( target_element->name, name );
  }

  return target_element;
}

