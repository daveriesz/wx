
#ifndef __DPRJSON_H__
#define __DPRJSON_H__

typedef struct _dprJson dprJson;
typedef struct _dj_value djValue;

dprJson *dj_load_from_file(const char *filename);
dprJson *dj_load_from_data(char *chardata);
void dj_dump(dprJson *dj, FILE *fp);
void dj_delete(dprJson *dj);
dprJson *dj_get_data();
djValue *dj_get_value(dprJson *dj, const char *query);
//void dj_dump_value (djValue  *djv, FILE *fp);
char *dj_value_to_string(djValue *djv);

#endif /* __DPRJSON_H__ */
