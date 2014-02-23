// expr.h

#ifndef __EXPR_H__
#define __EXPR_H__

#include "pigtypes.h"

#define MAX_EXPR_LINE	80
#define MAX_EXPR	20

typedef char expr_t[MAX_EXPR_LINE];

typedef struct 
{
  int  num_expr;
  expr_t expressions[MAX_EXPR];
} expression_t;

#endif // __EXPR_H__
