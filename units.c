
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wx.h"

void units_temperature(char **value, char **units)
{
  double fwb = 211.97;
  double cwb = 100.0 ;
  double fwf =  32.0 ;
  double cwf =   0.0 ;
  char vbuf[128], ubuf[128];
  double val = atof(*value);
  double newval;
  
  memset(vbuf, 0, 128);
  memset(ubuf, 0, 128);
  
  if(!strcmp(*units, "unit:degC"))
  {
    if(using_metric()) { sprintf(ubuf, "\u00B0C"); free(*units); *units = strdup(ubuf); }
    else
    {
      newval = (((fwb - fwf) / (cwb - cwf)) * val ) + 32.0;
      sprintf(vbuf, "%.1f", newval); free(*value); *value = strdup(vbuf);
      sprintf(ubuf, "\u00B0F");      free(*units); *units = strdup(ubuf);
    }
  }
  else if(!strcmp(*units, "unit:degF"))
  {
    if(using_imperial()) { sprintf(ubuf, "\u00B0F"); free(*units); *units = strdup(ubuf); }
    else
    {
      newval = (val - 32.0) * ((cwb - cwf) / (fwb - fwf));
      sprintf(vbuf, "%.1f", newval); free(*value); *value = strdup(vbuf);
      sprintf(ubuf, "\u00B0C");      free(*units); *units = strdup(ubuf);
    }
  }
}

void units_angle(char **value, char **units)
{
  if(strcmp(*units, "unit:degree_(angle)")) { return; }
  free(*units);
  *units = strdup("\u00B0");  
}

void units_velocity(char **value, char **units)
{
  double cvf = 2.2369363;
  char vbuf[128], ubuf[128];
  double val = atof(*value);
  double newval;

  memset(vbuf, 0, 128);
  memset(ubuf, 0, 128);

  if(!strcmp(*units, "unit:m_s-1"))
  {
    if(using_metric()) { sprintf(ubuf, "m/s"); free(*units); *units = strdup(ubuf); }
    else
    {
      newval = val * cvf;
//      printf("velocity val=%f, cvf=%f, newval=%f, val*cvf=%f\n", val, cvf, newval, (val*cvf));
      sprintf(vbuf, "%.1f", newval); free(*value); *value = strdup(vbuf);
      sprintf(ubuf, "mph");          free(*units); *units = strdup(ubuf);
    }
  }
  else if(!strcmp(*units, "unit:mi_hr-1"))
  {
    if(using_imperial()) { sprintf(ubuf, "mph"); free(*units); *units = strdup(ubuf); }
    else
    {
      newval = val / cvf;
      sprintf(vbuf, "%.1f", newval); free(*value); *value = strdup(vbuf);
      sprintf(ubuf, "m/s");          free(*units); *units = strdup(ubuf);
    }
  }
}

void units_pressure(char **value, char **units)
{
  double cvf = 0.00029529983;
  char vbuf[128], ubuf[128];
  double val = atof(*value);
  double newval;

  memset(vbuf, 0, 128);
  memset(ubuf, 0, 128);

  if(!strcmp(*units, "unit:Pa"))
  {
    if(using_metric()) { sprintf(ubuf, "Pa"); free(*units); *units = strdup(ubuf); }
    else
    {
      newval = val * cvf;
//      printf("velocity val=%f, cvf=%f, newval=%f, val*cvf=%f\n", val, cvf, newval, (val*cvf));
      sprintf(vbuf, "%.1f", newval); free(*value); *value = strdup(vbuf);
      sprintf(ubuf, "inHg");         free(*units); *units = strdup(ubuf);
    }
  }
  else if(!strcmp(*units, "unit:mi_hr-1"))
  {
    if(using_imperial()) { sprintf(ubuf, "inHg"); free(*units); *units = strdup(ubuf); }
    else
    {
      newval = val / cvf;
      sprintf(vbuf, "%.1f", newval); free(*value); *value = strdup(vbuf);
      sprintf(ubuf, "Pa");           free(*units); *units = strdup(ubuf);
    }
  }
}

void units_distance(char **value, char **units)
{
  double cvf = 3.2808399;
  char vbuf[128], ubuf[128];
  double val = atof(*value);
  double newval;

  memset(vbuf, 0, 128);
  memset(ubuf, 0, 128);

  if(!strcmp(*units, "unit:m"))
  {
    if(using_metric()) { sprintf(ubuf, "m/s"); free(*units); *units = strdup(ubuf); }
    else
    {
      newval = val * cvf;
//      printf("velocity val=%f, cvf=%f, newval=%f, val*cvf=%f\n", val, cvf, newval, (val*cvf));
      sprintf(vbuf, "%.1f", newval); free(*value); *value = strdup(vbuf);
      sprintf(ubuf, "ft");          free(*units); *units = strdup(ubuf);
    }
  }
  else if(!strcmp(*units, "unit:ft"))
  {
    if(using_imperial()) { sprintf(ubuf, "ft"); free(*units); *units = strdup(ubuf); }
    else
    {
      newval = val / cvf;
      sprintf(vbuf, "%.1f", newval); free(*value); *value = strdup(vbuf);
      sprintf(ubuf, "m");            free(*units); *units = strdup(ubuf);
    }
  }
}

void convert_units(char **value, char **units)
{
  if(!strcmp(*value, "null")) { free(*value); *value = strdup("0.0"); }
//  printf("change units: %s\n", using_imperial()?"imperial":"metric");
  if     (!strcmp(*units, "unit:degC"          )) { units_temperature(value, units); }
  else if(!strcmp(*units, "unit:degree_(angle)")) { units_angle      (value, units); }
  else if(!strcmp(*units, "unit:m_s-1"         )) { units_velocity   (value, units); }
  else if(!strcmp(*units, "unit:Pa"            )) { units_pressure   (value, units); }
  else if(!strcmp(*units, "unit:m"             )) { units_distance   (value, units); }
  else if(!strcmp(*units, "unit:ft"            )) { units_distance   (value, units); }
  else if(!strcmp(*units, "unit:percent"       )) { free(*units); *units = strdup("%");}
//  printf("... val=%s, unit=%s\n", *value, *units);
}
