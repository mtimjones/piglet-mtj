// Pig types

#ifndef __PIGTYPES_H__
#define __PIGTYPES_H__

#include <stdio.h>

#define LONG	1
#define DOUBLE	2
#define STRING  3
#define TUPLE   4

#define MAX_NAME	60

struct tuple_s;

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
typedef struct row_s {
  struct row_s* next;
  element_t* head_element;
} tuple_t;

// Type of a relation.
typedef struct relation_s {
  char name[MAX_NAME];
  struct tuple_s* row_tuple;
} relation_t;

//
// Miscellaneous Types and definitions
//

#define MAX_FILENAME	80

#endif // __PIGTYPES_H__
