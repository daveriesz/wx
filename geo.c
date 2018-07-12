
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dprjson.h>
#include <dprstrings.h>

#include "wx.h"

geoloc *geo_info_esri(const char *query_str);

geoloc *geo_info(const char *query_str)
{
  char *loc;
  
  if(query_str == NULL)
  {
    fprintf(stderr, "Geographic Information Query received NULL location\n");
    return NULL;
  }
  
  loc = strdup(query_str);
  
//  fprintf(stderr, "geo_info(%s)\n", loc);
  free(loc);
  
  return geo_info_esri(query_str);
}

#define ESRI_CANDIDATE_URL "http://geocode.arcgis.com/arcgis/rest/services/World/GeocodeServer/findAddressCandidates?singleLine=%s&category=%s&outFields=*&langCode=EN&forStorage=false&f=pjson"

static char *esri_categories[] =
{
  "Airport",
  "Heliport",
  "City",
  "Primary Postal",
  "College",
  "Region",
  "Subregion",
  "Island",
  "Military+Base",
  "Landmark",
  "Amusement Park",
  "Historical Monument",
};

geoloc *geo_info_esri(const char *query_str)
{
  char url[16384];
  char *data;
  dprJson *dj;
  djValue *candidates, *val, *choice=NULL;
  char *categories = NULL;
  int ii, jj;
  char *cp;
  int cdcount;
  geoloc *glc=NULL;
  char *type, *country, *placename;
  
  for(ii=0 ; ii<(sizeof(esri_categories)/sizeof(char *)) ; ii++)
  {
//    printf("category %d: %s\n", ii, esri_categories[ii]);
    if(categories != NULL) { categories = strapp(categories, ","); }
    categories = strapp(categories, esri_categories[ii]);
  }
  
  bzero(url, 16384);
  sprintf(url, ESRI_CANDIDATE_URL, query_str, categories);
  replace_chr(url, ' ', '+');
  data = wx_fetch_url(url);

//  printf("data = %s\n", data);

  dj = dj_load_from_data(data);

  candidates = dj_get_value(dj, "candidates");
  cdcount = dj_array_length(candidates);
//  printf("candidates = %p\n", candidates);
  printf("candidates length = %d\n", cdcount);

  for(ii=0 ; (choice==NULL) && (ii<(sizeof(esri_categories)/sizeof(char *))) ; ii++)
  {
    for(jj=0 ; (choice==NULL) && (jj<cdcount) ; jj++)
//    for(val=candidates ; (choice==NULL) && val ; val=dj_value_next(val))
    {
      val = dj_array_element(candidates, jj);
      type = dj_value_to_string(dj_get_subvalue(val, "attributes.Type"));
      if(!strcmp(type, esri_categories[ii]))
      {
//        printf("  val = >>%s<<\n", dj_value_to_string(dj_get_subvalue(val, "attributes.LongLabel")));
        choice = val;
      }
      free(type);
    }
  }
  
  for(ii=0 ; (choice==NULL) && (ii<cdcount) ; ii++)
  {
    val = dj_array_element(candidates, ii);
    country = dj_value_to_string(dj_get_subvalue(val, "attributes.Country"));
    placename = dj_value_to_string(dj_get_subvalue(val, "attributes.PlaceName"));
    printf("val: %s, %s\n", country, placename);
    if((!strcmp(country, "USA")) && (!strcasecmp(placename, query_str))) { choice = val; }
    free(country);
    free(placename);
  }

  if(choice == NULL) { choice = dj_array_element(candidates, 0); }

  if(choice!=NULL)
  {
    glc = geo_create_geoloc(
      query_str,
      dj_value_to_string(dj_get_subvalue(choice, "attributes.LongLabel")),
      dj_value_to_number(dj_get_subvalue(choice, "location.y")),
      dj_value_to_number(dj_get_subvalue(choice, "location.x")));
  }

  dj_delete(dj);
  free(data);

  return glc;
}

geoloc *geo_create_geoloc(const char *query, const char *name, double lat, double lon)
{
  geoloc *glc = (geoloc *)malloc(sizeof(geoloc));

  if(glc)
  {
    memset(glc, 0, sizeof(geoloc));
    glc->query = strdup(query);
    glc->name = strdup(name);
    glc->lat = lat;
    glc->lon = lon;
  }

  return glc;
}

void geo_delete_geoloc(geoloc *glc)
{
  if(!glc) { return; }
  if(glc->query) { free(glc->query); }
  if(glc->name ) { free(glc->name ); }
  free(glc);
}

