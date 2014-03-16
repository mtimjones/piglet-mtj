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


int compareElements( element_t* elem1, element_t* elem2 )
{
  assert(elem1); assert(elem2);

  // Both types should be the same...
  assert( elem1->type == elem2->type );

  if        ( elem1->type == CHARARRAY )
  {
    return ( strcmp( elem1->u.s, elem2->u.s ) < 0);
  } else if ( elem1->type == BYTEARRAY )
  {
    return ( strcmp( elem1->u.s, elem2->u.s ) < 0);
  } else if ( elem1->type == LONG )
  {
    return ( elem1->u.l < elem2->u.l );
  } else if ( elem1->type == DOUBLE )
  {
    return ( elem1->u.g < elem2->u.g );
  } else if ( elem1->type == BOOLEAN )
  {
    return ( elem1->u.l < elem2->u.l );
  }

  assert(0);

  return 0;
}

