// Elements Functions

#include "pigtypes.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>


element_t* allocateElement( void )
{
  return calloc(1, sizeof(element_t));
}


void printElement( element_t* element )
{
  assert(element);

  switch( element->type )
  {
    case LONG:
      printf("%ld\n", element->u.l);
      break;

    case DOUBLE:
      printf("%g\n", element->u.g);
      break;

    case BYTEARRAY:
    case CHARARRAY:
      printByteArray( element );
      break;

    case TUPLE:
      // emit a bag.
      break;
    
    default:
      assert(0);

  }

  return;
}
