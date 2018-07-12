
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

#include "wx.h"

static int wx_terminal_width()
{
	static int width = 0;
	static char *wenv;
	struct winsize wsz;

	if(width > 0) return width;
	
	if(wenv=getenv("WX_SCREEN_WIDTH"))
		if((width=atoi(wenv))>0)
			return width;

	if(ioctl(STDIN_FILENO, TIOCGWINSZ, (char *)&wsz) < 0)
	{
		fprintf(stderr, "wx_terminal_width: ioctl() failed: %s\n", strerror(errno));
		width = 80;
	}
	else
	{
		width = wsz.ws_col;
	}
	
	return width;
}

void wx_print_columns(char **cola, char **colb, char sep, int rows)
{
  char fmtstr[1024];
  int maxr, ii, jj, ll, cblen, wida=0;
  char *cb, *cp1, *cp2;
  char *buf;
  
  for(ii=0 ; ii<rows ; ii++)
  {
    if((ll=strlen(cola[ii]))>wida) { wida = ll; }
  }

  sprintf(fmtstr, "%%-%ds %%c %%s\n", wida);
  maxr = (wx_terminal_width() - (wida + 3));
  buf = (char *)malloc(maxr+1);

  for(ii=0 ; ii<rows ; ii++)
  {
    cp1 = cb = strdup(colb[ii]);
    cblen = strlen(cb);
    if(strlen(cp1)<maxr)
    {
      printf(fmtstr, cola[ii], ':', cb);
    }
    else
    {
      while(strlen(cp1)>maxr)
      {
        memset(buf, 0, maxr+1); 
        cp2 = &(cp1[maxr-1]);
        while(*cp2 != ' ') { *cp2--; }
        if(cp2 != cp1) { *cp2 = '\0' ; cp2++; strcpy(buf, cp1); }
        else           { strncpy(buf, cp1, cp2-cp1); }
        printf(fmtstr, (cp1==cb)?cola[ii]:" ", (cp1==cb)?':':' ', buf);
        cp1 = cp2;
      }
      if(cp1-cb < cblen)
      {
        printf(fmtstr, " ", ' ', cp1);
      }
    }
    free(cb);
  }
  free(buf);
}

