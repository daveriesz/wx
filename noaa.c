
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dprstrings.h>
#include <dprjson.h>

#include "wx.h"

#define NOAA_FORECAST_URL "https://api.weather.gov/points/%f,%f/forecast"

void noaa_forecast(geoloc *glc)
{
  char url[16384];
  char *data, *name, *fcst;
  dprJson *dj;
  djValue *dv;
  int ii, cc, ll, llt;
  char **nameList, **fcstList;
  
  memset(url, 0, 16384);

  if(glc == NULL)
  {
    fprintf(stderr, "noaa_forecast(): no geodata received\n");
    return;
  }

  sprintf(url, NOAA_FORECAST_URL, glc->lat, glc->lon);
  data = wx_fetch_url(url);
  
//  printf("%s\n", data);

  dj = dj_load_from_data(data);
  dv = dj_get_value(dj, "properties.periods");
  cc = dj_array_length(dv);
  if(cc <= 0)
  {
    fprintf(stderr, "error: empty forecast\n");
    return;
  }

  nameList = (char **)malloc(sizeof(char *) * cc);
  fcstList = (char **)malloc(sizeof(char *) * cc);

  for(ii=0 ; ii<cc ; ii++)
  {
    nameList[ii]  = dj_value_to_string(dj_get_subvalue(dj_array_element(dv, ii), "name"));
    fcstList[ii] = dj_value_to_string(dj_get_subvalue(dj_array_element(dv, ii), "detailedForecast"));
  }

  for(ii=0, ll=0 ; ii<cc ; ii++) { if((llt=strlen(nameList[ii]))>ll) ll = llt; }

  wx_print_columns(nameList, fcstList, ':', cc, ll);

  for(ii=0 ; ii<cc ; ii++)
  {
    free(nameList[ii]);
    free(fcstList[ii]);
  }
  free(nameList);
  free(fcstList);

}
