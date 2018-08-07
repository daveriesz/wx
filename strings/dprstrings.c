
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

char **new_list(const char *src, int delim)
{
  char **list = NULL;
  const char *cp, *cpd;
  int len;
  
//  printf("new_list: src=>>%s<<, delim=>>%c<<\n", src, delim);
  
  if((src==NULL) || (delim=='\0')) { return strarrayapp(NULL, src); }
  
  for(cp=src ; cp && *cp ; cp=strchr(cp, delim))
  {
    if(*cp == delim) { cp++; }
    if(cpd = strchr(cp, delim)) { len = cpd - cp; }
    else                        { len = strlen(cp); }
    list = strarrayapp(list, strndup(cp, len));
  }

//  printf("new list finished\n");
  return list;
}

char *replace_str(char *str, char *sf, char *st)
{
  char **list = NULL;
  char *cp, *cpd;
  int lf=strlen(sf), lt=strlen(st);
  int len;
  int ii;
  char *newstring = NULL;

  if((str==NULL) || (sf==NULL) || (st==NULL)) { return str; }
  
  for(cp=str ; cp && *cp ; cp=strstr(cp, sf))
  {
    if(!strncmp(cp, sf, lf)) { cp+=lf; }
    if(cpd = strstr(cp, sf)) { len = cpd - cp; }
    else                     { len = strlen(cp); }
    list = strarrayapp(list, strndup(cp, len));
  }

  if((list==NULL) || (list[0]==NULL)) { return str; }

  for(ii=0 ; list[ii] != NULL ; ii++)
  {
    if(ii>0) { newstring = strapp(newstring, st); }
    newstring = strapp(newstring, list[ii]);
  }
  free(str);
  free_null_terminated_array(&list);

  return newstring;
}
