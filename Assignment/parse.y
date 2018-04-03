%{
#include <bits/stdc++.h>	
using namespace std;
int yyparse(void);
int yylex(void); 
void yyerror(const char*);
int lable_count = 0;
map<string,string> vars;
%}
%union
{
	char *val;
}
%start stmts
%token loop_while
%token <val> var data_type 
%type <val> while_loop stmt stmts decl if_stmt

%%
stmts : stmt 
				{
				$$ = (char*)(malloc(sizeof(char)*strlen($1)));
				strcpy($$,$1);
				}
		 | stmts stmt 
	 				{
	 					string code1($1),code2($2);
	 					code1 += code2+"\n";
	 					$$ = (char*)(malloc(sizeof(char)*(code1.length())));
	 					strcpy($$,code1.c_str());
	 					cout<<$$;
	 					strcpy($$,"\0");
	 				}
		 ;

stmt :  decl 	{
					$$ = (char*)(malloc(sizeof(char)*strlen($1)));
					strcpy($$,$1);
				}
		| while_loop 
					  {
					  	$$ = (char*)(malloc(sizeof(char)*strlen($1)));
						strcpy($$,$1);
					  }
		| var {
					$$ = (char*)(malloc(sizeof(char)*strlen($1)));
					strcpy($$,$1);
			  }
		| block 
			{
					$$ = (char*)(malloc(sizeof(char)*strlen($1)));
					strcpy($$,$1);
			  }
		;

while_loop: loop_while '(' stmt ')' '{' stmts '}'
											  {
											  	string code = "";
											  	char num[100];
											  	sprintf(num,"%d",lable_count);
											  	code += "L"+ string(num) +":\nif "+string($3)+" <=0 goto L";
												sprintf(num,"%d",lable_count+1);
												code +=string(num) +"\n" + string($6) + "\n";
												sprintf(num,"%d",lable_count);
												code += "goto L" + string(num) + "\n";
												lable_count++;
												sprintf(num,"%d",lable_count);
												code += "L"+string(num)+":\n";
												lable_count++;
												$$ = (char*)(malloc(sizeof(char)*code.length()));
												strcpy($$,code.c_str());
											   }
			;

decl: data_type var ';' {
							string var_name($2);
							string var_type($1);
							if(vars[var_name]=="")
							{
								vars[var_name] = var_type;
								string x = "declare "+var_name+"["+var_type+"]\n";
								$$ = (char*)(malloc(sizeof(char)*x.length()));
								strcpy($$,x.c_str());
							}
							else
								cout<<"\nError Redeclartion of variable : "<<$2<<endl;
						 }
		;

if_stmt :
			if_ '(' stmt ')' '{' stmts '}'
			|
			if_ '(' stmt ')' '{' stmts '}' else 

		;

%%
int main()
{
	yyparse();
	return 0;
}
