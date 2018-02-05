%{
#include <bits/stdc++.h>
using namespace std;
set<string> keywords;
void check_word(char*);
%}
%%
[a-zA-Z]+[a-zA-Z0-9]* {check_word(yytext);}
[0-9]+ {printf("%s is a number.\n",yytext);}
. ;
%%
int main()
{
  keywords.insert("main");
  keywords.insert("#define");
  keywords.insert("int");
  keywords.insert("float");
  keywords.insert("double");
  keywords.insert("return");
  keywords.insert("cout");
  keywords.insert("cin");
  keywords.insert("using");
  keywords.insert("namespace");
  keywords.insert("std");
  keywords.insert("#include");
  yylex();
  return 0;
}
int yywrap()
{
return 1;
}
void check_word(char* str)
{
  string s(str);
  if(keywords.find(s) != keywords.end())
  {
    cout<<s<<" is keyword.\n";
  }
  else
  {
    cout<<s<<" is identifier.\n";
  }
}
