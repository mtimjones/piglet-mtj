// Tuples Functions

#include "pigtypes.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void addTupleToRelation( tuple_t* tuple, relation_t* relation )
{
  assert(tuple);
  assert(relation);

  relation->row_tuple = tuple;

  return;
}


tuple_t* allocateTuple( void )
{
  return calloc(1, sizeof(tuple_t));
}


#if 0
void deallocateTuple( tuple_t* tuple )
{
  element_t* walker;

  assert(tuple);

  while (walker)
  {
    deallocateElement(
    
#endif


