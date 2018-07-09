
%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dg_grammar.y.h"

long yylineno;

char yystringV[1024];
char yyderefV[1024];
char yygroupV[1024];
char yyvariableV[1024];
long yybracketV;
double yydoubleV;
long yylongV;
long yyhexV;

#define ud undosify(yytext)

void undosify(char *s)
{
	char *ch;
	if((ch = strchr(s, 0x0d))) *ch = '\0';
}

void lex_variable_assignment(char *yt, char *pfx)
{
	char *cp;
	strcpy(yyvariableV, &(yt[strlen(pfx)]));

	if(cp = strchr(yyvariableV, ' ')) *cp = '=';
}

/***************************************
***************************************/

%}

%%

\015
[ ]
[\n]                                    yylineno++;

^"# Microsoft Developer Studio"         ud; return MicrosoftDeveloperStudio;
"Project File - Name="                  ud; return ProjectFileName;
"- Package Owner="                      ud; return PackageOwner;
"Generated Build File, Format Version"  ud; return GeneratedBuildFile;
^"# ** DO NOT EDIT **"                  ud; return DoNotEdit;
^"# TARGTYPE"                           ud; return TargetType;
^"!MESSAGE ".*$                         ud; return Message;
^"# PROP BASE ".*                       ud; return PropertyBase;
^"# PROP ".*$                           ud; lex_variable_assignment(yytext, "# PROP "); return PropertyNoBase;
^"# Begin Project".*$                   ud; return BeginProject;
^"# Begin Target".*$                    ud; return BeginTarget;
^"# Begin Source File".*$               ud; return BeginSourceFile;
^"# Begin Custom Build".*$              ud; return BeginCustomBuild;
^"# Begin Group".*$                     ud; strcpy(yygroupV, &(yytext[strlen("# Begin Group ")])); return BeginGroup;
^"# Section".*$                         ud; return BeginSection;
^"# End Project".*$                     ud; return EndProject;
^"# End Source File".*$                 ud; return EndSourceFile;
^"# End Group".*$                       ud; return EndGroup;
^"# End Target".*$                      ud; return EndTarget;
^"# End Custom Build".*$                ud; return EndCustomBuild;
^"# End Section".*$                     ud; return EndSection;
^"!IF "                                 ud; return BeginIf;
^"!ELSEIF "                             ud; return ElseIf;
^"!ENDIF".*$                            ud; return EndIf;
^"# ADD BASE ".*$                       ud; return AddBase;
^"# ADD ".*$                            ud; lex_variable_assignment(yytext, "# ADD "); return AddNoBase;
^"# SUBTRACT BASE ".*$                  ud; return SubtractBase;
^"# SUBTRACT ".*$                       ud; return SubtractNoBase;
^"# Name "                              ud; return NameLine;
^\t.*$                                  ud; return RuleCommand;
$\([a-zA-Z_][a-zA-Z_0-9]*\)             ud; bzero(yyderefV, 1024); strncpy(yyderefV, &(yytext[2]), strlen(yytext)-3); return DeRefValue;
\"$\([a-zA-Z_][a-zA-Z_0-9]*\)\"         ud; bzero(yyderefV, 1024); strncpy(yyderefV, &(yytext[3]), strlen(yytext)-5); return DeRefValue;
^"#"[\t ]+[0-9].*$                      ud; return SectionConstruct;
^[a-zA-Z_][a-zA-Z_0-9]*=.*$             ud; strcpy(yyvariableV, yytext); return Variable;

\<[0-9]+\>           ud; *(strchr(yytext, '>')) = '\0'; yybracketV = atol(&(yytext[1])); return bracketV;
\"[^\"]*\"           ud; bzero(yystringV, 1024); strncpy(yystringV, &(yytext[1]), strlen(yytext)-2);             return stringV;
[0-9]*\.[0-9]+       ud; yydoubleV = atof(yytext);                                       return doubleV;
0x[0-9]+             ud; sscanf(yytext, "0x%x", &yyhexV);                                return hexV;
" : "                ud; return spcolonS;
" == "               ud; return spequalS;

%%


int run_lexer(char *dspfile)
{
	FILE *dspfp;

/*	printf("attempting to parse %s\n", dspfile); */

	if(!(dspfp = fopen(dspfile, "r")))
	{
		fprintf(stderr, "Error:  could not open %s for read.\n", dspfile);
		return 1;
	}

	yylineno = 1;

	yyin = dspfp;

	yyparse();

	return 0;
}

