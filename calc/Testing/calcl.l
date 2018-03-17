%{
#include "y.tab.h"
#include "global.h"
int size;
//char *operators_;
//int *my_priority_;
int get_periority(char*);
%}
%%
[0-9]+ {yylval.a_number = atoi(yytext);return number;}
[ \t\n] ;
[;] {yylval.op_type=';';return sem;}
[+\-*/] {yylval.op_type=yytext[0];return get_periority(yytext);}
. {ECHO; yyerror("unexpected character");}
%%
int get_periority(char *str)
{
  for(int i=0;i<size;i++)
  {
    if(str[0] == operators_[i])
        return my_priority_[i];
  }
  return per1;
}
int yywrap(){
return 1;
}
