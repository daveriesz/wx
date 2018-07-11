
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

void wx_print_columns(char **cola, char **colb, char sep, int rows, int wida)
{
  char fmtstr[1024];
  int maxr, ii, jj;
  char *cb, *cp1, *cp2;
  
  sprintf(fmtstr, "%%-%ds %c %%s\\n", wida, sep);
  maxr = (wx_terminal_width() - (wida + 3));

  for(ii=0 ; ii<rows ; ii++)
  {
    cb = strdup(colb[ii]);
    for(cp1=cb ; strlen(cp1)<maxr ; cp1=cp2)
    {
      for(cp2=&(cp1[maxr-2]) ; *cp2 != ' ' ; cp2--);
//      if(cp2
    
    }
    
    
    free(cb);
  }
  
}

