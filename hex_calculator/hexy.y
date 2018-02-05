%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int to_decimal(char *hex_num);
void to_hex(int,char*);
%}
%token add sem AND OR NOT
%union {char *a_number;}
%start lines
%token <a_number> number
%type <a_number> line exp factor factor2

%%
lines : line | lines line;
line : exp sem {printf("result is %s\n",$1);}
      ;
exp : factor2           {$$ = $1;}
    | exp add factor2        {
    											//printf("%s $1  %s $3\n",$1,$3);
    											to_hex(to_decimal($1)+to_decimal($3),$$);}
    | exp '-' factor2        {
    												to_hex(to_decimal($1)-to_decimal($3),$$);
    											}
    											;
factor2:factor            {$$=$1;}
     |'(' exp ')'         {$$ = $2;}
    |factor2 '*' factor	{
    												to_hex(to_decimal($1)*to_decimal($3),$$);
    								}
			| factor2 '/' factor{
    												to_hex(to_decimal($1)/to_decimal($3),$$);
    								}
    	| factor2 '%' factor {
    												to_hex(to_decimal($1)%to_decimal($3),$$);
    								}
    	| factor2 AND factor {
    												to_hex(to_decimal($1)&to_decimal($3),$$);
    								}
    	| factor2 OR factor {
    												to_hex(to_decimal($1)|to_decimal($3),$$);
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

int pow_(int a,int b)
{
	int f = 1;
	while(b--)
	{
		 f *= a;
	}
	return f;
}
int to_decimal(char *hex_num)
{
	int num = 0;
	int p = 0;
	//printf("\nconverting %s \n",hex_num);
	for(int i=strlen(hex_num)-1;i>=0;i--,p++)
	{
		if(isdigit(hex_num[i]))
		{
			num += (hex_num[i]-'0')*pow_(16,p);
		}
		else
		{
			num += (hex_num[i]-'a'+10)*pow_(16,p);
		}
	}
	//printf("%d converted\n",num);
	return num;
}

void to_hex(int num,char* num_hex)
{
	//printf("\nconverting to hex : %d\n",num);
	char hex_num[100];
	int i=0;
	while(num)
	{
		int rem = num%16;
		num /= 16;
		if(rem<10)
		{
			hex_num[i]=rem+48;
		}
		else
		{
			hex_num[i] = rem-10 + 'a';
		}
		i++;
	}
	hex_num[i]='\0';
	// reverse
	char final_hex[100];
	for(int i=0;i<strlen(hex_num);i++)
	{
		final_hex[strlen(hex_num)-1-i] = hex_num[i];
	}
	final_hex[strlen(hex_num)]='\0';
	//printf("\ndecimal to hex : %s\n",final_hex);
	strcpy(num_hex,final_hex);
}








