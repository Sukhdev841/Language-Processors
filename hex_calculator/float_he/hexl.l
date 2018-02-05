%{
#include "y.tab.h"
#include <string.h>
%}
%%
[0-9a-z]+['.']*[0-9a-z]* {yylval.a_number=malloc(yyleng); sprintf(yylval.a_number,"%s",yytext);return number;}
[ \t\n] ;
[+]  return add;
[;] return sem;
[-*/()%] {return yytext[0];}
[&]	return AND;
[|] return OR;
[~] return NOT;
. {ECHO; yyerror("unexpected character");}
%%
int yywrap(){
return 1;
}
