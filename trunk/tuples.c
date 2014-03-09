// Tuples Functions

#include "pigtypes.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

static tuple_t* curTuple;

tuple_t* getCurrentTuple( void )
{
  return curTuple;
}

void setCurrentTuple( tuple_t* tuple )
{
  curTuple = tuple;
  return;
}

void addTupleToRelation( tuple_t* tuple, relation_t* relation )
{
  assert(tuple);
  assert(relation);

  listAdd( (linked_list_t*)relation, (link_t*)tuple );

  return;
}


void addElementToTuple( element_t* element, tuple_t* tuple )
{
  assert(element);
  assert(tuple);

  listAdd( (linked_list_t*)&tuple->element_list, (link_t*)element );

  return;
}


tuple_t* allocateTuple( void )
{
  tuple_t* tuple = calloc(1, sizeof(tuple_t));

  assert(tuple);

  tuple->node.next = tuple->node.prev = NULL;
  tuple->element_list.first = tuple->element_list.last = NULL;

  return tuple;
}


void iterateRows( relation_t* list, row_iterator_t callback )
{
  link_t* link;

  assert(list);

  for (link = list->tuple_list.first ; link ; link = link->next) {
    callback( (tuple_t*)link );
  }

  return;
}

void iterateElements( tuple_t* list, element_iterator_t callback )
{
  link_t* link;

  assert(list);

  for (link = list->element_list.first ; link ; link = link->next) {
    callback( (element_t*)link );
  }

  return;
}


void copyTuple( tuple_t* output, tuple_t* input )
{
  link_t* link;
  element_t* element;

  assert( output );
  assert( input );

  for (link = input->element_list.first ; link ; link = link->next) {
    element = copyElement( (element_t*)link );
    addElementToTuple( element, output );
  }

  return;
}

