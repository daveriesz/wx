
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

#include <dprstrings.h>

#include "wx.h"

static geoloc *geo_location = NULL;

static int    opt_argc;
static char **opt_argv;

typedef struct optarg optarg;
typedef void (*optfcn)(optarg *);
struct optarg {
  int         set;
  const char *opt;
  const char *xpt;
        char *xps;
  const char *dsc;
  optfcn      fcn;
};

static void opt_usage                 (optarg *arg);
static void opt_geo_info              (optarg *arg);
static void opt_forecast              (optarg *arg);
static void opt_conditions            (optarg *arg);
static void opt_usemetric             (optarg *arg);
static void opt_useimperial           (optarg *arg);
static void opt_noaa_product_types    (optarg *arg);
static optarg options[]= 
{
  { 0, "-h"  , NULL, NULL, "print usage"                         , opt_usage                  },
  { 0, "-g"  , NULL, NULL, "print geographic information"        , opt_geo_info               },
  { 0, "-f"  , NULL, NULL, "forecast"                            , opt_forecast               },
  { 0, "-c"  , "=" , NULL, "conditions"                          , opt_conditions             },
  { 0, "-mm" , NULL, NULL, "use metric units"                    , opt_usemetric              },
  { 1, "-ii" , NULL, NULL, "use imperial units"                  , opt_useimperial            },
  { 0, "-np" , "=" , NULL, "available NOAA product types"        , opt_noaa_product_types     },
};
static int option_count = (sizeof(options) / sizeof(optarg) );

static char *query_str = NULL;;

static void opt_usage(optarg *arg) { usage(); }
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
//    printf("readopt ii=%d, arg = >>%s<<\n", ii, arg_value(ii));
    set = 0;
    for(jj=0 ; (set==0) && (jj<option_count) ; jj++)
    {
      if(!strcmp(arg_value(ii), options[jj].opt))
      {
        set = options[jj].set = 1;
      }
      else if(
               (!strncmp(arg_value(ii), options[jj].opt, strlen(options[jj].opt))) &&
               (!strncmp(&(arg_value(ii)[strlen(options[jj].opt)]), options[jj].xpt, strlen(options[jj].xpt)))
             )
      {
        set = options[jj].set = 1;
        options[jj].xps = strdup(&(arg_value(ii)[strlen(options[jj].opt)+strlen(options[jj].xpt)]));
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

  for(ii=0 ; ii<option_count ; ii++)
  {
    if(options[ii].set != 0)
    {
      options[ii].fcn(&(options[ii]));
    }
  }
}

static void opt_geo_info(optarg *arg)
{
  if(geo_location == NULL) { geo_location = geo_info(query_str); }
  printf("Geographic Location Information:\n");
  if(geo_location)
  {
    printf("  query     = >>%s<<\n", geo_location->query);
    printf("  name      = >>%s<<\n", geo_location->name );
    printf("  latitude  = >>%f<<\n", geo_location->lat  );
    printf("  longitude = >>%f<<\n", geo_location->lon  );
    printf("  lat-lon   = >>%f,%f<<\n", geo_location->lat, geo_location->lon);
  }
  else
  {
    printf("  nothing received\n");
  }
}

static void opt_forecast(optarg *arg)
{
  if(geo_location == NULL) { geo_location = geo_info(query_str); }
  noaa_forecast(geo_location);
}

static void opt_conditions(optarg *arg)
{
  if(geo_location == NULL) { geo_location = geo_info(query_str); }
  noaa_conditions(geo_location, arg->xps);
}

static enum UnitsType { imperial, metric } units_type = imperial;
static void opt_usemetric(optarg *arg)
{
  units_type = metric;
}

static void opt_useimperial(optarg *arg)
{
  units_type = imperial;
}

int using_metric  () { return (units_type==metric  )?1:0; }
int using_imperial() { return (units_type==imperial)?1:0; }

static void opt_noaa_product_types(optarg *arg)
{
  if(geo_location==NULL) { geo_location = geo_info(query_str) ; }
  if((geo_location==NULL) || ((arg->xps)==NULL)) { noaa_list_products(geo_location); }
  else { noaa_products(geo_location, arg->xps); }
}


