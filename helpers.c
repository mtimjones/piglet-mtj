// Via Stackoverflow

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "pigtypes.h"


int trimwhitespace(char *out, int len, const char *str)
{
  if(len == 0)
    return 0;

  const char *end;
  int out_size;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
  {
    *out = 0;
    return 1;
  }

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;
  end++;

  // Set output size to minimum of trimmed string length and buffer size minus 1
  out_size = (end - str) < len-1 ? (end - str) : len-1;

  // Copy trimmed string and add null terminator
  memcpy(out, str, out_size);
  out[out_size] = 0;

  return out_size;
}


void listAdd( struct link_s* list, struct link_s* item )
{
  assert(list);
  assert(item);

  item->next = item->prev = NULL;

  if (list->prev) 
  {
    // Non-empty list, item added to the tail
    list->prev->next = item;
    item->prev = list->prev;
    list->prev = item;
  }
  else
  {
    // Empty list, single item added
    list->next = item;
    list->prev = item;
  }

  return;
}


void *allocateByteArray( int size )
{
  return calloc( 1, size );
}

void printByteArray( element_t* element )
{
  int i;

  assert( element->type == BYTEARRAY);

  for (i = 0 ; i < element->size)
  {
    printf("%c", element->u.b[i]);
  }

  return;
}

