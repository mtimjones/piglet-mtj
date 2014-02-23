
#include <stdio.h>
#include <string.h>
#include "interp.h"

int main( void )
{
  int ret;
  char test[100];

  interp_init();

  strcpy( test, "4 5 * 40 /" );
  ret = interpret_go( test );
  printf("ret = %d (2)\n", ret);

  strcpy( test, "4 5 * 5 3 * ==" );
  ret = interpret_go( test );
  printf("ret = %d (0)\n", ret);

  strcpy( test, "4 5 * 5 3 * >" );
  ret = interpret_go( test );
  printf("ret = %d (0)\n", ret);

  strcpy( test, "4 5 * 5 3 * <" );
  ret = interpret_go( test );
  printf("ret = %d (1)\n", ret);

  strcpy( test, "75 85 -" );
  ret = interpret_go( test );
  printf("ret = %d (10)\n", ret);

  strcpy( test, "   75   85 -   " );
  ret = interpret_go( test );
  printf("ret = %d (10)\n", ret);

  strcpy( test, "75      85 -               " );
  ret = interpret_go( test );
  printf("ret = %d (10)\n", ret);

  return 0;
}
