
#ifndef __DPRUTIL__DPRSTRINGS_H__
#define __DPRUTIL__DPRSTRINGS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

char *strapp(char *dest, const char *src); /* append src onto dest */
char *strrew(char *dest, const char *src); /* replace dest with src */
#define strfree(__str) { if(__str) { free(__str); __str = NULL; } }

unsigned char *dataapp(unsigned char *dest, size_t oldsz, const unsigned char *src, size_t srcsz);

void replace_chr(char *str, char chf, char chr);

char **strarrayapp(char **dest, const char *src); /* append string to list, NULL-terminate */
void free_null_terminated_array(char ***arr);

char **new_list(const char *src, int delim);

char *replace_str(char *str, char *sf, char *st);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* __DPRUTIL__DPRSTRINGS_H__ */
