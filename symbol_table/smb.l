%{
#include <bits/stdc++.h>
using namespace std;
void word(char*);
void print();
struct data
{
	int count;
	vector<int> line_numbers;
	data()
	{
		count = 0;
	}
};
map<string,data> symbol_table;
int line_number = 1;
%}
%%
"\n" {line_number++;}
[a-zA-Z]+"'"[a-zA-Z]+ {word(yytext);}
[a-zA-Z]+ {word(yytext);}
%%
int main()
{
	yylex();
	print();
	return 0;
}
int yywrap()
{
	return 1;
}
void print()
{
	cout<<endl;
	map<string,data>::iterator it = symbol_table.begin();
	while(it!=symbol_table.end())
	{
		cout<<it->first<<" | ";
		cout<<it->second.count<<" | ";
		for(int i=0;i<it->second.line_numbers.size();i++)
		{
			cout<<it->second.line_numbers[i]<<" ";
		}
		cout<<endl;
		it++;
	}
}
void word(char* s)
{
	string str(s);
	data temp_data = symbol_table[str];
	temp_data.count++;
	vector<int>::iterator it = find(temp_data.line_numbers.begin(),temp_data.line_numbers.end(),line_number);
	if(it==temp_data.line_numbers.end())
	temp_data.line_numbers.push_back(line_number);
	symbol_table[str] = temp_data;
}
