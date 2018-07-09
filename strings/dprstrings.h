
#ifndef __DPRUTIL__DPRSTRINGS_H__
#define __DPRUTIL__DPRSTRINGS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

char *strapp(char *dest, const char *src);
char *strrew(char *dest, const char *src);
#define strfree(__str) { if(__str) { free(__str); __str = NULL; } }

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* __DPRUTIL__DPRSTRINGS_H__ */
