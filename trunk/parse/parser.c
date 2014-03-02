// Piglet
// Simple parser (brute-force for the simple syntax)

#include <stdio.h>
#include "pigtypes.h"

int parseLoad( char* line )
{
  char relation[MAX_RELATION_NAME];
  char filename[MAX_FILENAME];
  char delim;
  int ret;
  
  ret = sscanf(line, "%s = LOAD %s DELIM '%c'", relation, filename, &delim);

  // Expecting 3 parsed arguments.
  if (ret == 3)
  {
    ret = executeLoad( relation, filename, delim ); 
  }

  return ret;
}

int parseDump( char* line )
{
  char relation[MAX_RELATION_NAME];
  int  ret;

  ret = sscanf(line, "DUMP %s", relation);

  // Expecting 1 parsed argument.
  if (ret == 1)
  {
    ret = executeDump( relation );
  }

  return ret;
}


#define MAX_TOK		50

int parseForeach( char* line )
{
  // Entire Foreach/Generate must be on one line...
  char token[MAX_TOK+1];
  relation_t* output_relation;
  relation_t* input_relation;
  expr_t expressions[MAX_EXPR];
  int cur_expr = 0;
  int ret;

  // Brute-force parser using a tokenizer
  
  initParser( line, " " );

  // Parse the output relation
  if (parseToken( token, MAX_TOK ))
  {
    // First element will be the relation to create
    output_relation = findRelation( token );

    if (output_relation) 
    {
      printf("Output relation from foreach/generate should not exist.\n");
      return -1;
    }
  } else return -1;

  // Consume the '='
  if (!consumeToken( "=" ))
  {
    printf("Missing = in foreach/generate.\n");
    return -1;
  }

  if (!consumeToken( "FOREACH" ))
  {
    printf("Missing FOREACH in foreach/generate.\n");
    return -1;
  }
  
  if (parseToken( token, MAX_TOK ))
  {
    // First element will be the relation to create
    input_relation = findRelation( token );

    if (!input_relation) 
    {
      printf("Input relation from foreach/generate not found.\n");
      return -1;
    }
  } else return -1;

  if (!consumeToken( "GENERATE" ))
  {
    printf("Missing GENERATE in foreach/generate.\n");
    return -1;
  }

  // loop through and load each expression into a new slot.
  resetDelimiter( "," );

  memset( (void*)expressions, 0, sizeof(expressions) );

  while (parseToken(token, MAX_TOK))
  {
    ret = parseExpression( token, 
                           expressions[cur_expr].expr_str,
                           &expressions[cur_expr].type,
                           expressions[cur_expr].name );

    printf("ret = %d, cur_expr = %d\n", ret, cur_expr);

    expressions[cur_expr].active = ret;

    if (!ret) break;
    else cur_expr++;
  }  

  // Execute the expression list over the input relation
  if (cur_expr > 0)
  {
    ret = executeForeach( input_relation, output_relation, expressions );
  }
  
  return 0;
}

