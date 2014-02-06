// Piglet
// Simple parser (brute-force for the simple syntax)

#include <stdio.h>
#include "pigtypes.h"

int parseLoad( char* line )
{
  char relation[MAX_RELATION_NAME];
  char filename[MAX_FILENAME];
  char delim;
  int ret;
  
  ret = sscanf(line, "%s = LOAD %s DELIM '%c';", relation, filename, &delim);

  printf(" relation %s, filename %s, delim %c\n", relation, filename, delim);

  return ret;
}

int parseDump( char* line )
{
  char relation[MAX_RELATION_NAME];
  int  ret;

  ret = sscanf(line, "DUMP %s;", relation);

  printf(" relation %s\n", relation);

  return ret;
}

