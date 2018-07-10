
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dprstrings.h"

char *strapp(char *dest, const char *src)
{
  char *newstr = dest;
  int len=0;

  if(src)
  {
    len = (dest?strlen(dest):0) + strlen(src) + 1;
    newstr = realloc(dest, len);
    if(!dest) { strcpy(newstr, src); }
    else      { strcat(newstr, src); }
  }

  return newstr;
}

char *strrew(char *dest, const char *src)
{
  if(dest) { free(dest); }
  return strapp(NULL, src);
}

unsigned char *dataapp(unsigned char *dest, size_t oldsz, const unsigned char *src, size_t srcsz)
{
  dest = (unsigned char *)realloc((void *)dest, oldsz+srcsz);
  memcpy((void *)&(dest[oldsz]), (void *)src, srcsz);
  return dest;
}
