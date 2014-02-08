// execute.c - Execution Engine

#include "pigtypes.h"
#include <string.h>

#define MAX_LINE	512
#define MAX_ITEM	80

int executeLoad( char* relation_name, char* filename, char delimiter )
{
  int ret;
  relation_t* relation;
  char line[MAX_LINE+1];
  char item[MAX_ITEM+1];
  char delim[2]={delimiter, 0};
  char *token;
  FILE* fp;

  fp = fopen(filename, "r");
  if (fp == (FILE*)0)
  {
    printf("Unable to open %s\n", filename);
    ret = -1;
  } 
  else 
  {

//    printf("executeLoad: relation %s, filename %s, delim %c\n", 
//            relation_name, filename, delim);

    relation = allocateRelation( relation_name );
    if (relation)
    {
      addTupleToRelation( allocateTuple(), relation );

      while(fgets(line, 512, fp) != NULL)
      {

        printf("line: %s", line);

        // Start tokenizing input...
        token = strtok(line, delim);

        while (token)
        {

          trimwhitespace( item, MAX_ITEM, token );

          printf("token: %s\n", item);

          token = strtok(NULL, delim);

        }

      }
    
    }
    else
    {
      printf("Unable to create relation %s.\n", relation_name);
      ret = -1;
    }

  }

  return ret;
}

