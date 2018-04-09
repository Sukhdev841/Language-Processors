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
	string type;
	int width;
	string place;
	data(){}
	data(string val) { this->val = val;}
};

stack<data> trace;
stack<data> special_stack1;
stack<data> special_stack2;

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

void operator_code_generator(string oper)
{
	vector<data> obj = get_top_n(2);
	vector<string> temps = get_temp(1);
	vector<string> code_parts;

	data obj1;
	obj1.place = temps[0];
	code_parts.push_back(obj[0].code);
	code_parts.push_back(obj[1].code);
	code_parts.push_back(temps[0] + " := " + obj[0].place + " "+ oper + " " + obj[1].place);
	obj1.code = merge_code(code_parts);
	trace.push(obj1);
}

%}

%union
{
	char *val;
}
%start start_
%token loop_while keyword_if keyword_else tok_int tok_float tok_double tok_bool tok_switch_stmt tok_case tok_default
%token tok_colon tok_break
%token <val> var tok_num binary_oper_per1 binary_oper_per2

%left tok_l_or
%left tok_l_and
%left tok_b_or
%left tok_b_xor
%left tok_b_and
%left tok_equal_to tok_not_equal_to
%left tok_less_than tok_less_eql tok_greater_than tok_greater_eql
%left tok_plus tok_minus
%left tok_mul tok_div tok_mod
%right tok_b_not tok_l_not

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
		| switch_stmt 	{/* no change */}
		| while_loop 	{ /* no change */}
		| if_stmt		{ /* no change */}
		| expr ';' 		{ /* no change */ }
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

switch_stmt : tok_switch_stmt '(' expr ')' '{' switch_block '}'
				{
					vector<data> obj = get_top_n(1);
					vector<string> code_parts;
					vector<string> lables = get_label(special_stack1.size()+1);

					code_parts.push_back(obj[0].code);
					int i = 0;
					while(!special_stack1.empty())
					{
						code_parts.push_back("if "+ special_stack1.top().place +" == " + obj[0].place +" goto "+lables[i]);
						i++;
						special_stack1.pop();
					}
					code_parts.push_back("goto "+lables[lables.size()-1]);
					i = 0;
					while(!special_stack2.empty())
					{
						code_parts.push_back(lables[i]+":");
						code_parts.push_back(special_stack2.top().code);
						code_parts.push_back("goto "+lables[lables.size()-1]);
						special_stack2.pop();
						i++;
					}
					code_parts.push_back(lables[lables.size()-1]+":");
					data obj3;
					obj3.code = merge_code(code_parts);
					trace.push(obj3);
				}
				;

switch_block : case_stmt	{/* nothing */}
			|
			switch_block case_stmt	{/* nothing */}
			;
			
case_stmt : tok_case variable tok_colon stmts tok_break ';'
			{
				vector<data> obj = get_top_n(2);
				special_stack1.push(obj[0]);
				special_stack2.push(obj[1]);
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
decl: data_type variable ';' {
							vector<data> obj = get_top_n(2);
							if(vars[obj[1].place]=="")
							{
								vars[obj[1].place] = obj[0].type;
								string x = "declare "+obj[1].place+"["+obj[0].type+"]\n";
								data obj1;
								obj1.code = x;
								obj1.place = obj[1].place;
								trace.push(obj1);
							}
							else
							{
								cout<<"\nError Redeclartion of variable : "<<obj[1].place<<endl;
								exit(0);
							}
						 }
		;

data_type: tok_int 
					{
						data obj;
						obj.type = "int";
						obj.width = 4;
						trace.push(obj);
					}
			|
			tok_float 
					{
						data obj;
						obj.type = "float";
						obj.width = 4;
						trace.push(obj);
					}
			| tok_double
					{
						data obj;
						obj.type = "double";
						obj.width = 8;
						trace.push(obj);
					}
			| tok_bool
					{
						data obj;
						obj.type = "bool";
						obj.width = 1;
						trace.push(obj);
					}
			;

block : '{' stmts '}'	{ /* no change */ }
		|	stmt 		{ /* no change */ }
		;

expr : 
		'(' expr ')'	{ /* no change */}
		| expr tok_l_or expr {  operator_code_generator("||"); }
		| expr tok_l_and expr {  operator_code_generator("&&"); }
		| expr tok_b_or expr {  operator_code_generator("|"); }
		| expr tok_b_xor expr {  operator_code_generator("^"); }
		| expr tok_b_and expr {  operator_code_generator("&"); }
		| expr tok_equal_to expr {  operator_code_generator("=="); }
		| expr tok_not_equal_to expr {  operator_code_generator("!="); }
		| expr tok_less_than expr {  operator_code_generator("<"); }
		| expr tok_less_eql expr {  operator_code_generator("<="); }
		| expr tok_greater_than expr {  operator_code_generator(">"); }
		| expr tok_greater_eql expr {  operator_code_generator(">="); }
		| expr tok_plus expr {  operator_code_generator("+"); }
		| expr tok_minus expr {  operator_code_generator("-"); }
		| expr tok_mul expr {  operator_code_generator("*"); }
		| expr tok_div expr {  operator_code_generator("/"); }
		| expr tok_mod expr {  operator_code_generator("%"); }
		| tok_l_not
		{
			vector<data> obj = get_top_n(1);
			vector<string> temps = get_temp(1);
			vector<string> code_parts;

			data obj1;
			obj1.place = temps[0];
			code_parts.push_back(obj[0].code);
			code_parts.push_back(temps[0] + " := ! " + obj[0].place);
			obj1.code = merge_code(code_parts);
			trace.push(obj1);
		}
		| tok_b_not expr
		{
			vector<data> obj = get_top_n(1);
			vector<string> temps = get_temp(1);
			vector<string> code_parts;

			data obj1;
			obj1.place = temps[0];
			code_parts.push_back(obj[0].code);
			code_parts.push_back(temps[0] + " := ~ " + obj[0].place);
			obj1.code = merge_code(code_parts);
			trace.push(obj1);	
		}
		| tok_minus expr %prec tok_mul 
		{
			vector<data> obj = get_top_n(1);
			vector<string> temps = get_temp(1);
			vector<string> code_parts;

			data obj1;
			obj1.place = temps[0];
			code_parts.push_back(obj[0].code);
			code_parts.push_back(temps[0] + " := 0 - " + obj[0].place);
			obj1.code = merge_code(code_parts);
			trace.push(obj1);
		}
		| variable { /* no change */ }
		;

variable : var 			{
							data obj;
							obj.place = string($1);
							trace.push(obj); 
						}
			|
			tok_num		
						{
							data obj;
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

