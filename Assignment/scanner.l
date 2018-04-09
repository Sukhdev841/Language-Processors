%{
#include "y.tab.h"
#include <bits/stdc++.h>
using namespace std;
void yyerror(const char* );
%}
%%
";"			{return yytext[0];}
":"			{return tok_colon;}
"if"		{return keyword_if;}
"else"		{return keyword_else;}
"while"		{return loop_while;}
"int"		{return tok_int;}
"float"		{return tok_float;}
"double"	{return tok_double;}
"bool"		{return tok_bool;}
"switch"	{return tok_switch_stmt;}
"case"		{return tok_case;}
"default"	{return tok_default;}
"break"		{return tok_break;}
[a-zA-Z]+ 	{yylval.val =(char*)malloc(yyleng); sprintf(yylval.val,"%s",yytext); return var;}
[0-9]+		{yylval.val =(char*)malloc(yyleng); sprintf(yylval.val,"%s",yytext); return tok_num;}
"{"|"}"|"("|")"|"=" {return yytext[0];}

"+"			{return tok_plus;}
"-"			{return tok_minus;}
"*"			{return tok_mul;}
"/"			{return tok_div;}
"%"			{return tok_mod;}
"<"			{return tok_less_than;}
"<="		{return tok_less_eql;}
">"			{return tok_greater_than;}
">="		{return tok_greater_eql;}
"=="		{return tok_equal_to;}
"!="		{return tok_not_equal_to;}
"~"			{return tok_b_not;}
"&"			{return tok_b_and;}
"|"			{return tok_b_or;}
"^"			{return tok_b_xor;}
"&&"		{return tok_l_and;}
"||"		{return tok_l_or;}
"!"			{return tok_l_not;}

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