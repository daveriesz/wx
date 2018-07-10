
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dprjson.h>

#include "wx.h"

void geo_info_esri(const char *query_str);

void geo_info(const char *query_str)
{
  char *loc;
  
  if(query_str == NULL)
  {
    fprintf(stderr, "Geographic Information Query received NULL location\n");
    return;
  }
  
  loc = strdup(query_str);
  
  fprintf(stderr, "geo_info(%s)\n", loc);
  
  geo_info_esri(query_str);
}

#define ESRI_CANDIDATE_URL "http://geocode.arcgis.com/arcgis/rest/services/World/GeocodeServer/findAddressCandidates?singleLine=%s&category=%s&outFields=*&langCode=EN&forStorage=false&f=pjson"

void geo_info_esri(const char *query_str)
{
  char url[16384];
  char *data;
  dprJson *dj;
  
  bzero(url, 16384);
  sprintf(url, ESRI_CANDIDATE_URL, query_str, "Airport,Heliport");
  data = wx_fetch_url(url);

//  printf("data = %s\n", data);

  dj = dj_load_from_data(data);

  
  free(data);
}
