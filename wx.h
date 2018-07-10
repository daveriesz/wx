
#ifndef __WX_H__
#define __WX_H__

/*** main.c ***/

/*** opt.c ***/
void readopt(int argc, char **argv);
void usage();
int arg_count();
const char *arg_value(int nn);

/*** wxcurl.c ***/
char *wx_fetch_url(const char *url);

/*** geo.c ***/
typedef struct geoloc {
  double lon, lat;
  char *query;
  char *name;
} geoloc;
void geo_info(const char *arg);

/*** noaa.c ***/

#endif /* __WX_H__ */
