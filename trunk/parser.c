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
  
  ret = sscanf(line, "%s = LOAD %s DELIM '%c'", relation, filename, &delim);

  // Expecting 3 parsed arguments.
  if (ret == 3)
  {
    ret = executeLoad( relation, filename, delim ); 
  }

  return ret;
}

int parseDump( char* line )
{
  char relation[MAX_RELATION_NAME];
  int  ret;

  ret = sscanf(line, "DUMP %s", relation);

  // Expecting 1 parsed argument.
  if (ret == 1)
  {
    ret = executeDump( relation );
  }

  return ret;
}

