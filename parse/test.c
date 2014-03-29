
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_TOKEN	80

char sample[200];
char token[MAX_TOKEN];

int main( void )
{
  strcpy( sample, 
  "mydata = FOREACH banks GENERATE [$1]:CHARARRAY:f1 ,  [ $1 $2 + $3 * ]:LONG:f2\n" );

  initParser( sample, " " );

//  while (parseToken( token, MAX_TOKEN))
//  {
//    printf("Token: %s\n", token);
//  }

  // Parse the output relation
  if (parseToken(token, MAX_TOKEN))
  {
    printf("Output relation: %s\n", token);
  } else assert(0);

  // Consume the '='
  if (!consumeToken( "=" )) assert(0);

  // Consume the 'FOREACH'
  if (!consumeToken( "FOREACH" )) assert(0);

  // Parse the input relation
  if (parseToken(token, MAX_TOKEN))
  {
    printf("Input relation: %s\n", token);
  } else assert(0);

  // Consume the 'GENERATE'
  if (!consumeToken( "GENERATE" )) assert(0);

  // Parse the expressions
  resetDelimiter( "," );
  while (parseToken(token, MAX_TOKEN))
  {
    char expr[100];
    int type;
    char name[100];

    parseExpression( token, expr, &type, name );

    printf("Expr: %s\n", expr);
    printf("Type: %d\n", type);
    printf("Name: %s\n\n", name);
  }

  return 0;
}
