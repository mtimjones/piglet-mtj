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

// List structure

struct link_s {
  struct link_s* next; // first
  struct link_s* prev; // last
};


// Type of an element (basic type or tuple).
typedef struct element_s {
  struct link_s element_list;
  char name[MAX_NAME];
  unsigned int type;
  unsigned int size; // for bytearray
  union {
    double d;
    long l;
    unsigned char* b; // bytearray
    char *s;
    struct tuple_s *t; // tuple
  } u;
} element_t;


// Type of a row of a relation.
typedef struct tuple_s {
  struct link_s row_list;
  struct link_s element_list;
} tuple_t;


// Type of a relation.
typedef struct relation_s {
  struct link_s row_list;
  char name[MAX_NAME];
} relation_t;


//
// Helper functions
//
#define max(x,y)	((x > y) ? x : y)

int trimwhitespace( char* out, int len, const char* str );

void listAdd( struct link_s* list, struct link_s* item );

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
void addElementToTuple( element_t* element, tuple_t* tuple );
void* allocateByteArray( int size );

element_t* allocateElement( void );

int parseLoad( char* line );
int parseDump( char* line );

int executeLoad( char* relation_name, char* filename, char delimiter );
int executeDump( char* relation_name );


#endif // __PIGTYPES_H__
