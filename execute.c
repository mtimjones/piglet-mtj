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
  printElement( element );
  if (element->node.next != NULL) printf(",");
}


void row_iter( tuple_t* tuple )
{
  printf("{");
  iterateElements( tuple, elem_iter );  
  printf("}\n");

  return;
}


int executeDump( char* relation_name )
{
  relation_t* relation;

  relation = findRelation( relation_name );

  if (!relation) 
  {
    printf("Could not Dump relation, not found.\n");
    return -1;
  }

  iterateRows( relation, row_iter );

  return 0;
}

static relation_t* output_relation;
static expr_t* exprs;

void foreach_iter( tuple_t* tuple )
{
  /* The expressions list contains the elements that are required to be
   * written to the output relation.  This process is then for each row,
   * iterate the expressions, compute the result, and then output the 
   * resulting object to the new row for the output relation.
   */

  int i;

  // Allocate a tuple

  for (i = 0 ; i < MAX_EXPR ; i++)
  {
    if (exprs[i].active)
    {

      // Execute the expression

      // Afterwards, the element on the stack should be added to the
      // current row.

    } else break;
  }

  return;
}


int executeForeach( relation_t* input, relation_t* output, expr_t* expressions )
{
#if 0
  int i;
  for (i = 0 ; i < MAX_EXPR ; i++)
  {
    if (expressions[i].active)
    {
      printf("Expression #%d\n", i);
      printf("\tName: %s\n", expressions[i].name);
      printf("\tType: %d\n", expressions[i].type);
      printf("\tExpr: %s\n", expressions[i].expr_str);

    }
  }
#endif

  output_relation = output;
  exprs = expressions;

  iterateRows( input, foreach_iter );

  return 0;
}

