// Pig types

#ifndef __PIGTYPES_H__
#define __PIGTYPES_H__

#include <stdio.h>
#include <string.h>

#define LONG	   1
#define DOUBLE	   2
#define CHARARRAY  3
#define BYTEARRAY  4
#define TUPLE      5

#define MAX_NAME	60


// List structure (node and list)

typedef struct link_s {
  struct link_s* next;
  struct link_s* prev;
} link_t;

typedef struct list_s {
  link_t* first;
  link_t* last;
} linked_list_t;


// Type of an element (basic type or tuple).
typedef struct element_s {
  link_t node;
  char name[MAX_NAME];
  unsigned int type;
  unsigned int size; // for bytearray
  union {
    double g;
    long l;
    unsigned char* b; // bytearray
    char *s;
    struct tuple_s *t; // tuple
  } u;
} element_t;


// Type of a row of a relation.
typedef struct tuple_s {
  link_t node;
  linked_list_t element_list;
//  linked_list_t group_list; // list for a group.
} tuple_t;


// Type of a relation.
typedef struct relation_s {
  linked_list_t tuple_list;
  char name[MAX_NAME];
} relation_t;


#define MAX_EXPR_LINE   80
#define MAX_EXPR_NAME   80
#define MAX_EXPR	20

typedef struct
{
  int  active;
  char name[MAX_EXPR_NAME+1];
  int  type;
  char expr_str[MAX_EXPR_LINE+1];
} expr_t;


//
// Helper functions
//
#define max(x,y)	((x > y) ? x : y)

int trimwhitespace( char* out, int len, const char* str );

void listAdd( linked_list_t* list, link_t* item );

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
int parseForeach( char* line );

// Parser/Tokenizer
char* initParser( char* line, char* delim );
void resetDelimiter( char* delim );
int parseToken( char* output, int length );
int consumeToken( char* token );
int parseExpression( char* token, char* expr, int* type, char* name );

int executeLoad( char* relation_name, char* filename, char delimiter );
int executeDump( char* relation_name );
int executeForeach( relation_t* input, relation_t* output, expr_t* expressions );

void printElement( element_t* element );
void printByteArray( element_t* element );

typedef void (*row_iterator_t)( tuple_t* tuple );
typedef void (*element_iterator_t)( element_t* element );

void iterateRows( relation_t* list, row_iterator_t callback );
void iterateElements( tuple_t* list, element_iterator_t callback );

void iterateList( struct link_s* list, void *);

#endif // __PIGTYPES_H__
