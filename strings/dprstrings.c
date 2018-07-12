
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

void replace_chr(char *str, char chf, char chr)
{
  char *cp;
  while(cp = strchr(str, chf))
  {
    *cp = chr;
  }
}

char **strarrayapp(char **dest, const char *src)
{
  char **cpp;
  int ii;
  
  if(dest == NULL)
  {
    cpp = (char **)malloc(sizeof(char *) * 2);
    cpp[0] = strdup(src);
    cpp[1] = NULL;
  }
  else
  {
    for(ii=0 ; dest[ii]!=NULL ; ii++);
//    printf("strarrayapp: newsize = %ld\n", sizeof(char *)*(ii+2));
    cpp = (char **)realloc(dest, sizeof(char *) * (ii+2));
    cpp[ii] = strdup(src);
    cpp[ii+1] = NULL;
  }
  return cpp;
}

void free_null_terminated_array(char ***arr)
{
  char **cpp = *arr;
  int ii;
  if(cpp == NULL) { return; }
  for(ii=0 ; cpp[ii] != NULL ; ii++)
  {
//    printf("freeing: %s\n", cpp[ii]);
    free(cpp[ii]);
  }
  free(cpp);
  *arr = NULL;
}

