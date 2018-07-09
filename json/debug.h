
#ifndef __JSON_DEBUG_H__
#define __JSON_DEBUG_H__

#ifdef DEBUG_PRINT_STATEMENTS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>

#define OUTBUFINFOSZ 16384
#define OUTBUFSZ     131072
#define CPIOSZ       32

static char dbgoutinfo[OUTBUFINFOSZ];
static int debug_init = 0;

static int debugf(const char *format, ...)
{
	static pthread_mutex_t mx;
	static int mutex_initialized = 0;
	static va_list ap;
	static int n, ii;
	static char *cpio[CPIOSZ], *cp;
	static FILE *fp;
	static char output_buffer[OUTBUFSZ];

	if(mutex_initialized == 0)
	{
		mutex_initialized = 1;
		pthread_mutex_init(&mx, NULL);
	}

	pthread_mutex_lock(&mx);

	if(debug_init == 0)
	{
		debug_init = 1;

		memset(dbgoutinfo, 0, OUTBUFINFOSZ);
		cp = getenv(DEBUG_PRINT_ENV);
		if(cp)
		{
			strncpy(dbgoutinfo, cp, OUTBUFINFOSZ);
		}
		memset(cpio, 0, (sizeof(char *)*CPIOSZ));
		for(ii=0 ; ii<CPIOSZ ; ii++)
		{
			if(ii==0)
			{
				cpio[ii] = dbgoutinfo;
			}
			else
			{
				cpio[ii] = strchr(cpio[ii-1], ',');
				if(cpio[ii])
				{
					*(cpio[ii]) = '\0';
					cpio[ii]++;
				}
				else
				{
					break;
				}
			}
		}
	}

	if(dbgoutinfo[0] && cpio[0])
	{
		va_start(ap, format);
		n = vsnprintf(output_buffer, OUTBUFSZ, format, ap);
		va_end(ap);

		for(ii=0 ; ii<CPIOSZ && cpio[ii] ; ii++)
		{
			if(!strcmp(cpio[ii], "stdout"))
			{
				fputs(output_buffer, stdout);
			}
			else if (!strcmp(cpio[ii], "stderr"))
			{
				fputs(output_buffer, stderr);
			}
			else
			{
				fp = fopen(cpio[ii], "a");
				if(fp)
				{ 
					fputs(output_buffer, fp);
					fclose(fp);
				}
			}
		}
	}
	else
	{
		n = 0;
	}
	
	pthread_mutex_unlock(&mx);

	return n;
}
#else
#define debugf(...)
#endif

#endif /* __JSON_DEBUG_H__ */
