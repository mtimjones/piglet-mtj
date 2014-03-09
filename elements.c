// Elements Functions

#include "pigtypes.h"
#include <string.h>
#include <stdlib.h>


element_t* allocateElement( void )
{
  return calloc(1, sizeof(element_t));
}


// Move copyElement here...


void printElement( element_t* element )
{
  assert(element);

  switch( element->type )
  {
    case LONG:
      printf("%ld", element->u.l);
      break;

    case DOUBLE:
      printf("%g", element->u.g);
      break;

    case BYTEARRAY:
    case CHARARRAY:
      printByteArray( element );
      break;

    case NUL:
      printf("NUL");
      break;

    case TUPLE:
      // emit a bag.
      break;

    case BOOLEAN:
      if (element->u.l) printf("TRUE");
      else printf("FALSE");
      break;
    
    default:
      assert(0);

  }

  return;
}


void printType( int type )
{
  switch( type )
  {
    case LONG:
      printf("LONG");
      break;
    case DOUBLE:
      printf("DOUBLE");
      break;
    case BYTEARRAY:
      printf("BYTEARRAY");
      break;
    case CHARARRAY:
      printf("CHARARRAY");
      break;
    case BOOLEAN:
      printf("BOOLEAN");
      break;
    default:
      assert(0);
  }
  return;
}
