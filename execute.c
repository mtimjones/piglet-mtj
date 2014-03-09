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
          element->u.b = allocateByteArray( element->size+1 );
          element->type = BYTEARRAY;
          
          for (i = 0 ; i < element->size ; i++)
          {
            element->u.b[i] = item[i];
          }
          element->u.b[i] = 0;

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
  printf("  {");
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

  printf( "%s = {\n", relation_name );

  iterateRows( relation, row_iter );

  printf("}\n\n");

  return 0;
}


static relation_t* output_relation;
static expr_t* exprs;

void foreach_iter( tuple_t* tuple )
{
  tuple_t* output_tuple;

  /* The expressions list contains the elements that are required to be
   * written to the output relation.  This process is then for each row,
   * iterate the expressions, compute the result, and then output the 
   * resulting object to the new row for the output relation.
   */

  int i;

  setCurrentTuple( tuple );

  output_tuple = allocateTuple( );

  for (i = 0 ; i < MAX_EXPR ; i++)
  {

    if (exprs[i].active)
    {

      // Execute the expression
      element_t* element = interpret_go( exprs[i].expr_str );

      convertElement( element, exprs[i].type );
      strcpy( element->name, exprs[i].name );

//      printElement( element );
//      printf("\n");

      // Afterwards, the element on the stack should be added to the
      // current row.

      addElementToTuple( element, output_tuple );

    } else break;

  }
//  printf("\n");

  // Add the new tuple to the output_relation
  addTupleToRelation( output_tuple, output_relation );

  setCurrentTuple( (tuple_t*)0 );

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


int executeDescribe( char* relation_name )
{
  relation_t* relation;
  tuple_t* tuple;
  link_t* link;
  element_t* element;

  relation = findRelation( relation_name );

  if (!relation) 
  {
    printf("Could not Describe relation, not found.\n");
    return -1;
  }

  tuple = (tuple_t*)relation->tuple_list.first;

  printf( "%s = {\n  ", relation_name );
  for ( link = tuple->element_list.first ; link ; link = link->next )
  {
    element = (element_t*)link;
    printf( "%s:", element->name );
    printType( element->type );
    if ( link->next ) printf(", ");
  }
  printf("\n}\n\n");

  return 0;
}


static void filter_iter( tuple_t* tuple )
{
  tuple_t* output_tuple;

  assert(tuple);

  setCurrentTuple( tuple );

  // Execute the expression
  element_t* element = interpret_go( exprs->expr_str );

  assert( element->type == BOOLEAN );

printf("element->u.l %ld\n", element->u.l);

  // If the result was TRUE, propagate the tuple.
  if ( element->u.l == TRUE )
  {
    output_tuple = allocateTuple( );

    copyTuple( output_tuple, tuple );

    // Add the new tuple to the output_relation
    addTupleToRelation( output_tuple, output_relation );
  }

  setCurrentTuple( (tuple_t*)0 );

  return;
}



int executeFilter( relation_t* input, relation_t* output, expr_t* expression )
{
  assert(input);
  assert(output);
  assert(expression);

//  printf("\tName: %s\n", expression->name);
//  printf("\tType: %d\n", expression->type);
//  printf("\tExpr: %s\n", expression->expr_str);

  output_relation = output;
  exprs = expression;

  iterateRows( input, filter_iter );

  return 0;
}

