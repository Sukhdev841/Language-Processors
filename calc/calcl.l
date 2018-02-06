%{
#include "y.tab.h"
%}
%%
[0-9]+ {yylval.a_number = atoi(yytext);return number;}
[ \t\n] ;
[+]  return add;
[;] return sem;
[-*/()%] {return yytext[0];}
. {ECHO; yyerror("unexpected character");}
%%
int yywrap(){
return 1;
}
