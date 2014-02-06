// Piglet
// Main program

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pigtypes.h"

#define MAX_FILENAME	80

int verbose = 0;
char script_file[MAX_FILENAME+1]={0};

int execute_script( char* script )
{
  FILE *fp = fopen(script, "r");
  char line[256];
  int  linenum=0;
  int  ret;

  if (strlen(script) == 0) 
  {
    printf("Must specify a script file.\n");
    return -1;
  }

  fp = fopen(script, "r");

  if (fp == NULL)
  {
    printf("Unable to open %s\n", script);
    return -1;
  }

  while (fgets(line, 256, fp) != NULL)
  {
    // Simple parser for a very regular grammar
    if      (strstr(line, "LOAD") != NULL)
    {
      ret = parseLoad( line );
    }
    else if (strstr(line, "DUMP") != NULL)
    {
      ret = parseDump( line );
    }
    else
    {
      printf("Cannot parse line %d\n", linenum);
      ret = -1;
      break;
    }

    linenum++;
  }

  return 0;
}

int main( int argc, char *argv[] )
{
  int opt;

  while ((opt = getopt(argc, argv, "vf:")) != -1)
  {
    switch (opt)
    {
      case 'v':
        verbose = 1;
        break;

      case 'f':
        strncpy( script_file, optarg, MAX_FILENAME );
        break;

    }

  }

  return execute_script( script_file );
}

