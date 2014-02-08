// Pig types

#ifndef __PIGTYPES_H__
#define __PIGTYPES_H__

#include <stdio.h>

#define LONG	   1
#define DOUBLE	   2
#define CHARARRAY  3
#define BYTEARRAY  4
#define TUPLE      5

#define MAX_NAME	60

//struct tuple_s;

// Type of an element (basic type or tuple).
typedef struct element_s {
  struct element_s* next;
  char name[MAX_NAME];
  unsigned int type;
  union {
    double d;
    long l;
    char *s;
    struct tuple_s *t; // tuple
  } u;
} element_t;

// Type of a row of a relation.
typedef struct tuple_s {
  struct tuple_s* next;
  element_t* head_element;
} tuple_t;

// Type of a relation.
typedef struct relation_s {
  char name[MAX_NAME];
  struct tuple_s* row_tuple;
} relation_t;

//
// Helper functions
//
#define max(x,y)	((x > y) ? x : y)

size_t trimwhitespace( char* out, size_t len, const char* str );

//
// Miscellaneous Types and definitions
//

#define MAX_FILENAME	80

//
// Public Functions
//

#define MAX_RELATION_NAME	80

void initRelations( void );
relation_t* allocateRelation( char* name );
relation_t* findRelation( char* name );

tuple_t* allocateTuple( void );
void addTupleToRelation( tuple_t* tuple, relation_t* relation );
void deallocateTuple( tuple_t* tuple );

element_t* allocateElement( void );

int parseLoad( char* line );
int parseDump( char* line );

int executeLoad( char* relation_name, char* filename, char delimiter );


#endif // __PIGTYPES_H__
