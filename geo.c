
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dprjson.h>
#include <dprstrings.h>

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

static char *esri_categories[] =
{
  "Airport,Heliport",
  "City",
  "Primary+Postal",
  "College",
  "Region",
  "Subregion",
  "Island",
  "Military+Base",
  "Landmark",
  "Amusement+Park",
  "Historical+Monument",
};

void geo_info_esri(const char *query_str)
{
  char url[16384];
  char *data;
  dprJson *dj;
  djValue *candidates;
  char *categories = NULL;
  int ii;
  
  for(ii=0 ; ii<(sizeof(esri_categories)/sizeof(char *)) ; ii++)
  {
    printf("category %d: %s\n", ii, esri_categories[ii]);
    if(categories != NULL) { categories = strapp(categories, ","); }
    categories = strapp(categories, esri_categories[ii]);
  }
  
  bzero(url, 16384);
  sprintf(url, ESRI_CANDIDATE_URL, query_str, categories);
  data = wx_fetch_url(url);

  printf("data = %s\n", data);

  dj = dj_load_from_data(data);

  candidates = dj_get_value(dj, "candidates");
  printf("candidates = %p\n", candidates);
  printf("candidates length = %d\n", dj_array_length(candidates));
  
  free(data);
}
