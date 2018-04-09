%{
#include "y.tab.h"
#include <bits/stdc++.h>
using namespace std;
void yyerror(const char* );
%}
%%
";"			{return yytext[0];}
"if"		{return keyword_if;}
"else"		{return keyword_else;}
"while"		{return loop_while;}
"float"|"int"|"double"		{yylval.val = (char*)malloc(yyleng); sprintf(yylval.val,"%s",yytext); return data_type;}
[a-zA-Z]+ {yylval.val =(char*)malloc(yyleng); sprintf(yylval.val,"%s",yytext); return var;}
"{"|"}"|"("|")"|"=" {return yytext[0];}
[+-]				{yylval.val = (char*)malloc(yyleng); sprintf(yylval.val,"%s",yytext); return binary_oper_per1; }
[*/]				{yylval.val = (char*)malloc(yyleng); sprintf(yylval.val,"%s",yytext); return binary_oper_per2; }
" " {}
"\n" {}
. {}
%%
int yywrap(){
return 1;
}

void yyerror(const char *s)
{
	printf("\nError %s \n",s);
}