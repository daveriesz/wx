
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dprstrings.h>

#include "jsondata.h"
#define DEBUG_PRINT_ENV "DEBUG_JSONDATA_OUTPUT"
#include "debug.h"

extern char *yytext;

#define malloc_obj(ptr, type, size) \
	{ \
		(ptr) = (type *)malloc(size); \
		memset((ptr), 0, (size)); \
	}

#define make_obj(ptr, type) \
	{ \
		(ptr) = (type *)malloc(sizeof(type)); \
		memset((ptr), 0, sizeof(type)); \
	}

/* use rn=0 to return address for creating a new object */
/* use rn=1 to return address of last existing object */
#define ppiter(root,ptr,rn) \
	for( \
		(ptr)=&(root) \
		; \
		((!(rn)) && ( (ptr) && *(ptr) )) || ((rn) && (ptr) && (*(ptr)) && (*(ptr))->next ) \
		; \
		(ptr) = &((*(ptr))->next) \
		)

#define ppindex(root,ptr,idx,ii) \
	for( \
		(ptr)=&(root) , ii=0 \
		; \
		((ii) < (idx)) && (ptr) && (*ptr) \
		; \
		(ptr) = &((*(ptr))->next) \
		)

#define dj_type_funcs_impl1(type,name) \
	type *dj_##name##_root = NULL; \
	type *dj_push_##name(type *pv) \
	{ \
		type **pp; \
		ppiter(dj_##name##_root, pp, 0); /* addr for new item */ \
		if((pv) != NULL) { *pp = pv; } \
		else { make_obj(*pp, type); } \
		return *pp; \
	} \
	type *dj_pop_##name() \
	{ \
		type **pp, *p; \
		ppiter(dj_##name##_root, pp, 1); /* addr of last item */ \
		p = *pp; \
		*pp = NULL; \
		return p; \
	} \
	type *dj_top_##name() \
	{ \
		type **pp; \
		ppiter(dj_##name##_root, pp, 1); /* addr of last item */ \
		return *pp; \
	}

#define dj_type_funcs_impl2(type,name,pname) \
	type *dj_##name##_root = NULL; \
	type *dj_push_##name(type *pv) \
	{ \
		type **pp; \
		/* ppiter(): return addr for new item */ \
		if(dj_##pname##_root != NULL) { ppiter(dj_top_##pname()->name##s, pp, 0); } \
		else { ppiter(dj_##name##_root, pp, 0); } \
		if((pv) != NULL) { *pp = pv; } \
		else { make_obj(*pp, type); } \
		return *pp; \
	} \
	type *dj_pop_##name() \
	{ \
		type **pp, *p; \
		/* ppiter(): return addr of last item */ \
		if(dj_##pname##_root != NULL) { ppiter(dj_top_##pname()->name##s, pp, 1); } \
		else { ppiter(dj_##name##_root, pp, 1); } \
		p = *pp; \
		*pp = NULL; \
		return p; \
	} \
	type *dj_top_##name() \
	{ \
		type **pp; \
		/* ppiter(): return addr of last item */ \
		if(dj_##pname##_root != NULL) { ppiter(dj_top_##pname()->name##s, pp, 1); } \
		else { ppiter(dj_##name##_root, pp, 1); } \
		return *pp; \
	}

dj_type_funcs_impl1(djObject , object        )
dj_type_funcs_impl1(djArray  , array         )
dj_type_funcs_impl1(djString , string        )
dj_type_funcs_impl2(djValue  , value , array )
dj_type_funcs_impl2(djPair   , pair  , object)

void dj_array()
{
//	debugf("*** *** dj_array()\n");
}

void dj_pair()
{
	djPair *pr;
	djString *ps;
	pr = dj_push_pair(NULL);
	ps = dj_pop_string();
	pr->name = ps->string;
	free(ps);
	pr->value = dj_pop_value();
}

void dj_string()
{
	djString *ps;
	debugf("*** dj_string(): %s\n", yytext);
	ps = dj_push_string(NULL);
	malloc_obj(ps->string, char, strlen(yytext)-1);
	strncpy(ps->string, &(yytext[1]), strlen(yytext)-2);
}

void dj_members_1() {}
void dj_members_2() {}
void dj_elements_1() {}
void dj_elements_2() {}

void dj_value(djVType vt)
{
	djValue *pv;
	djString *ps;
#if 1
	const char *svtype = NULL;
	switch(vt)
	{
		case dj_vt_string: svtype = "string"; break;
		case dj_vt_number: svtype = "number"; break;
		case dj_vt_object: svtype = "object"; break;
		case dj_vt_array : svtype = "array" ; break;
		case dj_vt_tfn   : svtype = "tfn"   ; break;
		default: svtype = "unknown"; break;
	}
	debugf("dj_value(%-7s): %s\n", svtype, yytext);
#endif
//	pv = dj_push_value(NULL);
	make_obj(pv, djValue);
	pv->vtype = vt;
	switch(vt)
	{
		case dj_vt_string:
#if 1
			ps = dj_pop_string();
			pv->vstring = ps->string;
			free(ps);
#else
			malloc_obj(pv->vstring, char, strlen(yytext)-1);
			strncpy(pv->vstring, &(yytext[1]), strlen(yytext)-2);
#endif
			debugf("    string: %s\n", pv->vstring);
			break;
		case dj_vt_number:
			malloc_obj(pv->vnumber, char, strlen(yytext)+1);
			strcpy(pv->vnumber, yytext);
			debugf("    number: %s\n", pv->vnumber);
			break;
		case dj_vt_object:
			pv->vobject = dj_pop_object();
			break;
		case dj_vt_array:
			pv->varray = dj_pop_array();
			break;
		case dj_vt_tfn:
			strncpy(pv->vtfn, yytext, 5);
			debugf("    tfn: %s\n", pv->vtfn);
			break;
		default: break;
	}
	dj_push_value(pv);
}

dprJson *dj_get_data()
{
	dprJson *dj;
	make_obj(dj      , dprJson);
	make_obj(dj->root, djValue);
	dj->root->vtype  = dj_vt_object;
	dj->root->vobject = dj_pop_object();
	return dj;
}

void dj_dump_value (djValue  *djv, FILE *fp);
void dj_dump_object(djObject *djo, FILE *fp);
void dj_dump_array (djArray  *dja, FILE *fp);
void dj_dump_string(char     *djs, FILE *fp);
void dj_dump_number(char     *djn, FILE *fp);
void dj_dump_tfn   (char     *djt, FILE *fp);

void dj_dump_value(djValue *djv, FILE *fp)
{
	if(!djv) { return; }
	switch(djv->vtype)
	{
		case dj_vt_string: dj_dump_string(djv->vstring, fp); break;
		case dj_vt_number: dj_dump_number(djv->vnumber, fp); break;
		case dj_vt_object: dj_dump_object(djv->vobject, fp); break;
		case dj_vt_array : dj_dump_array (djv->varray , fp); break;
		case dj_vt_tfn   : dj_dump_tfn   (djv->vtfn   , fp); break;
		default:  break;
	}
}

void dj_dump_object(djObject *djo, FILE *fp)
{
	djPair **ppp;
	fprintf(fp, "{");
	ppiter(djo->pairs, ppp, 0)
	{
		dj_dump_string((*ppp)->name, fp);
		fprintf(fp, ":");
		if((*ppp)->value)
		{
			dj_dump_value((*ppp)->value, fp);
		}
		else
		{
			fprintf(fp, "((value=%p))", (*ppp)->value);
		}
		if((*ppp)->next) { fprintf(fp, ","); }
	}
	fprintf(fp, "}");
}

void dj_dump_array(djArray *dja, FILE *fp)
{
	djValue **vpp;
	fprintf(fp, "[");
	ppiter(dja->values, vpp, 0)
	{
		if((*vpp))
		{
			dj_dump_value(*vpp, fp);
		}
		else
		{
			fprintf(fp, "((value=%p))", (*vpp));
		}
		if((*vpp)->next) { fprintf(fp, ","); }
	}
	fprintf(fp, "]");
}

void dj_dump_string(char *djs, FILE *fp)
{
	fprintf(fp, "\"%s\"", djs);
}

void dj_dump_number(char *djn, FILE *fp)
{
	fprintf(fp, "%s", djn);
}

void dj_dump_tfn(char *djt, FILE *fp)
{
	fprintf(fp, "%s", djt);
}

void dj_dump(dprJson *dj, FILE *fp)
{
//	printf("dump...\n");
	dj_dump_value(dj->root, fp);
	printf("\n");
}

char *dj_value_to_string (djValue  *djv);
char *dj_object_to_string(djObject *djo);
char *dj_array_to_string (djArray  *dja);
char *dj_string_to_string(char     *djs);
char *dj_number_to_string(char     *djn);
char *dj_tfn_to_string   (char     *djt);

char *dj_value_to_string(djValue *djv)
{
	if(!djv) { return NULL; }
	switch(djv->vtype)
	{
		case dj_vt_string: return dj_string_to_string(djv->vstring); break;
		case dj_vt_number: return dj_number_to_string(djv->vnumber); break;
		case dj_vt_object: return dj_object_to_string(djv->vobject); break;
		case dj_vt_array : return dj_array_to_string (djv->varray ); break;
		case dj_vt_tfn   : return dj_tfn_to_string   (djv->vtfn   ); break;
		default:  break;
	}
	return NULL;
}

char *dj_object_to_string(djObject *djo)
{
	djPair **ppp;
	char *cp1=NULL, *cp2=NULL;
	int ll;

	cp1 = strapp(cp1, "{");
	ppiter(djo->pairs, ppp, 0)
	{
		cp2 = dj_string_to_string((*ppp)->name);
		cp1 = strapp(cp1, cp2);
		cp1 = strapp(cp1, ":");
		free(cp2);
		if((*ppp)->value)
		{
			cp2 = dj_value_to_string((*ppp)->value);
			cp1 = strapp(cp1, cp2);
			cp1 = strapp(cp1, ",");
			free(cp2);
		}
		else
		{
			char serr[32];
			sprintf(serr, "((value=0x%p))", (*ppp)->value);
			cp1 = strapp(cp1, serr);
		}
	}
	cp1[strlen(cp1)-1] = '}'; // replace traling comma
	return cp1;
}

char *dj_array_to_string (djArray  *dja)
{
	djValue **vpp;
	char *cp1=NULL, *cp2=NULL;
	int ll;

	cp1 = strapp(cp1, "[");
	ppiter(dja->values, vpp, 0)
	{
		if((*vpp))
		{
			cp2 = dj_value_to_string(*vpp);
			cp1 = strapp(cp1, cp2);
			cp1 = strapp(cp1, ",");
			free(cp2);
		}
		else
		{
			char serr[32];
			sprintf(serr, "((value=0x%p))", (*vpp));
			cp1 = strapp(cp1, serr);
		}
	}
	cp1[strlen(cp1)-1] = ']'; // replace traling comma
	return cp1;
}

char *dj_string_to_string(char     *djs)
{
	char *s;
	malloc_obj(s, char, strlen(djs)+3);
	sprintf(s, "\"%s\"", djs);
	return s;
}

char *dj_number_to_string(char     *djn)
{
	char *s;
	malloc_obj(s, char, strlen(djn)+1);
	sprintf(s, "\"%s\"", djn);
	return s;
}

char *dj_tfn_to_string   (char     *djt)
{
	char *s;
	malloc_obj(s, char, strlen(djt)+1);
	sprintf(s, "\"%s\"", djt);
	return s;
}

void dj_delete_value (djValue  **djv);
void dj_delete_object(djObject **djo);
void dj_delete_array (djArray  **dja);
void dj_delete_pair  (djPair   **djp);
void dj_delete_string(char     **djs);
void dj_delete_number(char     **djn);

void dj_delete_value(djValue **djv)
{
	djValue *vp;
	if(!djv) { return; }
	switch((*djv)->vtype)
	{
		case dj_vt_string: dj_delete_string(&((*djv)->vstring)); break;
		case dj_vt_number: dj_delete_number(&((*djv)->vnumber)); break;
		case dj_vt_object: dj_delete_object(&((*djv)->vobject)); break;
		case dj_vt_array : dj_delete_array (&((*djv)->varray )); break;
		case dj_vt_tfn   : /* nothing to delete */               break;
		default:  break;
	}
	vp = *djv;
	*djv = vp->next;
	free(vp);
}

void dj_delete_object(djObject **djo)
{
	djObject *op;
	if(!djo || !(*djo)) { return; }
	while((*djo)->pairs) { dj_delete_pair(&((*djo)->pairs)); }
	op = *djo;
	*djo = op->next;
	free(op);
}

void dj_delete_array(djArray **dja)
{
	djArray *ap;
	if(!dja || !(*dja)) { return; }
	while((*dja)->values) { dj_delete_value(&(*dja)->values); }
	ap = *dja;
	*dja = ap->next;
	free(ap);
}

void dj_delete_pair(djPair **djp)
{
	djPair *pp;
	if(!djp || !(*djp)) { return; }
	dj_delete_string(&((*djp)->name ));
	while((*djp)->value) { dj_delete_value (&((*djp)->value)); }
	pp = *djp;
	*djp = pp->next;
	free(pp);
}

void dj_delete_string(char **djs)
{
	if(!djs || !(*djs)) { return; }
	free(*djs);
	*djs = NULL;
}

void dj_delete_number(char **djn)
{
	if(!djn || !(*djn)) { return; }
	free(*djn);
	*djn = NULL;
}

void dj_delete(dprJson *dj)
{
	dj_delete_value(&(dj->root));
}

/*
 * rv return codes:
 * 0: no error
 * -1: not found
 * -2: query format error
 * -3: array queried, non-array encountered
 * -4: object queried, non-object encountered
 * -5: query format error
 */
 
djValue *dj_value_query(djValue *vp, const char *query, int *rv)
{
	const char *cp;
	char *id, *ap, *ep;
	djValue *djv;
	djPair *djp;
	int ii, ll, index=-1;
	
	*rv = 0;
	
// no nulls!

	if(!vp || !query || !rv) { return vp; }
	
// find the end of the element id
	for(cp=query ; cp && *cp && ((*cp) != '.') ; cp++);
	ll = cp - query;
	malloc_obj(id, char, ll+1);
	strncpy(id, query, ll);

// check for an array index
	if(ap = strchr(id, '['))
	{
		if(ep = strchr(ap, ']'))
		{
			*ep = '\0';
			*ap = '\0';
			ap++;
			index = atol(ap);
		}
		else
		{
			djv = NULL;
			*rv = -5; // query format error
			// printf("XX - returning %d\n", *rv);
		}
	}

	// printf("id: %s\n", id);
//	if(index >= 0) { printf("    %d (ap=%s)\n", index, ap); }

// find the correct value-pair
	if(vp->vtype == dj_vt_object)
	{
		for(djp=(vp->vobject->pairs) ; djp && djp->name && strcmp(djp->name, id) ; djp=(djp->next));
		if(djp == NULL)
		{
			djv = NULL;
			*rv = -1; // not found
			// printf("XX - returning %d\n", *rv);
		}
		else
		{
			djv = (djp->value);
		}
	}
	else
	{
		djv = NULL;
		*rv = -4; // object queried, non-object encountered
		// printf("XX - returning %d\n", *rv);
	}

// querying an array element? find the correct element
	if(djv && (index >= 0))
	{
		if(djv->vtype == dj_vt_array)
		{
			for(ii=0, djv=(djv->varray->values) ; djv && ii<index ; ii++, djv=(djv->next));
			if(djv == NULL)
			{
				*rv = -1; // not found
				// printf("XX - returning %d\n", *rv);
			}
		}
		else
		{
			djv = NULL;
			*rv = -3; // array queried, non-array encountered
			// printf("XX - returning %d\n", *rv);
		}
	}

// not the end of query?
	if(djv && ((*cp) == '.'))
	{
		djv = dj_value_query(djv, cp+1, rv);
	}

	free(id);
	return djv;
}


djValue *dj_get_value(dprJson *dj, const char *query)
{
	int rv;
	// printf("query: %s\n", query);
	return dj_value_query(dj->root, query, &rv);
}

djValue *dj_get_subvalue(djValue *dv, const char *query)
{
  int rv;
  djValue *djv;
  return dj_value_query(dv, query, &rv);
}
