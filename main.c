// Piglet
// Main program

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_FILENAME	80

int verbose = 0;
char filename[MAX_FILENAME+1];

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
        strncpy( filename, optarg, MAX_FILENAME );
printf("input file is %s\n", filename);
        break;

    }

  }

  return 0;
}
