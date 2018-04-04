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
%start start_
%token loop_while keyword_if keyword_else
%token <val> var data_type 
%type <val> while_loop if_stmt stmt stmts decl block start_ expr

%nonassoc IFX
%nonassoc keyword_else

%%
start_ :
			stmts {
					cout<<$$<<endl;
			}
			;
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
		|	if_stmt
					{
						$$ = (char*)(malloc(sizeof(char)*strlen($1)));
						strcpy($$,$1);
					}
		| var {
					$$ = (char*)(malloc(sizeof(char)*strlen($1)));
					strcpy($$,$1);
			  }
		|
			expr ';' {
					$$ = (char*)(malloc(sizeof(char)*(strlen($1)+1)));
					strcpy($$,$1);	
					strcat($$,"\n");
			}
		;

while_loop: loop_while '(' expr ')' block
											  {
											  	string code = "";
											  	char num[100];
											  	sprintf(num,"%d",lable_count);
											  	code += "L"+ string(num) +":\nif "+string($3)+" == 0 goto L";
												sprintf(num,"%d",lable_count+1);
												code +=string(num) +"\n" + string($5);
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

if_stmt : keyword_if '(' expr ')' block keyword_else block
									{
										string code = "";
										char L1[100],L2[100];
										sprintf(L1,"%d",lable_count);
										sprintf(L2,"%d",lable_count+1);
										lable_count += 2;
										code += "if "+string($3)+" == 0 goto L";
										code += string(L1)+"\n";
										// true
										code += string($5);
										code += "goto L"+string(L2)+"\n";
										// false
										code += "L"+string(L1)+":\n";
										code += string($7);
										code += "L"+string(L2)+":\n";

										$$ = (char*)(malloc(sizeof(char)*code.length()));
										strcpy($$,code.c_str());
									}
			|keyword_if '(' expr ')' block %prec IFX
									{
										string code = "";
										char L1[100];
										sprintf(L1,"%d",lable_count);
										lable_count += 1;
										code += "if "+string($3)+" == 0 goto L";
										code += string(L1)+"\n";
										// true
										code += string($5);
										// false
										code += "\nL"+string(L1)+":\n";
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

block : '{' stmts '}'	{
							$$ = (char*)(malloc(sizeof(char)*strlen($2)));
							strcpy($$,$2);
						}
		|
		stmt			{
							$$ = (char*)(malloc(sizeof(char)*strlen($1)));
							strcpy($$,$1);
						}
		;

expr : '(' expr ')' {
						$$ = (char*)(malloc(sizeof(char)*strlen($2)));
						strcpy($$,$2);
					}
		|
		var{
					$$ = (char*)(malloc(sizeof(char)*strlen($1)));
					strcpy($$,$1);
				} 
		;

%%
int main()
{
	yyparse();
	return 0;
}
