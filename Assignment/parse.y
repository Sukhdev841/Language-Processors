%{
#include <bits/stdc++.h>	
using namespace std;

int yyparse(void);
int yylex(void); 
void yyerror(const char*);

string merge_code(vector<string>);
//vector<data> get_top_n(int);
vector<string> get_label(int);
vector<string> get_temp(int);
string to_string_(int);

int lable_count = 0;
int temp_var_count = 0;
map<string,string> vars;

struct data
{
	string code;
	string val;
	string id;
	string place;
};

stack<data> trace;

vector<data> get_top_n(int n)
{
	vector<data> top_n(n);
	for(int i=0;i<n;i++)
	{
		top_n[n-i-1] = trace.top();
		trace.pop();
	}
	return top_n;
}

%}

%union
{
	char *val;
}
%start start_
%token loop_while keyword_if keyword_else
%token <val> var data_type binary_oper_per1 binary_oper_per2

%nonassoc IFX
%nonassoc keyword_else

%%
start_ :
			stmts {
					cout<<trace.top().code<<endl;
					trace.pop();
				  }
			;
stmts : stmt { /* no change */}
		 | stmts stmt 
	 				{
	 					vector<data> obj = get_top_n(2);
	 					data obj1;
	 					obj1.code = obj[0].code + obj[1].code;
	 					trace.push(obj1);
	 				}
		 ;

stmt :    decl 			{ /* no change */} 	
		| while_loop 	{ /* no change */}
		| if_stmt		{ /* no change */}
		| expr ';' {
					// data obj1 = get_top_n(1)[0];
					// obj1.code += "\n";
					// trace.push(obj1);
				   }
		| var '=' expr ';' 
							{
								vector<data> obj = get_top_n(1);
								vector<string> code_parts;

								data obj1;
								obj1.place = string($1);

								code_parts.push_back(obj[0].code);
								code_parts.push_back(obj1.place + " := " + obj[0].place);

								obj1.code = merge_code(code_parts);
								trace.push(obj1);
							}
		;

while_loop: loop_while '(' expr ')' block
											  {
											  	vector<data> obj = get_top_n(2);
											  	vector<string> lables = get_label(2);

											  	data obj3;
											  	vector<string> code_parts;
											  	code_parts.push_back(lables[0]+":");
											  	code_parts.push_back(obj[0].code);
											  	code_parts.push_back("if "+obj[0].place+" == 0 goto "+lables[1]);
											  	code_parts.push_back(obj[1].code);
											  	code_parts.push_back("goto "+lables[0]);
											  	code_parts.push_back(lables[1]+":");

											  	obj3.code = merge_code(code_parts);

												trace.push(obj3);
											   }
			;

if_stmt : keyword_if '(' expr ')' block keyword_else block
									{
										vector<data> obj = get_top_n(3);
										vector<string> lables = get_label(2);
										vector<string> code_parts;
										code_parts.push_back(obj[0].code);
										code_parts.push_back("if "+obj[0].place + " == 0 goto "+lables[0]);
										code_parts.push_back("goto "+lables[1]);
										code_parts.push_back(obj[1].code);
										code_parts.push_back("goto "+lables[1]);
										code_parts.push_back(lables[0]+":");
										code_parts.push_back(obj[2].code);
										code_parts.push_back(lables[1]+":");

										data obj4;
										obj4.code = merge_code(code_parts);
										trace.push(obj4);
									}
			|keyword_if '(' expr ')' block %prec IFX
									{
										vector<data> obj = get_top_n(2);
										vector<string> lables = get_label(1);
										vector<string> code_parts;

										code_parts.push_back(obj[0].code);
										code_parts.push_back("if "+obj[0].place+" == 0 goto "+lables[0]);
										code_parts.push_back(obj[1].code);
										code_parts.push_back(lables[0]+":");
										
										data obj3;
										obj3.code = merge_code(code_parts);
										trace.push(obj3);
									}
									;
decl: data_type var ';' {
							string var_name($2);
							string var_type($1);
							if(vars[var_name]=="")
							{
								vars[var_name] = var_type;
								string x = "declare "+var_name+"["+var_type+"]\n";
								data obj;
								obj.code = x;
								obj.place = var_name;
								trace.push(obj);
							}
							else
								cout<<"\nError Redeclartion of variable : "<<$2<<endl;
						 }
		;

block : '{' stmts '}'	{ /* no change */ }
		|	stmt 		{ /* no change */ }
		;

expr : 
		'(' expr ')'	{ /* no change */}

		| ar_expr {/* no change */ }
		| bin_expr { /* no change */ }
		;

ar_expr : ar_expr1		{ /* not change */ }
		| ar_expr binary_oper_per1 ar_expr1 
							{
								vector<data> obj = get_top_n(2);
								vector<string> temps = get_temp(1);
								vector<string> code_parts;

								data obj1;
								obj1.place = temps[0];
								code_parts.push_back(obj[0].code);
								code_parts.push_back(obj[1].code);
								code_parts.push_back(temps[0] + " := " + obj[0].place + string($2) + obj[1].place);
								obj1.code = merge_code(code_parts);
								trace.push(obj1);
							}
			;

ar_expr1 : ar_expr2		{/*no change */}
			| ar_expr1 binary_oper_per2 variable
						{
							vector<data> obj = get_top_n(2);
							vector<string> temps = get_temp(1);
							vector<string> code_parts;

							data obj1;
							obj1.place = temps[0];
							code_parts.push_back(obj[0].code);
							code_parts.push_back(obj[1].code);
							code_parts.push_back(temps[0] + " := " + obj[0].place + string($2) + obj[1].place);
							obj1.code = merge_code(code_parts);
							trace.push(obj1);
						}
			;

bin_expr : 


variable : var 			{
							data obj;
							//obj.code = string();
							obj.place = string($1);
							trace.push(obj); 
						}
			;

%%
int main()
{
	yyparse();
	return 0;
}

string merge_code(vector<string> codes)
{
	string merged_code = "";
	for(int i=0;i<codes.size();i++)
	{
		if(codes[i] != "" && codes[i] != " ")
			merged_code += codes[i] + "\n";
	}
	return merged_code;
}

vector<string> get_label(int count)
{
	vector<string> new_lables;
	for(int i=0;i<count;i++,lable_count++)
	{
		new_lables.push_back("L"+to_string_(lable_count));
	}
	return new_lables;
}
vector<string> get_temp(int count)
{
	vector<string> new_temp_vars;
	for(int i=0;i<count;i++,temp_var_count++)
	{
		new_temp_vars.push_back("t"+to_string_(temp_var_count));
	}
	return new_temp_vars;
}
string to_string_(int num)
{
	char temp[100];
	sprintf(temp,"%d",num);
	return string(temp);
}

