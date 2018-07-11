
#ifndef __DPRJSON_H__
#define __DPRJSON_H__

/* djVType */
enum _dj_vtype { dj_vt_string , dj_vt_number , dj_vt_object , dj_vt_array , dj_vt_tfn };
typedef enum   _dj_vtype    djVType   ;

typedef struct _dprJson dprJson;
typedef struct _dj_value djValue;

dprJson *dj_load_from_file(const char *filename);
dprJson *dj_load_from_data(char *chardata);
void dj_dump(dprJson *dj, FILE *fp);
void dj_delete(dprJson *dj);
dprJson *dj_get_data();
djValue *dj_get_value   (dprJson *dj, const char *query);
djValue *dj_get_subvalue(djValue *dv, const char *query);
//void dj_dump_value (djValue  *djv, FILE *fp);
char *dj_value_to_string(djValue *djv);
djValue *dj_value_next(djValue *djv);

/* -1=not an array, 0=empty, >0=length */
int dj_array_length(djValue *djv);

djValue *dj_array_element(djValue *djv, int nn);

djVType dj_value_type(djValue *djv);

double dj_value_to_number(djValue *dv);

#endif /* __DPRJSON_H__ */
