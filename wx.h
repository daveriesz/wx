
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
  double lat, lon;
  char *query;
  char *name;
} geoloc;
geoloc *geo_info(const char *arg);
geoloc *geo_create_geoloc(const char *query, const char *name, double lat, double lon);
void geo_delete_geoloc(geoloc *glc);

/*** noaa.c ***/
void noaa_point_info(geoloc *glc, char **fields, char **values_return);
void noaa_forecast(geoloc *glc);
void noaa_conditions(geoloc *glc);

/*** output.c ***/
void wx_print_columns(char **cola, char **colb, char sep, int rows);

#endif /* __WX_H__ */
