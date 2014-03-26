// Pig types

#ifndef __PIGTYPES_H__
#define __PIGTYPES_H__

#include <stdio.h>
#include <string.h>
#include <assert.h>

#define LONG	   1
#define DOUBLE	   2
#define CHARARRAY  3
#define BYTEARRAY  4
#define TUPLE      5
#define BOOLEAN    6
#define NUL        7
#define RELATION   8

#define MAX_NAME	60

#define TRUE       1
#define FALSE      0

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
    struct relation_s *r;
  } u;
} element_t;


// Type of a row of a relation.
typedef struct tuple_s {
  link_t node;
  linked_list_t element_list;
} tuple_t;


// Type of a relation.
typedef struct relation_s {
  linked_list_t tuple_list;
  char name[MAX_NAME];
} relation_t;

#define MAX_LONG_STRING		20
#define MAX_DOUBLE_STRING	40

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
tuple_t* getCurrentTuple( void );
void setCurrentTuple( tuple_t* tuple );
void copyTuple( tuple_t* output, tuple_t* input );
void insertTupleSorted( tuple_t* tuple, relation_t* relation, 
                          char* field, int sort_dir );

void* allocateByteArray( int size );

element_t* allocateElement( void );
void freeElement( element_t *element );

int parseLoad( char* line );
int parseDump( char* line );
int parseForeach( char* line );
int parseDescribe( char* line );
int parseFilter( char* line );
int parseSort( char* line );
int parseGroup( char* line );
int parseStore( char* line );

#define DESCEND    0
#define ASCEND     1

// Parser/Tokenizer
char* initParser( char* line, char* delim );
void resetDelimiter( char* delim );
int parseToken( char* output, int length );
int consumeToken( char* token );
int parseExpression( char* token, char* expr, int* type, char* name );

int executeLoad( char* relation_name, char* filename, char delimiter );
int executeDump( char* relation_name );
int executeDescribe( char* relation_name );
int executeForeach( relation_t* inp, relation_t* out, expr_t* expressions );
int executeFilter( relation_t* inp, relation_t* out, expr_t* expression );
int executeSort( relation_t* inp, relation_t* out, char* field, int dir );
int executeGroup( relation_t* inp, relation_t* out, char* field );
int executeStore( char* relation_name, char* filename, char delimiter );

void printElement( element_t* element );
void printType( int type );
void printByteArray( element_t* element );
element_t* retrieveElement( tuple_t* tuple, char* name );
element_t* copyElement( element_t* element );
void convertElement( element_t* element, int type );
int compareElements( element_t* elem1, element_t* elem2 );

typedef void (*row_iterator_t)( tuple_t* tuple );
typedef void (*element_iterator_t)( element_t* element );

void iterateRows( relation_t* list, row_iterator_t callback );
void iterateElements( tuple_t* list, element_iterator_t callback );

void iterateList( struct link_s* list, void *);

// Interpreter Prototypes
element_t* interpret_go( char* instr );
void interpret_init( void );
void ext_push( element_t* elem_ptr );


#endif // __PIGTYPES_H__
