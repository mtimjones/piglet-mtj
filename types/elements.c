// Elements Functions

#include "pigtypes.h"
#include <string.h>
#include <stdlib.h>


element_t* allocateElement( void )
{
  return calloc(1, sizeof(element_t));
}


void freeElement( element_t *element )
{
  free( element );
  return;
}


static void printInnerRelation( relation_t* relation )
{
  link_t* olink;

  extern void elem_iter( element_t* element );

  assert(relation);

  printf("\n    {\n");
  for ( olink = relation->tuple_list.first ; olink ; olink = olink->next )
  {

    printf("      {");
    iterateElements( (tuple_t*)olink, elem_iter );
    if (olink->next) printf("},\n");
    else printf("}\n");

  }
  printf("    }\n");

  return;
}

void printElement( element_t* element )
{
  assert(element);

  switch( element->type )
  {
    case LONG:
      printf("%ld", element->u.l);
      break;

    case DOUBLE:
      printf("%g", element->u.g);
      break;

    case BYTEARRAY:
    case CHARARRAY:
      printByteArray( element );
      break;

    case NUL:
      printf("NUL");
      break;

    case TUPLE:
      // emit a bag.
      printf("Tuple");
      break;

    case BOOLEAN:
      if (element->u.l) printf("TRUE");
      else printf("FALSE");
      break;

    case RELATION:
      printInnerRelation( element->u.r );
      break;
    
    default:
      assert(0);

  }

  return;
}


void printType( int type )
{
  switch( type )
  {
    case LONG:
      printf("LONG");
      break;
    case DOUBLE:
      printf("DOUBLE");
      break;
    case BYTEARRAY:
      printf("BYTEARRAY");
      break;
    case CHARARRAY:
      printf("CHARARRAY");
      break;
    case BOOLEAN:
      printf("BOOLEAN");
      break;
    case TUPLE:
      printf("TUPLE");
      break;
    case RELATION:
      printf("RELATION");
      break;
    default:
      assert(0);
  }
  return;
}


element_t* copyElement( element_t* source )
{
  element_t* target = allocateElement( );

  if (!source)
  {
    target->type = NUL;
    return target;
  }

  strcpy( target->name, source->name );
  target->type = source->type;
  target->size = source->size;

  switch( source->type )
  {
    case LONG:
      target->u.l = source->u.l;
      break;
    case DOUBLE:
      target->u.g = source->u.g;
      break;
    case BYTEARRAY:
      {
        int i;
        target->u.b = allocateByteArray( target->size+1 );
        for ( i = 0 ; i < source->size ; i++ )
        {
          target->u.b[i] = source->u.b[i];
        }
        target->u.b[i] = 0;
      }
      break;
    case CHARARRAY:
      {
        int i;
        target->u.s = malloc( target->size+1 );
        for ( i = 0 ; i < source->size ; i++ )
        {
          target->u.s[i] = source->u.b[i];
        }
        target->u.s[i] = 0;
      }
      break;
    case BOOLEAN:
      target->u.l = source->u.l;
      break;
    case TUPLE:
      assert(0);
    case NUL:
      break;
    case RELATION:
      target->u.r = source->u.r;
      break;
    default:
      assert(0);
  }

  return target;
}


void convertElement( element_t* element, int type )
{
  long larg;
  double darg;

  if ( element->type == type ) return;

  if        ( element->type == BYTEARRAY )
  {
    switch( type )
    {
      case CHARARRAY:
        element->type = CHARARRAY;
        break;
      case LONG:
        larg = atoi( (char *)element->u.b );
        element->u.l = larg;
        element->type = LONG;
        break;
      case DOUBLE:
        sscanf( (char *)element->u.b, "%lg", &darg );
        element->u.g = darg;
        element->type = DOUBLE;
        break;
      case BOOLEAN:
        assert(0);
        break;
    }
  } else if ( element->type == CHARARRAY )
  {
    switch( type )
    {
      case BYTEARRAY:
        element->type = BYTEARRAY;
        break;
      case LONG:
        larg = atoi( (char *)element->u.s );
        element->u.l = larg;
        element->type = LONG;
        break;
      case DOUBLE:
        sscanf( (char *)element->u.s, "%lg", &darg );
        element->u.g = darg;
        element->type = DOUBLE;
        break;
      case BOOLEAN:
        assert(0);
        break;
    }
  } else if ( element->type == LONG )
  {
    switch( type )
    {
      case CHARARRAY:
      case BYTEARRAY:
        larg = element->u.l;
        element->u.s = malloc( MAX_LONG_STRING+1 );
        snprintf( element->u.s, MAX_LONG_STRING, "%ld", larg );
        element->type = type;
        break;
      case DOUBLE:
        element->u.g = (double)element->u.l;
        element->type = DOUBLE;
        break;
      case BOOLEAN:
        assert(0);
        break;
    }
  } else if ( element->type == DOUBLE )
  {
    switch( type )
    {
      case CHARARRAY:
      case BYTEARRAY:
        darg = element->u.g;
        element->u.s = malloc( MAX_DOUBLE_STRING+1 );
        snprintf( element->u.s, MAX_DOUBLE_STRING, "%lg", darg );
        element->type = type;
        break;
      case LONG:
        element->u.l = (long)element->u.g;
        element->type = LONG;
        break;
      case BOOLEAN:
        assert(0);
        break;
    }

  } else if ( element->type == BOOLEAN )
  {
    assert(0);
  }

  return;
}


