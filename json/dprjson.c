
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

