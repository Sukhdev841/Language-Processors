%{
#include "y.tab.h"
#include <bits/stdc++.h>
using namespace std;
void yyerror(const char* );
%}
%%
";"			{return yytext[0];}
"while"		{return loop_while;}
"float"|"int"|"double"		{yylval.val = (char*)malloc(yyleng); sprintf(yylval.val,"%s",yytext); return data_type;}
[a-zA-Z]+ {yylval.val =(char*)malloc(yyleng); sprintf(yylval.val,"%s",yytext); return var;}
"{"|"}"|"("|")" {return yytext[0];}
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