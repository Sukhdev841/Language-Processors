%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int to_decimal(char *hex_num);
void to_hex(int,char*);
%}
%token add sem AND OR NOT
%union {int a_number;}
%start lines
%token <a_number> number
%type <a_number> line exp factor factor2

%%
lines : line | lines line;
line : exp sem {printf("result is %d\n",$1);}
      ;
exp : factor2           {$$ = $1;}
    | exp add factor2        {

    											//printf("%s $1  %s $3\n",$1,$3);
    											$$=$1+$3;}
    | exp '-' factor2     {
    												$$=$1-$3;
    											}
    											;
factor2:factor            {$$=$1;}
     |'(' exp ')'         {$$ = $2;}
    |factor2 '*' factor	{
    												$$ = $1*$3;
    								}
			| factor2 '/' factor{
    												$$ = $1/$3;
    								}
    	| factor2 '%' factor {
    												$$ = $1%$3;
    								}
    	| factor2 AND factor {
    												$$ = $1 & $3;
    								}
    	| factor2 OR factor {
    												$$ = $1 | $3;
    								}
			;
factor: number        {$$=$1;}
    ;

%%

int main()
{
  yyparse();
  return 0;
}

void yyerror(char *s)
{
  fprintf(stderr,"%s\n",s);
}
