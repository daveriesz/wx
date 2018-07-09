
#ifndef __JSONDATA_H__
#define __JSONDATA_H__

#include "dprjson.h"

typedef struct _dj_object   djObject  ;
typedef struct _dj_pair     djPair    ;
typedef struct _dj_array    djArray   ;
typedef struct _dj_value    djValue   ;
typedef struct _dj_string   djString  ;
typedef enum   _dj_vtype    djVType   ;

// typedef struct _dj_members  djMembers ; /* linked list of pairs */
// typedef struct _dj_elements djElements; /* linked list of values */

/* djObject */
struct _dj_object
{
	djPair *pairs;
	djObject *next;
};

/* djPair */
struct _dj_pair
{
	char *name;
	djValue *value;
	djPair *next;
};

/* djArray */
struct _dj_array
{
	djValue *values;
	djArray *next;
};

/* djVType */
enum _dj_vtype { dj_vt_string , dj_vt_number , dj_vt_object , dj_vt_array , dj_vt_tfn };

/* djValue */
struct _dj_value
{
	djVType vtype;
	union
	{
		char     *vstring;
		char     *vnumber;
		djObject *vobject;
		djArray  *varray;
		char      vtfn[6]; /* true, false, null */
	};

	djValue *next;
	djValue *prev;
};

/* djString */
struct _dj_string
{
	char *string;
	djString *next;
};

#define dj_type_funcs_decl(type,name) \
	type *dj_push_##name(type *pv); \
	type *dj_pop_##name(); \
	type *dj_top_##name();
	
dj_type_funcs_decl(djObject , object)
dj_type_funcs_decl(djArray  , array )
dj_type_funcs_decl(djValue  , value )
dj_type_funcs_decl(djPair   , pair  )
dj_type_funcs_decl(djString , string)

void dj_members_1();
void dj_members_2();
void dj_pair();
void dj_array();
void dj_elements_1();
void dj_elements_2();
void dj_value(djVType vt);
void dj_string();

struct _dprJson
{
	djValue *root;
};

#endif /* __JSONDATA_H__ */
