
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dprstrings.h>
#include <dprjson.h>

#include "wx.h"

#define NOAA_FORECAST_URL     "https://api.weather.gov/points/%f,%f/forecast"
#define NOAA_POINT_URL        "https://api.weather.gov/points/%f,%f"
#define NOAA_STATIONS_URL     "https://api.weather.gov/points/%f,%f/stations"
#define NOAA_OBSERVATIONS_URL "https://api.weather.gov/stations/%s/observations/current"

void noaa_point_info(geoloc *glc, char **fields, char **values_return)
{
  char url[16384];
  char *data;
  static dprJson *dj = NULL;
  int ii;

  memset(url, 0, 16384);

  if(glc == NULL)
  {
    fprintf(stderr, "noaa_point_info(): no geodata received\n");
    return;
  }

  if(dj == NULL)
  {
    sprintf(url, NOAA_POINT_URL, glc->lat, glc->lon);
    data = wx_fetch_url(url);
    dj = dj_load_from_data(data);
    free(data);
  }

  for(ii=0 ; fields[ii] != NULL ; ii++)
  {
    values_return[ii] = dj_value_to_string(dj_get_value(dj, fields[ii]));
  }
}

void noaa_forecast(geoloc *glc)
{
  char url[16384];
  char *data;
  dprJson *dj;
  djValue *dv;
  int ii, cc, ll, llt;
  char **nameList, **fcstList;
  char *point_info_fields[] = {
    "properties.cwa",
    "properties.relativeLocation.properties.city",
    "properties.relativeLocation.properties.state",
    NULL };
  char *point_values[3];

  memset(url, 0, 16384);

  if(glc == NULL)
  {
    fprintf(stderr, "noaa_forecast(): no geodata received\n");
    return;
  }

  noaa_point_info(glc, point_info_fields, point_values);
  printf("*** Forecast for %s, %s (%s) ***\n", point_values[1], point_values[2], point_values[0]);

  sprintf(url, NOAA_FORECAST_URL, glc->lat, glc->lon);
  data = wx_fetch_url(url);
  
//  printf("%s\n", data);

  dj = dj_load_from_data(data);
  free(data);
  dv = dj_get_value(dj, "properties.periods");
  cc = dj_array_length(dv);
  if(cc <= 0)
  {
    fprintf(stderr, "error: empty forecast\n");
    dj_delete(dj);
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

  wx_print_columns(nameList, fcstList, ':', cc);

  for(ii=0 ; ii<cc ; ii++)
  {
    free(nameList[ii]);
    free(fcstList[ii]);
  }
  free(nameList);
  free(fcstList);
  dj_delete(dj);

}

void append_measurement(char ***vma, char ***uma, dprJson *dj, char *field)
{
  char *vv, *uu, *vf=NULL, *uf=NULL;
  vf = strapp(vf, field); vf = strapp(vf, ".value");
  uf = strapp(uf, field); uf = strapp(uf, ".unitCode");
  vv = dj_value_to_string(dj_get_value(dj, vf)); // printf("vf=%s, vv=%s\n", vf, vv);
  uu = dj_value_to_string(dj_get_value(dj, uf)); // printf("uf=%s, uv=%s\n", uf, uu);
  convert_units(&vv, &uu);
  *vma = strarrayapp(*vma, vv);
  *uma = strarrayapp(*uma, uu);
  free(vf);
  free(uf);
}

char *measurement_string(dprJson *dj, char *field)
{
  char *vv, *uu, *vf=NULL, *uf=NULL;
  char *str = NULL;
  
  vf = strapp(vf, field); vf = strapp(vf, ".value");
  uf = strapp(uf, field); uf = strapp(uf, ".unitCode");
  vv = dj_value_to_string(dj_get_value(dj, vf));
  uu = dj_value_to_string(dj_get_value(dj, uf));
  convert_units(&vv, &uu);
  str = strapp(str, vv);
  str = strapp(str, " ");
  str = strapp(str, uu);

  free(vf);
  free(uf);
  free(vv);
  free(uu);

  return str;
}

void noaa_conditions(geoloc *glc)
{
  char url[16384];
  char *data, *iden, *name;
  dprJson *dj;
  djValue *dv;
  char *point_info_fields[] = {
    "properties.cwa",
    "properties.relativeLocation.properties.city",
    "properties.relativeLocation.properties.state",
    NULL };
  char *point_values[3];
  int ii;

  noaa_point_info(glc, point_info_fields, point_values);
  printf("*** Conditions at %s, %s (%s) ***\n", point_values[1], point_values[2], point_values[0]);

  sprintf(url, NOAA_STATIONS_URL, glc->lat, glc->lon);
  data = wx_fetch_url(url);
  dj = dj_load_from_data(data);
//  printf("data = %s\n", data);
  dv = dj_array_element(dj_get_value(dj, "features"), 0);
  free(data);
  iden = dj_value_to_string(dj_get_subvalue(dv, "properties.stationIdentifier"));
  name = dj_value_to_string(dj_get_subvalue(dv, "properties.name"));
  dj_delete(dj);

  sprintf(url, NOAA_OBSERVATIONS_URL, iden);
  data = wx_fetch_url(url);
//  printf("data = %s\n", data);
  dj = dj_load_from_data(data);
  
  {
    char **measurements = NULL;
    char **measuredescs = NULL;
    char **cloudLayerLevel = NULL;
    char **cloudLayerAmnts = NULL;
    char **cloudLayerBaseValue = NULL;
    char **cloudLayerBaseUnits = NULL;
    char **cloudLayerBaseAmnts = NULL;
    char  *wind = NULL;
    char  *clouds = NULL;
    int cloudElements = dj_array_length(dj_get_value(dj, "properties.cloudLayers"));

    wind = strapp(wind, measurement_string(dj, "properties.windDirection"));
    wind = strapp(wind, " @ ");
    wind = strapp(wind, measurement_string(dj, "properties.windSpeed"));
    wind = strapp(wind, " / gust: ");
    wind = strapp(wind, measurement_string(dj, "properties.windGust"));

    measurements = strarrayapp(measurements, dj_value_to_string(dj_get_value(dj, "properties.timestamp"      )));
    measurements = strarrayapp(measurements, dj_value_to_string(dj_get_value(dj, "properties.rawMessage"     )));
    measurements = strarrayapp(measurements, dj_value_to_string(dj_get_value(dj, "properties.textDescription")));
    measurements = strarrayapp(measurements, measurement_string(dj, "properties.temperature"              ));
    measurements = strarrayapp(measurements, measurement_string(dj, "properties.dewpoint"                 ));
    measurements = strarrayapp(measurements, wind);
    measurements = strarrayapp(measurements, measurement_string(dj, "properties.barometricPressure"       ));
//    measurements = strarrayapp(measurements, measurement_string(dj, "properties.seaLevelPressure"         ));
    measurements = strarrayapp(measurements, measurement_string(dj, "properties.visibility"               ));
    measurements = strarrayapp(measurements, measurement_string(dj, "properties.maxTemperatureLast24Hours"));
    measurements = strarrayapp(measurements, measurement_string(dj, "properties.minTemperatureLast24Hours"));
    measurements = strarrayapp(measurements, measurement_string(dj, "properties.precipitationLastHour"    ));
    measurements = strarrayapp(measurements, measurement_string(dj, "properties.precipitationLast3Hours"  ));
    measurements = strarrayapp(measurements, measurement_string(dj, "properties.precipitationLast6Hours"  ));
    measurements = strarrayapp(measurements, measurement_string(dj, "properties.relativeHumidity"         ));
    measurements = strarrayapp(measurements, measurement_string(dj, "properties.windChill"                ));
    measurements = strarrayapp(measurements, measurement_string(dj, "properties.heatIndex"                ));

    measuredescs = strarrayapp(measuredescs, "Time"            );
    measuredescs = strarrayapp(measuredescs, "METAR"           );
    measuredescs = strarrayapp(measuredescs, "Observation"     );
    measuredescs = strarrayapp(measuredescs, "Temperature"     );
    measuredescs = strarrayapp(measuredescs, "Dewpoint"        );
    measuredescs = strarrayapp(measuredescs, "Wind"            );
    measuredescs = strarrayapp(measuredescs, "Visibility"      );
    measuredescs = strarrayapp(measuredescs, "Pressure"        );
    measuredescs = strarrayapp(measuredescs, "24 hour max temp");
    measuredescs = strarrayapp(measuredescs, "24 hour min temp");
    measuredescs = strarrayapp(measuredescs, "1 hour precip"   );
    measuredescs = strarrayapp(measuredescs, "3 hour precip"   );
    measuredescs = strarrayapp(measuredescs, "6 hour precip"   );
    measuredescs = strarrayapp(measuredescs, "Humidity"        );
    measuredescs = strarrayapp(measuredescs, "Wind chill"      );
    measuredescs = strarrayapp(measuredescs, "Heat index"      );
    
    for(ii=0 ; ii<cloudElements ; ii++)
    {
      dv = dj_array_element(dj_get_value(dj, "properties.cloudLayers"), ii);
      cloudLayerBaseValue = strarrayapp(cloudLayerBaseValue, dj_value_to_string(dj_get_subvalue(dv, "base.value"   )));
      cloudLayerBaseUnits = strarrayapp(cloudLayerBaseUnits, dj_value_to_string(dj_get_subvalue(dv, "base.unitCode")));
      cloudLayerBaseAmnts = strarrayapp(cloudLayerBaseAmnts, dj_value_to_string(dj_get_subvalue(dv, "amount"       )));
      clouds = strapp(clouds, "Clouds ");
      clouds = strapp(clouds, dj_value_to_string(dj_get_subvalue(dv, "base.value")));
      clouds = strapp(clouds, " ");
      clouds = strapp(clouds, dj_value_to_string(dj_get_subvalue(dv, "base.unitCode")));
      cloudLayerLevel = strarrayapp(cloudLayerLevel, clouds);
      cloudLayerAmnts = strarrayapp(cloudLayerBaseAmnts, dj_value_to_string(dj_get_subvalue(dv, "amount"       )));
    }
    
    for(ii=0 ; measurements && (measurements[ii] != NULL) ; ii++); // ii--;
    wx_print_columns(measuredescs, measurements, ':', ii);

    free_null_terminated_array(&measurements);
    free_null_terminated_array(&measuredescs);
    free_null_terminated_array(&cloudLayerBaseValue);
    free_null_terminated_array(&cloudLayerBaseUnits);
    free_null_terminated_array(&cloudLayerBaseAmnts);
  }
  
  free(iden);
  free(name);
  dj_delete(dj);
}
