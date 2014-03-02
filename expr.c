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


element_t* getFieldFromExpr( tuple_t* tuple, char* name )
{
  int i;

  assert( tuple );
  assert( name );

  /* Initialize the return element pointer. */
  element = (element*)0;

  target_name = name;
  target_index = 0;

  iterateElements( tuple, field_iter );


   

}
