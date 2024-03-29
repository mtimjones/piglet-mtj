// Relations Functions

#include "pigtypes.h"
#include <string.h>
#include <stdlib.h>

#define MAX_RELATIONS		128

relation_t* relations[MAX_RELATIONS];

void initRelations( void )
{
  int i;

  for (i = 0 ; i < MAX_RELATIONS ; i++) 
  {
    relations[i] = (relation_t*)0;
  }

  return;
}

relation_t* allocateRelation( char* name )
{
  int i, len;

  len = strlen(name);

  // First, check to see if it exists.
  for (i = 0 ; i < MAX_RELATIONS ; i++)
  {
    if (relations[i])
    {
      len = max(len, strlen(relations[i]->name));
      if (!strncmp(name, relations[i]->name, len))
      {
        break;
      }
    }
  }

  // Next, find an empty slot and allocate one.
  for (i = 0 ; i < MAX_RELATIONS ; i++)
  {
    if (!relations[i])
    {
      relations[i] = calloc(1, sizeof(relation_t));
      strncpy(relations[i]->name, name, strlen(name));
      relations[i]->tuple_list.first = relations[i]->tuple_list.last = NULL;
      return relations[i];
    }
  }

  return (relation_t*)0;
}

relation_t* findRelation( char* name )
{
  int i, len;

  len = strlen(name);

  for (i = 0 ; i < MAX_RELATIONS ; i++)
  {
    if (relations[i])
    {
      len = max(len, strlen(relations[i]->name));
      if (!strncmp(name, relations[i]->name, len))
      {
        return relations[i];
      }
    }
  }

  return (relation_t*)0;
}
  
