%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
//#include "global.h"
int size;
char *operators_;
int *my_priority_;
int operation(char,int,int);
%}
%union
 {
 int a_number;
 char op_type;
 }
%start lines
%token <a_number> number
%token <op_type> sem per1 per2 per3 per4
%type <a_number> line exp factor2 factor3 factor4 factor5

%%
lines : line | lines line;
line : exp sem {printf("result is %d\n",$1);}
      ;
exp : factor2           {$$ = $1;}
    | exp per1 factor2     {
    								 printf("periority 1 sign %c",$2);
                            $$ = operation($2,$1,$3);
    								}
    ;
factor2:factor3           {$$=$1;}
    |factor2 per2 factor3	{
    								 printf("periority 2 sign %c",$2);
                            $$ = operation($2,$1,$3);
    								}
			;
factor3: factor4
      | factor3 per3 factor4 {
      								printf("periority 3 sign %c",$2);
                              $$ = operation($2,$1,$3);
      								}
      ;
factor4: factor5
      | factor4 per4 factor5 {
      								printf("periority 4 sign %c",$2);
                              $$ = operation($2,$1,$3);
      								}
      ;
factor5: number        {$$=$1;}
    ;

%%
int main()
{
  printf("\nEnter size : ");
  size = 4;
  //cin.clear();
  //cin.ignore();
  operators_ =(char*)(malloc(sizeof(char)*size));
  my_priority_ =(int*)(malloc(sizeof(int)*size));
  char temp_ops[]={'+','-','/','*'};
  for(int i=0;i<size;i++)
    operators_[i]=temp_ops[i];
  printf("\nEnter prioritys : \n");
  for(int i=0;i<size;i++)
  {
    int temp;
    printf("Enter priority for %c : ",operators_[i]);
    scanf("%d",&temp);
    switch(temp)
    {
      case 1:
              my_priority_[i]=per1;
              break;
      case 2:
              my_priority_[i]=per2;
              break;
      case 3:
              my_priority_[i]=per3;
              break;
      case 4:
              my_priority_[i]=per4;
              break;
      default:
              printf("\nNo such priority available.Setting to default(per1).\n");
              my_priority_[i]=per1;
              break;
    }
  }
  printf("\nPeriorities assigned.\n");
  yyparse();
  return 0;
}
void yyerror(char *s)
{
  fprintf(stderr,"%s\n",s);
}

int operation(char c,int a,int b)
{
  switch(c)
  {
    case '+':
              return a+b;
    case '-':
              return a-b;
    case '*':
              return a*b;
    case '/':
              return a/b;
    case '%':
              return a%b;
    default:
              printf("\n%c operation not defined yet.",c);
              return 0;
  }
}
