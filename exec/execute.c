// execute.c - Execution Engine

#include "pigtypes.h"
#include <string.h>
#include <stdlib.h>

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

    relation = allocateRelation( relation_name );
    if (relation)
    {

      while(fgets(line, 512, fp) != NULL)
      {
        tuple_t* tuple = allocateTuple();
        addTupleToRelation( tuple, relation );

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


static FILE* storefp;
static char delimiter;

// @TODO: Does not yet emit relations...
void store_elem_iter( element_t* element )
{
  int i;

  switch( element->type)
  {
    case LONG:
      fprintf( storefp, "%ld", element->u.l );
      break;
    case DOUBLE:
      fprintf( storefp, "%lg", element->u.g );
      break;
    case NUL:
      fprintf( storefp, "NUL" );
      break;
    case TUPLE:
    case RELATION:
      fprintf( storefp, "TBD" );
      break;
    case BOOLEAN:
      fprintf( storefp, "%s", ((element->u.l) ? "TRUE" : FALSE ) );
      break;
    case CHARARRAY:
    case BYTEARRAY:
      for (i = 0 ; i < element->size ; i++) fprintf( storefp, "%c", element->u.s[i] );
      break;
    default:
      assert( 0 );
      break;
  }

  if (element->node.next != NULL) fprintf( storefp, "%c", delimiter );

  return;
}


void store_row_iter( tuple_t* tuple )
{
  iterateElements( tuple, store_elem_iter );  
  fprintf( storefp, "\n" );

  return;
}

int executeStore( char* relation_name, char* filename, char delim )
{
  int ret;
  relation_t* relation;

  storefp = fopen(filename, "w");
  if (storefp)
  {
    relation = findRelation( relation_name );
    if (relation)
    {
      delimiter = delim;
      iterateRows( relation, store_row_iter );
      ret = 0;
    }
    else
    {
      printf("Could not find relation %s\n", relation_name);
      ret = -1;
    }

  }
  else
  {
    printf("Unable to open %s for STORE.\n", filename);
    ret = -1;
  }

  fclose( storefp );

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
  printf("}");

  if (tuple->node.next != NULL) printf(",");
  printf("\n");

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
      interpret_init( );
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
  interpret_init( );
  element_t* element = interpret_go( exprs->expr_str );

  assert( element->type == BOOLEAN );

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
  assert(input); assert(output); assert(expression);

//  printf("\tName: %s\n", expression->name);
//  printf("\tType: %d\n", expression->type);
//  printf("\tExpr: %s\n", expression->expr_str);

  output_relation = output;
  exprs = expression;

  iterateRows( input, filter_iter );

  return 0;
}


static char* sort_field;
static int sort_dir;

static void sort_iter( tuple_t* tuple )
{
  tuple_t* output_tuple;

  assert(tuple);

  output_tuple = allocateTuple( );

  copyTuple( output_tuple, tuple );

  insertTupleSorted( output_tuple, output_relation, sort_field, sort_dir );

  return;
}


int executeSort( relation_t* inp, relation_t* out, char* field, int dir )
{
  assert(inp); assert(out); assert(field);

  output_relation = out;
  sort_field = field;
  sort_dir = dir;

  iterateRows( inp, sort_iter );

  return 0;
}


static char* group_field;

static void group_iter( tuple_t* tuple )
{
  static relation_t* cur_output_relation = (relation_t*)0;
  static element_t* last_element = (element_t*)0;
  element_t *cur_element;
  static int first = 1;

  assert(tuple);

  cur_element = copyElement( retrieveElement( tuple, group_field ) );

  // Internally, the group is represented as follows:
  //
  //  relation->group_tuple( group_name, internal_relation )
  //
  //  internal_relation->( tuple, tuple, ... )
  //

  if (!first)
  {
    char instr[10];
    element_t* ret_element;

    // Create the postfix instructions to execute.
    strcpy(instr, "==");

    interpret_init( );

    // Compare the two elements.
    ext_push( copyElement( cur_element ) );
    ext_push( copyElement( last_element ) );

//printElement( cur_element ); printf("\n");
//printElement( last_element ); printf("\n");

    ret_element = interpret_go( instr );

    assert( ret_element->type == BOOLEAN );

//printf("Compare result %ld\n", ret_element->u.l);

    // If they're different, it's a new group.
    if (ret_element->u.l == 0) 
    {
//      freeElement( last_element );
//      last_element = copyElement( cur_element );
      first = 1;
    }

    freeElement( ret_element );
  }

  // If we're working on a new group, allocate a new root tuple
  if (first)
  {
    element_t *group_element, *internal_tuple;
    tuple_t* group_tuple;

    // Store the current element we're working with (for aggregation).
    if (last_element) freeElement( last_element );
    last_element = copyElement( cur_element );

    group_tuple = allocateTuple( );

    // Grab the group element and copy in into the current tuple.
    group_element = copyElement( retrieveElement( tuple, group_field ) );
    addElementToTuple( group_element, group_tuple );

    // Create a internal tuple for the list.
    internal_tuple = allocateElement( );
    strcpy( internal_tuple->name, "group");
    internal_tuple->type = RELATION;

    // Create an anonymous relation (list of tuples)
    internal_tuple->u.r = calloc( 1, sizeof( relation_t) );
    cur_output_relation = internal_tuple->u.r;
    cur_output_relation->tuple_list.first = 
        cur_output_relation->tuple_list.last = NULL;

    addElementToTuple( internal_tuple, group_tuple );

    addTupleToRelation( group_tuple, output_relation );

    first = 0;
  }

  // Add the current tuple to the list
  {
    tuple_t* temp_tuple = allocateTuple( );
    assert(temp_tuple);
    copyTuple( temp_tuple, tuple );
    addTupleToRelation( temp_tuple, cur_output_relation );
  }

  return;
}

int executeGroup( relation_t* inp, relation_t* out, char* field )
{
  assert(inp); assert(out); assert(field);

  output_relation = out;
  group_field = field;

  iterateRows( inp, group_iter );

  return 0;
}

