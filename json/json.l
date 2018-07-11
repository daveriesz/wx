/* for layout see http://flex.sourceforge.net/manual/index.html */

%{

/* supporting C code */

#include "json.tab.h"
#include "jsondata.h"

typedef void *yyscan_t;


%}

/* Definitions Section */

js_digit19 [1-9]
js_digit   [0-9]
js_digits  {js_digit}+
js_intv    {js_digit}|{js_digit19}{js_digits}
js_int     {js_intv}|-{js_intv}
js_frac    [.]{js_digits}
js_exp     {js_e}{js_digits}
js_e       [eE][-+]?
js_hex     [0-9a-fA-F]
js_uechar  [ -!#-\[\]-~]
js_echar   \\["\\bfnrt/]
js_uncchar \\u{js_hex}{js_hex}{js_hex}{js_hex}
js_extchar [\x80-\xff]
js_char    {js_uechar}|{js_echar}|{js_uncchar}|{js_extchar}
js_chars   {js_char}+
js_quote   [\"]
js_number  {js_int}|{js_int}{js_frac}|{js_int}{js_exp}|{js_int}{js_frac}{js_exp}

%% /* Rules Section */

true       {                       return js_true    ; }
false      {                       return js_false   ; }
null       {                       return js_null    ; }
\{         { dj_push_object(NULL); return js_lbrace  ; }
\}         {                       return js_rbrace  ; }
\[         { dj_push_array (NULL); return js_lbracket; }
\]         {                       return js_rbracket; }
,          {                       return js_comma   ; }
:          {                       return js_colon   ; }

{js_quote}{js_quote}|{js_quote}{js_chars}{js_quote} { /* code */ dj_string(); return js_string; }
{js_number}                                         { /* code */              return js_number; }

[ ]
[\t]
[\n]
\015

. { printf("unexpected: %c ( %s )\n", *yytext, yytext); }

%%

/* more supporting C code */

void yyerror(const char *msg)
{
	fprintf(stderr, "yyerror: %d: %s at %s\n", yylineno, msg, yytext);
//	fprintf(stderr, "yyerror: %s\n", msg);
	exit(1);
}
