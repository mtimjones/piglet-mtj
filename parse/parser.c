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
  
  ret = sscanf( line, "%s = LOAD %s DELIM '%c'", relation, filename, &delim );

  // Expecting 3 parsed arguments.
  if ( ret == 3 )
  {
    ret = executeLoad( relation, filename, delim ); 
  }

  return ret;
}

int parseStore( char* line )
{
  char relation[MAX_RELATION_NAME];
  char filename[MAX_FILENAME];
  char delim;
  int  ret;

  ret = sscanf( line, "STORE %s INTO %s DELIM '%c'", relation, filename, &delim );

  if ( ret == 3 )
  {
    ret = executeStore( relation, filename, delim );
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


int parseDescribe( char* line )
{
  char relation[MAX_RELATION_NAME];
  int  ret;

  ret = sscanf(line, "DESCRIBE %s", relation);

  // Expecting 1 parsed argument.
  if (ret == 1)
  {
    ret = executeDescribe( relation );
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
    else
    {
      output_relation = allocateRelation( token );
      assert( output_relation );
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


int parseFilter( char* line )
{
  // Entire Foreach/Generate must be on one line...
  char token[MAX_TOK+1];
  relation_t* output_relation;
  relation_t* input_relation;
  expr_t expression;
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
      printf("Output relation from filter should not exist.\n");
      return -1;
    }
    else
    {
      output_relation = allocateRelation( token );
      assert( output_relation );
    }
  } else return -1;

  // Consume the '='
  if (!consumeToken( "=" ))
  {
    printf("Missing = in filter.\n");
    return -1;
  }

  if (!consumeToken( "FILTER" ))
  {
    printf("Missing FILTER in FILTER/BY.\n");
    return -1;
  }
  
  if (parseToken( token, MAX_TOK ))
  {
    // First element will be the relation to create
    input_relation = findRelation( token );

    if (!input_relation) 
    {
      printf("Input relation from FILTER/BY not found.\n");
      return -1;
    }
  } else return -1;

  if (!consumeToken( "BY" ))
  {
    printf("Missing BY in FILTER/BY.\n");
    return -1;
  }

  memset( (void*)&expression, 0, sizeof(expression) );

  resetDelimiter( "," );

  if (parseToken(token, MAX_TOK))
  {

    ret = parseExpression( token, 
                           expression.expr_str,
                           &expression.type,
                           expression.name );

    assert( ret == 1 );

  }
  else
  {
    assert(0);
  }
  
  return executeFilter( input_relation, output_relation, &expression );
}


int parseSort( char* line )
{
  char token[MAX_TOK+1];
  char field[MAX_TOK+1];
  relation_t* output_relation;
  relation_t* input_relation;
  int dir = -1;

  // Brute-force parser using a tokenizer
  
  initParser( line, " " );

  // Parse the output relation
  if (parseToken( token, MAX_TOK ))
  {
    // First element will be the relation to create
    output_relation = findRelation( token );

    if (output_relation) 
    {
      printf("Output relation from sort should not exist.\n");
      return -1;
    }
    else
    {
      output_relation = allocateRelation( token );
      assert( output_relation );
    }
  } else return -1;

  // Consume the '='
  if (!consumeToken( "=" ))
  {
    printf("Missing = in sort.\n");
    return -1;
  }

  if (!consumeToken( "SORT" ))
  {
    printf("Missing SORT in SORT/BY.\n");
    return -1;
  }
  
  if (parseToken( token, MAX_TOK ))
  {
    // First element will be the relation to create
    input_relation = findRelation( token );

    if (!input_relation) 
    {
      printf("Input relation from SORT/BY not found.\n");
      return -1;
    }
  } else return -1;

  if (!consumeToken( "BY" ))
  {
    printf("Missing BY in SORT/BY.\n");
    return -1;
  }

  // Parse the sort field
  if (!parseToken( field, MAX_TOK ))
  {
    printf("Missing the sort field.\n");
    return -1;
  }

  // Parse the direction
  if (parseToken( token, MAX_TOK ))
  {
    if      (!strncmp( token, "DESCEND", 6 )) dir = DESCEND;
    else if (!strncmp( token, "ASCEND", 5 )) dir = ASCEND;
    else 
    {
      printf("Sort direction (ASCEND/DESCEND) not defined.\n");
      return -1;
    }
  }

  return executeSort( input_relation, output_relation, field, dir );
}


int parseGroup( char* line )
{
  char token[MAX_TOK+1];
  char field[MAX_TOK+1];
  relation_t* output_relation;
  relation_t* input_relation;

  // Brute-force parser using a tokenizer
  
  initParser( line, " " );

  // Parse the output relation
  if (parseToken( token, MAX_TOK ))
  {
    // First element will be the relation to create
    output_relation = findRelation( token );

    if (output_relation) 
    {
      printf("Output relation from group should not exist.\n");
      return -1;
    }
    else
    {
      output_relation = allocateRelation( token );
      assert( output_relation );
    }
  } else return -1;

  // Consume the '='
  if (!consumeToken( "=" ))
  {
    printf("Missing = in group.\n");
    return -1;
  }

  if (!consumeToken( "GROUP" ))
  {
    printf("Missing GROUP in GROUP/BY.\n");
    return -1;
  }
  
  if (parseToken( token, MAX_TOK ))
  {
    // First element will be the relation to create
    input_relation = findRelation( token );

    if (!input_relation) 
    {
      printf("Input relation from GROUP/BY not found.\n");
      return -1;
    }
  } else return -1;

  if (!consumeToken( "BY" ))
  {
    printf("Missing BY in GROUP/BY.\n");
    return -1;
  }

  // Parse the group field
  if (!parseToken( field, MAX_TOK ))
  {
    printf("Missing the group field.\n");
    return -1;
  }

  return executeGroup( input_relation, output_relation, field );
}

