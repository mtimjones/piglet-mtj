// Elements Functions

#include "pigtypes.h"
#include <string.h>
#include <stdlib.h>


element_t* allocateElement( void )
{
  return calloc(1, sizeof(element_t));
}

