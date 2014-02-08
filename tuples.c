// Tuples Functions

#include "pigtypes.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void addTupleToRelation( tuple_t* tuple, relation_t* relation )
{
  assert(tuple);
  assert(relation);

  listAdd( (struct link_s*)relation, (struct link_s*)tuple );

  return;
}


void addElementToTuple( element_t* element, tuple_t* tuple )
{
  assert(element);
  assert(tuple);

  listAdd( (struct link_s*)&tuple->element_list, (struct link_s*)element );

  return;
}


tuple_t* allocateTuple( void )
{
  tuple_t* tuple = calloc(1, sizeof(tuple_t));

  assert(tuple);

  tuple->row_list.next = tuple->row_list.prev = NULL;
  tuple->element_list.next = tuple->element_list.prev = NULL;

  return tuple;
}


