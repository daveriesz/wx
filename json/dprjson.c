
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "dprjson.h"
#include "json.tab.h"
#include "jsondata.h"

extern FILE *yyin, *yyout;

dprJson *dj_load_from_file(const char *filename)
{
	char *str = NULL;
	char buf[1024];
	int fd, tsz, csz;
	dprJson *dj;

	if(0)
	{
		FILE *fp;
		if(!(fp = fopen(filename, "r"))) { return NULL; }
		yyin = fp;
		yyparse();
		return NULL;	
	}
	
	if((fd = open(filename, O_RDONLY)) < 0)
	{
		fprintf(stderr, "dj_load_from_file() error:\n  could not open file >>%s<<\n  %s\n", filename, strerror(errno));
		return NULL;
	}

	for(tsz=csz=0 ; (csz = read(fd, buf, 1024)) > 0 ; tsz += csz)
	{
		str = (char *)realloc(str, tsz+csz+1);
		memcpy(&(str[tsz]), buf, csz);
		str[tsz+csz] = '\0';
	}
	close(fd);
	
//	printf("bytes read: %d\n", tsz);
//	printf("strlen = %ld\n", strlen(str));
	
	dj = dj_load_from_data(str);
	free(str);
	return dj;
}

dprJson *dj_load_from_data(char *chardata)
{
	FILE *fp;
	dprJson *dj;

	fp = fmemopen(chardata, strlen(chardata), "r");
	yyin = fp;
	yyparse();
	fclose(fp);
	dj = dj_get_data();

	return dj;
}

int dj_array_length(djValue *djv)
{
  djValue *vp;
  int ii;

  if(dj_value_type(djv) != dj_vt_array) { return -1; }

  for(ii=0, vp=djv->varray->values ; vp ; ii++, vp=vp->next);
  
  return ii;
}

djVType dj_value_type(djValue *djv) { return djv->vtype; }

djValue *dj_array_element(djValue *djv, int nn)
{
  djValue *vp;
  int ii;

  if(dj_value_type(djv) != dj_vt_array) { return NULL; }

  for(ii=0, vp=djv->varray->values ; vp ; ii++, vp=vp->next)
  {
    if(ii == nn) { return vp; }
  }
  return NULL;
}

djValue *dj_value_next(djValue *djv)
{
  if(djv == NULL) { return NULL; }
  return djv->next;
}

double dj_value_to_number(djValue *dv)
{
  if(dv->vtype != dj_vt_number) { return 0; }
  return atof(dv->vnumber);
}
