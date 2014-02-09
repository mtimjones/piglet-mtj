// execute.c - Execution Engine

#include "pigtypes.h"
#include <string.h>

#define MAX_LINE	512
#define MAX_ITEM	80

int executeLoad( char* relation_name, char* filename, char delimiter )
{
  int ret, i;
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

      while(fgets(line, 512, fp) != NULL)
      {
        tuple_t* tuple = allocateTuple();
        addTupleToRelation( tuple, relation );

//        printf("line: %s", line);

        // Start tokenizing input...
        token = strtok(line, delim);

        while (token)
        {

          element_t* element = allocateElement();

          trimwhitespace( item, MAX_ITEM, token );

          element->size = strlen( item );
          element->u.b = allocateByteArray( element->size );
          element->type = BYTEARRAY;
          
          for (i = 0 ; i < element->size ; i++)
          {
            element->u.b[i] = item[i];
          }

          addElementToTuple( element, tuple );

//          printf("token: %s\n", item);

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


void elem_iter( element_t* element )
{
//  print_element( element );
    printByteArray( element );
    printf("\t");
}


void row_iter( tuple_t* tuple )
{
  iterateElements( tuple, elem_iter );  
  printf("\n");

  return;
}

int executeDump( char* relation_name )
{
  relation_t* relation;

  relation = findRelation( relation_name );

  if (!relation) return -1;

  iterateRows( relation, row_iter );

  return 0;
}

