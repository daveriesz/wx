
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

#include <dprstrings.h>

#include "wx.h"

static int    opt_argc;
static char **opt_argv;

typedef void (*optfcn)();
typedef struct optarg {
  int         set;
  const char *opt;
  const char *dsc;
  optfcn      fcn;
} optarg;

void opt_usage();
void opt_geo_info();
static optarg options[]= 
{
  { 1, "-h", "print usage",                  opt_usage     },
  { 0, "-g", "print geographic information", opt_geo_info  },
};
static int option_count = (sizeof(options) / sizeof(optarg) );

static char *query_str = NULL;;

void opt_usage() { usage(); }
void usage()
{
  int ii;
  char *argv0 = strdup(arg_value(0));
  char *bnstr = strdup(basename(argv0));

  fprintf(stderr, "usage: %s [options] <location>\n", bnstr);
  fprintf(stderr, "  options:\n");
  for(ii=0 ; ii<option_count ; ii++)
  {
    fprintf(stderr, "    %s: %s\n", options[ii].opt, options[ii].dsc);
  }
//  exit(0);
}

static void copy_args(int __argc, char **__argv)
{
  int ii;

  opt_argc = __argc;
  opt_argv = (char **)malloc(sizeof(char *) * (opt_argc + 1));
  for(ii=0 ; ii<opt_argc ; ii++)
  {
    opt_argv[ii] = strdup(__argv[ii]);
  }
  opt_argv[ii] = NULL;
}

int arg_count() { return opt_argc; }
const char *arg_value(int nn)
{
  if(nn >= arg_count()) { return NULL; }
  else { return opt_argv[nn]; }
}


void readopt(int __argc, char **__argv)
{
  int ii, jj, set;

  copy_args(__argc, __argv);
  if(arg_count() == 1) { usage(); }
  
  for(ii=1 ; ii<arg_count() ; ii++)
  {
    printf("readopt ii=%d, arg = >>%s<<\n", ii, arg_value(ii));
    set = 0;
    for(jj=0 ; (set==0) && (jj<option_count) ; jj++)
    {
      if(!strcmp(arg_value(ii), options[jj].opt))
      {
        set = options[jj].set = 1;
      }
    }

    if(set == 0)
    {
      if(query_str == NULL)
      {
        query_str = strdup(arg_value(ii));
      }
      else
      {
        query_str = strapp(query_str, " ");
        query_str = strapp(query_str, arg_value(ii));
      }
      set = 1;
    }
  }

  opt_usage();
  opt_geo_info();
  return;

  for(ii=0 ; ii<option_count ; ii++)
  {
    if(options[ii].set != 0)
    {
//      options[ii].fcn();
    }
    opt_usage();
    opt_geo_info();
  }
}

void opt_geo_info()
{
  geo_info(query_str);
}
