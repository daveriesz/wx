
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <curl/curl.h>

#include <dprstrings.h>

#include "wx.h"

struct wx_writedata {
  size_t size;
  char *data;
};

size_t wx_fetch_write(void *ptr, size_t size, size_t count, void *userdata)
{
  struct wx_writedata *wxd = (struct wx_writedata *)userdata;
  size_t insize;
  size_t outsize;
  
  insize  = (size * count);
  outsize = (insize + wxd->size);

//  printf("wx_fetch_write: insize=%ld, wxwsize=%ld\n", insize, wxd->size);

  wxd->data = dataapp(wxd->data, wxd->size, (unsigned char *)ptr, insize);
  wxd->size += insize;

//  *useradata = realloc(*userdata, (size*count));
//  memcpy(*userdata, ptr, (size*count));

//  printf("wx_fetch_write: returning %ld\n", insize);

  return (insize);
}


char *wx_fetch_url(const char *url)
{
  CURL *crl;
  struct wx_writedata wxd;
  memset(&wxd, 0, sizeof(wxd));

//  printf("wx_fetch_url: URL: >>%s<<\n", url);

  crl = curl_easy_init();

  curl_easy_setopt (crl, CURLOPT_URL           , url           );
  curl_easy_setopt (crl, CURLOPT_WRITEFUNCTION , wx_fetch_write);
  curl_easy_setopt (crl, CURLOPT_WRITEDATA     , &wxd          );
  curl_easy_setopt (crl, CURLOPT_FOLLOWLOCATION, 1L            );
  curl_easy_setopt (crl, CURLOPT_USERAGENT     , "dpr wx/1.0"  );
  curl_easy_perform(crl);
  curl_easy_cleanup(crl);

  wxd.data = (char *)realloc(wxd.data, wxd.size+1);
  wxd.data[wxd.size] = '\0';

//  printf("wx_fetch_url finished\n");

  return wxd.data;
}

