%{

/* supporting C code */

#include <stdio.h>
#include <stdlib.h>

#include "jsondata.h"

void yyerror(const char *msg);
//int yyparse(void);
int yylex(void);  
int yywrap() { return 1; }

%}

/* tokens */

%token js_string
%token js_chars
%token js_char
%token js_number
%token js_int
%token js_frac
%token js_exp
%token js_digits
%token js_e
%token js_true
%token js_false
%token js_null
%token js_lbrace
%token js_rbrace
%token js_lbracket
%token js_rbracket
%token js_comma
%token js_colon

 
%%

/* grammar from http://www.json.org/ */

/* constructs */

js_object:
		js_lbrace js_rbrace                 { /* dj_object(); */ }
	|	js_lbrace js_members js_rbrace      { /* dj_object(); */ }
	;
js_members:
		js_pair                             { dj_members_1(); }
	|	js_pair js_comma js_members         { dj_members_2(); }
	;
js_pair:
		js_string js_colon js_value         { dj_pair(); }
	;
js_array:
		js_lbracket js_rbracket             { dj_array(); }
	|	js_lbracket js_elements js_rbracket { dj_array(); }
	;
js_elements:
		js_value                            { dj_elements_1(); }
	|	js_value js_comma js_elements       { dj_elements_2(); }
	;
js_value:
		js_string                           { dj_value(dj_vt_string); }
	|	js_number                           { dj_value(dj_vt_number); }
	|	js_object                           { dj_value(dj_vt_object); }
	|	js_array                            { dj_value(dj_vt_array ); }
	|	js_true                             { dj_value(dj_vt_tfn   ); }
	|	js_false                            { dj_value(dj_vt_tfn   ); }
	|	js_null                             { dj_value(dj_vt_tfn   ); }
	;

%%

/* more C code */

