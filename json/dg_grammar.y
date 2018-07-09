
%{

#include <stdio.h>

#include "variable.h"
#include "ifelse.h"
#include "context.h"
#include "source.h"

extern char *yytext;
extern long yylineno;

extern char yystringV[];
extern char yyderefV[102];
extern char yygroupV[];
extern char yyvariableV[];
extern long yybracketV;
extern double yydoubleV;
extern long yylongV;
extern long yyhexV;

char project_name[1024];
long package_owner;
char target_type[1024];

char ifvalueA[1024];
char ifvalueB[1024];

char pchname[1024];

char *dg_strcpy(char *s1, const char *s2);

void print_summary();

%}

%token MicrosoftDeveloperStudio
%token ProjectFileName
%token PackageOwner
%token GeneratedBuildFile
%token TargetType
%token DoNotEdit
%token Message
%token BeginProject
%token EndProject
%token BeginTarget
%token BeginSourceFile
%token BeginCustomBuild
%token BeginGroup
%token BeginSection
%token EndSourceFile
%token EndGroup
%token EndTarget
%token EndProject
%token EndCustomBuild
%token EndSection
%token PropertyBase
%token PropertyNoBase
%token BeginIf
%token ElseIf
%token EndIf
%token AddNoBase
%token AddBase
%token SubtractNoBase
%token SubtractBase
%token NameLine
%token RandomLine
%token DeRefValue
%token RuleCommand
%token SectionConstruct
%token Variable

%token randomcharV
%token bracketV
%token stringV
%token uqstringV
%token doubleV
%token hexV

%token spcolonS
%token spequalS

%%

DSP_Stream:
		DSP_ConstructList
	;

DSP_ConstructList:
		DSP_Construct
	|	DSP_Construct DSP_ConstructList
	;

DSP_Construct:
		DSP_ProjectFile
	|	DSP_GeneratedBuildFile
	|	DSP_DoNotEdit
	|	DSP_TargetType
	|	DSP_Variable
	|	DSP_Message
	|	DSP_ProjectBlock
	|   DSP_Property
	|	DSP_IfElseBlock
	|	DSP_Add
	|	DSP_Subtract
	|	DSP_TargetBlock
	|	DSP_NameLine
	|	DSP_Group
	|	DSP_SourceFileBlock
	|	DSP_CustomBuildBlock
	|	DSP_Section
	;

DSP_ProjectFile:
		MicrosoftDeveloperStudio DSP_ProjectFileName DSP_PackageOwner { printf("project file line\n"); }
	;

DSP_ProjectFileName:
		ProjectFileName stringV
			{
				strcpy(project_name, yystringV);
				sprintf(pchname, "%s.pch", project_name);
				printf("project name = \"%s\"\n", project_name);
				push_context(yystringV);
			}
	;

DSP_PackageOwner:
		PackageOwner bracketV   { printf("package owner = < %d >\n", yybracketV); }
	;

DSP_GeneratedBuildFile:
		MicrosoftDeveloperStudio GeneratedBuildFile doubleV { printf("generated build file (v %f)\n", yydoubleV); }
	;

DSP_DoNotEdit:
		DoNotEdit /* throw away */
	;

DSP_TargetType:
		TargetType stringV hexV { printf("target type: \"%s\" 0x%x\n", yystringV, yyhexV); }
	;

DSP_Variable:
		Variable { if(ifelse_available()) assign_variable(yyvariableV, 0); }
	;

DSP_Message:
		Message /* throw away */
	;

DSP_ProjectBlock:
		DSP_BeginProject DSP_ConstructList DSP_EndProject
	;

DSP_BeginProject:
		BeginProject { if(ifelse_available()) push_context("project"); }
	;

DSP_EndProject:
		EndProject { if(ifelse_available()) pop_context(); print_summary(); }
	;

DSP_Property:
		PropertyBase   /* throw away */
	|	PropertyNoBase { if(ifelse_available()) assign_variable(yyvariableV, 0); }
	;

DSP_IfElseBlock:
		DSP_BeginIf DSP_EndIf
	|	DSP_BeginIf DSP_ElseBlock
	|	DSP_BeginIf DSP_ConstructList DSP_EndIf
	|	DSP_BeginIf DSP_ConstructList DSP_ElseBlock
	;

DSP_ElseBlock:
		DSP_ElseIf DSP_EndIf
	|	DSP_ElseIf DSP_ElseBlock
	|	DSP_ElseIf DSP_ConstructList DSP_EndIf
	|	DSP_ElseIf DSP_ConstructList DSP_ElseBlock
	;

DSP_BeginIf:
		BeginIf DSP_IfElseCondition
	;

DSP_ElseIf:
		ElseIf DSP_IfElseCondition
	;

DSP_EndIf:
		EndIf { current_ifelse_state = idle; }
	;

DSP_IfElseCondition:
		DSP_IfValueA spequalS DSP_IfValueB
		{
			if(!strcmp(ifvalueA, ifvalueB))
				current_ifelse_state = active;
			else if(current_ifelse_state == active) current_ifelse_state = finished;
			else current_ifelse_state = testing;
		}
	;

DSP_IfValueA:
		DSP_DeRefValue { bzero(ifvalueA, 1024); dg_strcpy(ifvalueA, variable_value(yyderefV)); }
	|	stringV        { bzero(ifvalueA, 1024); dg_strcpy(ifvalueA, yystringV); }
	;

DSP_IfValueB:
		DSP_DeRefValue { bzero(ifvalueB, 1024); dg_strcpy(ifvalueB, variable_value(yyderefV)); }
	|	stringV        { bzero(ifvalueB, 1024); dg_strcpy(ifvalueB, yystringV); }
	;

DSP_Add:
		AddNoBase { if(ifelse_available()) assign_variable(yyvariableV, 1); }
	|	AddBase   /* throw away */
	;

DSP_Subtract:
		SubtractNoBase
	|	SubtractBase   /* throw away */
	;

DSP_TargetBlock:
		DSP_BeginTarget DSP_ConstructList DSP_EndTarget
	;

DSP_BeginTarget:
		BeginTarget { if(ifelse_available()) push_context("target"); }
	;

DSP_EndTarget:
		EndTarget  { if(ifelse_available()) pop_context(); }
	;

DSP_NameLine:
		NameLine stringV
	;

DSP_Group:
		DSP_BeginGroup DSP_ConstructList DSP_EndGroup
	;

DSP_BeginGroup:
		BeginGroup { if(ifelse_available()) push_context("group"); }
	;

DSP_EndGroup:
		EndGroup { if(ifelse_available()) pop_context(); }
	;

DSP_SourceFileBlock:
		DSP_BeginSourceFile DSP_ConstructList DSP_EndSourceFile
	;

DSP_BeginSourceFile:
		BeginSourceFile { if(ifelse_available()) push_context("source file"); }
	;

DSP_EndSourceFile:
		EndSourceFile { if(ifelse_available()) { process_source_file(); pop_context(); } }
	;

DSP_CustomBuildBlock:
		DSP_BeginCustomBuild DSP_CustomBuildConstructList DSP_EndCustomBuild
	;

DSP_BeginCustomBuild:
		BeginCustomBuild { if(ifelse_available()) push_context("custom build"); }
	;

DSP_EndCustomBuild:
		EndCustomBuild { if(ifelse_available()) pop_context(); }
	;

DSP_CustomBuildConstructList:
		DSP_CustomBuildConstruct
	|	DSP_CustomBuildConstruct DSP_CustomBuildConstructList
	;

DSP_CustomBuildConstruct:
		DSP_Variable
	|	DSP_Rule
	;

DSP_Rule:
		DSP_RuleDef DSP_RuleCommandList
	;

DSP_RuleDef:
		DSP_RuleTarget spcolonS DSP_RuleDependencyList
	;

DSP_RuleTarget:
		stringV        { if(ifelse_available()) printf("rule target (%s)\n", yystringV); }
	|	DSP_DeRefValue { if(ifelse_available()) printf("rule target (%s)\n", variable_value(yyderefV)); }
	;

DSP_RuleDependencyList:
		DSP_RuleDependency
	|	DSP_RuleDependency DSP_RuleDependencyList
	;

DSP_RuleDependency:
		stringV        { if(ifelse_available()) printf("rule dependency (%s)\n", yystringV); }
	|	DSP_DeRefValue { if(ifelse_available()) printf("rule dependency (%s)\n", variable_value(yyderefV)); }
	;

DSP_DeRefValue:
		DeRefValue
	;

DSP_RuleCommandList:
		DSP_RuleCommand
	|	DSP_RuleCommand DSP_RuleCommandList
	;

DSP_RuleCommand:
		RuleCommand
	;

DSP_Section:
		BeginSection DSP_SectionConstructList EndSection
	;

DSP_SectionConstructList:
		DSP_SectionConstruct
	|	DSP_SectionConstruct DSP_SectionConstructList
	;

DSP_SectionConstruct:
		SectionConstruct
	;

%%


int yyerror(const char *msg)
{
	printf("\nyyerror!\n");
	printf("%d: %s at '%s'\n", yylineno, msg, yytext);
	exit(1);
}

char *dg_strcpy(char *s1, const char *s2)
{
	if(!s1) return NULL;
	else if(!s2) *s1 = '\0';
	else strcpy(s1, s2);

	return s1;
}

void print_summary()
{
	printf("*** SUMMARY INFORMATION ***\n");
	printf("PCH file = %s\n", pchname);
}

