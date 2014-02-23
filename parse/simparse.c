
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "pigtypes.h"

char *current;
char *delimiter;


char* initParser( char* line, char *delim )
{
  current = line;
  delimiter = delim;

  return current;
}

void resetDelimiter( char *delim )
{
  delimiter = delim;
  return;
}


static char* ignoreWhitespace( void )
{
  assert(current);

  while (*current == ' ') current++;

  return current;
}

int delimFound( char* delimiter, char chr )
{
  int i = 0;
  int len = strlen(delimiter);

  while (i < len)
  {
    if (chr == 0) return 1;
    if (chr == '\n') return 1;
    if (delimiter[i] == chr) return 1;
    i++;
  }

  return 0;
}

int parseToken( char* output, int length )
{
  char chr;

  if (*current == 0) return 0;

  ignoreWhitespace();

  *output = 0;

  while (1)
  {
    chr = *current++;
    if (delimFound( delimiter, chr ))
    {
      *output = 0;
      return 1;
    }
    else
    {
      *output++ = chr;
      *output = 0;
      length--;
    }
    assert(length > 0);
  }

  return 0;
}


int consumeToken( char* token )
{
  int len = strlen(token);

  ignoreWhitespace();

  while (*token)
  {
    if (*current != *token) return 0;
    *current++;
    *token++;
  }

  ignoreWhitespace();

  return 1;
}


int endOfStream( void )
{
  ignoreWhitespace();
  return (*current == 0);
}


int parseType( char* type )
{
  int ret;

  if      (!strcmp(type, "CHARARRAY")) return CHARARRAY;
  else if (!strcmp(type, "BYTEARRAY")) return BYTEARRAY;
  else if (!strcmp(type, "LONG")) return LONG;
  else if (!strcmp(type, "DOUBLE")) return DOUBLE;
  else if (!strcmp(type, "TUPLE")) return TUPLE;
  else assert(0);
}


void parseExpression( char* token, char *expr, int *type, char* name )
{
  int i=0;
  char typestring[80];

  // Initialize the returns
  *expr = 0;
  *type = 0; // Invalid Type
  *name = 0;

  // Search for the expression start symbol, '['
  while (*token)
  {
    if (*token++ == '[') break;
  }

  // Skip whitespace
  while (*token == ' ') token++;

  // Accumulate all characters until ']'
  while (*token)
  {
    if (*token == ']') break;
    *expr++ = *token++;
  }
  token++;
  *expr++ = 0;

  // Skip the ':' character
  while ((*token) && (*token != ':')) token++;
  token++;

  // Accumulate the type
  while (*token)
  {
    if (*token == ':') break;
    typestring[i++] = *token++;
  }
  token++;
  typestring[i] = 0;

  *type = parseType( typestring );

  // Accumulate the name
  while (*token)
  {
    if ((*token == ' ') || (*token == '\n')) break;
    *name++ = *token++;
  }
  *name++ = 0;

  return;
}

