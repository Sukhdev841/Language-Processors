%{
#include <bits/stdc++.h>

using namespace std;

#define stmt_type 1
#define var_type 0
#define int_type 2
#define u_int_type 3
#define bool_type 4
#define numeric_constant 5
#define bool_type_exp 6
#define arith_type_exp 7

int yyparse(void);
int yylex(void); 
void yyerror(const char*);

string merge_code(vector<string>);
vector<string> get_label(int);
vector<string> get_temp(int);
string to_string_(int);

int lable_count = 0;
int temp_var_count = 0;

struct true_false
{
	string true_lable;
	string false_lable;
};

struct data
{
	string code;
	string val;
	string id;
	string type;
	int width;
	int type2;
	int type3;
	int exp_type;
	string place;
	true_false jumps;
	data(){}
	data(string val) { this->val = val;}
};


struct symbol_talbe_row
{
	string type;
	string val;
	int type_id;
};

map<string,symbol_talbe_row> symbol_table;

stack<data> trace;
stack< queue<data> > special_stack1;
stack< queue<data> > special_stack2;

bool search(string var_name)
{
	if(symbol_table[var_name].type == "" || symbol_table[var_name].type == " " )
		return false;
	return true;
}

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

	if(obj[0].exp_type == bool_type_exp || obj[1].exp_type == bool_type_exp )
	{
		cout<<"\nErro operator "+oper+" on [boolean] not permitted\n";
		exit(0);
	}

	data obj1;
	obj1.place = temps[0];
	code_parts.push_back(obj[0].code);
	code_parts.push_back(obj[1].code);
	code_parts.push_back(temps[0] + " := " + obj[0].place + " "+ oper + " " + obj[1].place);
	obj1.code = merge_code(code_parts);
	obj1.type2 = stmt_type;
	obj1.exp_type = arith_type_exp;
	trace.push(obj1);
}

void and_code_generator()
{
	vector<data> obj = get_top_n(2);
	vector<string> lables = get_label(1);
	vector<string> code_parts;

	if(obj[0].exp_type != bool_type_exp || obj[1].exp_type != bool_type_exp )
	{
		cout<<"\nErro operator && only on [boolean] permitted.\n";
		exit(0);
	}

	code_parts.push_back(obj[0].code);
	code_parts.push_back(obj[0].jumps.true_lable+":");
	code_parts.push_back(obj[1].code);
	code_parts.push_back(obj[0].jumps.false_lable+":");
	code_parts.push_back(obj[1].jumps.false_lable+":");
	code_parts.push_back("goto "+lables[0]);

	data obj1;
	obj1.code = merge_code(code_parts);
	obj1.type2 = stmt_type;
	obj1.jumps.true_lable = obj[1].jumps.true_lable;
	obj1.jumps.false_lable = lables[0];
	obj1.exp_type = bool_type_exp;
	trace.push(obj1);

}

void or_code_generator()
{
	vector<data> obj = get_top_n(2);
	vector<string> lables = get_label(1);
	vector<string> code_parts;

	if(obj[0].exp_type == bool_type_exp || obj[1].exp_type == bool_type_exp )
	{
		cout<<"\nErro operator || only on [boolean] permitted.\n";
		exit(0);
	}

	code_parts.push_back(obj[0].code);
	code_parts.push_back(obj[0].jumps.false_lable+":");
	code_parts.push_back(obj[1].code);
	code_parts.push_back(obj[0].jumps.true_lable+":");
	code_parts.push_back(obj[1].jumps.true_lable+":");
	code_parts.push_back("goto "+lables[0]);

	data obj1;
	obj1.code = merge_code(code_parts);
	obj1.type2 = stmt_type;
	obj1.jumps.true_lable = lables[0];
	obj1.jumps.false_lable = obj[1].jumps.false_lable;
	obj1.exp_type = bool_type_exp;
	trace.push(obj1);	
}

void rel_operator_code_generator(string oper)
{
	vector<data> obj = get_top_n(2);
	vector<string> code_parts;
	vector<string> lables = get_label(2);

	if( oper != "==" && oper != "!=")
	{
		if(obj[0].exp_type == bool_type_exp || obj[1].exp_type == bool_type_exp)
		{
			cout<<"\nErro operator "+oper+" on [boolean] not permitted\n";
			exit(0);
		}
	}
	if(oper == "==" || oper == "!=")
	{
		if(obj[0].exp_type == bool_type_exp && obj[1].exp_type != bool_type_exp)
		{
			cout<<"\nErro operator "+oper+" on [boolean] vs [other] not permitted. Both should be boolean.\n";
			exit(0);
		}
		else if(obj[0].exp_type != bool_type_exp && obj[1].exp_type == bool_type_exp)
		{
			cout<<"\nErro operator "+oper+" on [boolean] vs [other] not permitted. both should be boolean;\n";
			exit(0);
		}
	}

	if(obj[0].type2 != var_type)
			code_parts.push_back(obj[0].code);
	if(obj[1].type2 != var_type)
			code_parts.push_back(obj[1].code);

	code_parts.push_back("if "+obj[0].place + " " + oper + " " + obj[1].place + " goto " + lables[0]);
	code_parts.push_back("goto "+lables[1]);
	data obj1;
	obj1.code = merge_code(code_parts);
	obj1.type2 = stmt_type;
	obj1.jumps.true_lable = lables[0];
	obj1.jumps.false_lable = lables[1];
	obj1.exp_type = bool_type_exp;
	trace.push(obj1);
}

%}

%union
{
	char *val;
}
%start start_
%token loop_while keyword_if keyword_else tok_int tok_u_int tok_bool tok_switch_stmt tok_case tok_default
%token tok_colon tok_break
%token <val> var tok_num binary_oper_per1 binary_oper_per2 bool_init

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
		| variable '=' expr ';' 
							{
								vector<data> obj = get_top_n(2);
								if( !search(obj[0].place))
								{
									cout<<"\nError "<<obj[0].place<<" is used without declaration\n";
									exit(0);
								}
								//cout<<obj[0].exp_type<<" "<<obj[1].exp_type<<endl;
								if(obj[0].exp_type != obj[1].exp_type)
								{
									cout<<"\nInvalid assignment\n";
									exit(0);
								}

								vector<string> code_parts;

								data obj1;

								if( obj[0].exp_type == bool_type_exp)
								{
									cout<<"\nExpression is bool type\n";
									vector<string> lables = get_label(1);
									code_parts.push_back(obj[1].code);
									code_parts.push_back(obj[1].jumps.true_lable+":");
									code_parts.push_back(obj[0].place+" := 1");
									code_parts.push_back("goto "+lables[0]);
									code_parts.push_back(obj[1].jumps.false_lable+":");
									code_parts.push_back(obj[0].place+" := 0");
									code_parts.push_back(lables[0]+":");
								}
								else
								{
									code_parts.push_back(obj[1].code);
									code_parts.push_back(obj1.place + " := " + obj[1].place);
								}

								
								obj1.place = string(obj[0].place);
								obj1.code = merge_code(code_parts);
								trace.push(obj1);
							}
		;

switch_stmt : switch_key '(' expr ')' '{' switch_block '}'
				{
					vector<data> obj = get_top_n(1);
					vector<string> code_parts;
					vector<string> lables = get_label(special_stack1.top().size()+1);

					queue<data> q1, q2;
					q1 = special_stack1.top();
					q2 = special_stack2.top();

					special_stack1.pop();
					special_stack2.pop();

					code_parts.push_back(obj[0].code);
					int i = 0;
					while(!q1.empty())
					{
						code_parts.push_back("if "+ obj[0].place +" == " + q1.front().place +" goto "+lables[i]);
						i++;
						q1.pop();
					}
					code_parts.push_back("goto "+lables[lables.size()-1]);
					i = 0;
					while(!q2.empty())
					{
						code_parts.push_back(lables[i]+":");
						code_parts.push_back(q2.front().code);
						code_parts.push_back("goto "+lables[lables.size()-1]);
						q2.pop();
						i++;
					}
					code_parts.push_back(lables[lables.size()-1]+":");
					data obj3;
					obj3.code = merge_code(code_parts);
					trace.push(obj3);
				}
				;
switch_key : tok_switch_stmt
							{
								queue<data> q1,q2;
								special_stack2.push(q2);
								special_stack1.push(q1);
							}
			; 

switch_block : case_stmt	{/* nothing */}
			|
			switch_block case_stmt	{/* nothing */}
			;
			
case_stmt : tok_case variable tok_colon stmts tok_break ';'
			{
				vector<data> obj = get_top_n(2);
				queue<data> q1, q2;
				q1 = special_stack1.top();
				q2 = special_stack2.top();
				special_stack1.pop();
				special_stack2.pop();
				q1.push(obj[0]);
				q2.push(obj[1]);
				special_stack1.push(q1);
				special_stack2.push(q2);
			}
			;

while_loop: loop_while '(' expr ')' block
											  {
											  	vector<data> obj = get_top_n(2);
											  	vector<string> lables = get_label(1);

											  	data obj3;
											  	vector<string> code_parts;
											  	code_parts.push_back(lables[0]+":");
											  	code_parts.push_back(obj[0].code);
											  	code_parts.push_back(obj[0].jumps.true_lable+":");
											  	code_parts.push_back(obj[1].code);
											  	code_parts.push_back("goto "+ lables[0]);
											  	code_parts.push_back(obj[0].jumps.false_lable+":");

											  	obj3.code = merge_code(code_parts);

												trace.push(obj3);
											   }
			;

if_stmt : keyword_if '(' expr ')' block keyword_else block
									{
										vector<data> obj = get_top_n(3);
										vector<string> lables = get_label(1);
										vector<string> code_parts;

										code_parts.push_back(obj[0].code);

										code_parts.push_back(obj[0].jumps.true_lable+":");
										code_parts.push_back(obj[1].code);
										code_parts.push_back("goto "+lables[0]);

										code_parts.push_back(obj[0].jumps.false_lable+":");
										code_parts.push_back(obj[2].code);

										code_parts.push_back(lables[0]+":");

										data obj4;
										obj4.code = merge_code(code_parts);
										trace.push(obj4);
									}
			|keyword_if '(' expr ')' block %prec IFX
									{
										vector<data> obj = get_top_n(2);
										vector<string> code_parts;

										code_parts.push_back(obj[0].code);
										code_parts.push_back(obj[0].jumps.true_lable+":");
										code_parts.push_back(obj[1].code);
										code_parts.push_back(obj[0].jumps.false_lable+":");
										data obj3;
										obj3.code = merge_code(code_parts);
										trace.push(obj3);
									}
									;
decl: data_type variable ';' {
							vector<data> obj = get_top_n(2);
							if(!search(obj[1].place))
							{
								symbol_talbe_row row;
								row.type = obj[0].type;
								row.val = "";
								row.type_id = obj[0].type2;
								symbol_table[obj[1].place] = row;

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
		|
		data_type variable '=' variable ';'
						{
							vector<data> obj = get_top_n(3);
							if(!search(obj[1].place) && (search(obj[2].place) || obj[2].type3 == numeric_constant) )
							{
								symbol_talbe_row row;
								row.type = obj[0].type;
								row.val = obj[2].place;
								row.type_id = obj[0].type2;
								symbol_table[obj[1].place] = row;

								string x = "declare "+obj[1].place+"["+obj[0].type+"] = "+row.val+"\n";
								data obj1;
								obj1.code = x;
								obj1.place = obj[1].place;
								trace.push(obj1);
							}
							else
							{
								cout<<"\nError in declartion of variable : "<<obj[1].place<<endl;
								exit(0);
							}
						}
		;

data_type: tok_int 
					{
						data obj;
						obj.type = "int";
						obj.type2 = int_type;
						obj.width = 4;
						trace.push(obj);
					}
			| tok_u_int
					{
						data obj;
						obj.type = "unsingned_int";
						obj.type2 = u_int_type;
						obj.width = 4;
						trace.push(obj);
					}
			| tok_bool
					{
						data obj;
						obj.type = "bool";
						obj.type2 = bool_type;
						obj.width = 1;
						trace.push(obj);
					}
			;

block : '{' stmts '}'	{ /* no change */ }
		|	stmt 		{ /* no change */ }
		;

expr : 
		'(' expr ')'	{ /* no change */}
		| expr tok_l_or expr {  or_code_generator(); }
		| expr tok_l_and expr {  and_code_generator(); }
		| expr tok_b_or expr {  operator_code_generator("|"); }
		| expr tok_b_xor expr {  operator_code_generator("^"); }
		| expr tok_b_and expr {  operator_code_generator("&"); }

		| expr tok_equal_to expr {  rel_operator_code_generator("=="); }
		| expr tok_not_equal_to expr { rel_operator_code_generator("!="); }
		| expr tok_less_than expr {  rel_operator_code_generator("<"); }
		| expr tok_less_eql expr {  rel_operator_code_generator("<="); }
		| expr tok_greater_than expr {  rel_operator_code_generator(">"); }
		| expr tok_greater_eql expr {  rel_operator_code_generator(">="); }

		| expr tok_plus expr {  operator_code_generator("+"); }
		| expr tok_minus expr {  operator_code_generator("-"); }
		| expr tok_mul expr {  operator_code_generator("*"); }
		| expr tok_div expr {  operator_code_generator("/"); }
		| expr tok_mod expr {  operator_code_generator("%"); }

		| tok_l_not expr
		{
			vector<data> obj = get_top_n(1);
			if(obj[0].exp_type == arith_type_exp)
			{
				cout<<"! on arithmetic not allowed\n";
				exit(0);
			}
			vector<string> temps = get_temp(1);
			vector<string> code_parts;

			data obj1;
			code_parts.push_back(obj[0].code);
			code_parts.push_back(temps[0] + " := ! " + obj[0].place);
			obj1.jumps.true_lable = obj[0].jumps.false_lable;
			obj1.jumps.false_lable = obj[0].jumps.true_lable;

			obj1.code = merge_code(code_parts);
			obj1.type2 = stmt_type;
			obj1.exp_type = bool_type_exp;
			obj1.place = temps[0];
			trace.push(obj1);
		}
		| tok_b_not expr
		{
			vector<data> obj = get_top_n(1);
			if(obj[0].exp_type == bool_type_exp)
			{
				cout<<"\n~ on boolean not permitted\n";
				exit(0);
			}
			vector<string> temps = get_temp(1);
			vector<string> code_parts;

			data obj1;
			obj1.place = temps[0];
			code_parts.push_back(obj[0].code);
			code_parts.push_back(temps[0] + " := ~ " + obj[0].place);
			obj1.code = merge_code(code_parts);
			obj1.type2 = stmt_type;
			obj1.exp_type = arith_type_exp;
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
			obj1.type2 = stmt_type;
			obj1.exp_type = arith_type_exp;
			trace.push(obj1);
		}
		| variable { /* no change */ }
		;

variable : var 			{
							data obj;
							obj.place = string($1);
							if(search(string($1)))
								switch(symbol_table[string($1)].type_id)
								{
									case int_type:
													obj.exp_type = arith_type_exp;
													break;
									case u_int_type:
													obj.exp_type = arith_type_exp;
													break;
									case bool_type:
													obj.exp_type = bool_type_exp;
													vector<string> lables = get_label(2);
													obj.jumps.true_lable = lables[0];
													obj.jumps.false_lable = lables[1];
													obj.code = "if " + obj.place +" == 1 goto "+lables[0]+"\ngoto "+lables[1]+"\n";
													break;
								}
							obj.type2 = var_type;
							trace.push(obj); 
						}
			|
			tok_num		
						{
							data obj;
							obj.place = string($1);
							obj.type2 = var_type;
							obj.type3 = numeric_constant;
							obj.exp_type = arith_type_exp;
							trace.push(obj);
						}
			| bool_init
						{
							data obj;
							obj.place = string($1);
							obj.type2 = var_type;
							obj.type3 = numeric_constant;
							obj.exp_type = bool_type_exp;
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

