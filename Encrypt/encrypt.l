%{
#include<bits/stdc++.h>
using namespace std;
void word(char*);
void number(char*);
void special_word(char*);
void other_character(char*);
void my_special_word(char*);
void space();
%}
%%
"main" {special_word(yytext);}
"W"+ {space();}
[a-zA-Z]+ {word(yytext);}
[0-9]+ {number(yytext);}
. {other_character(yytext);}
%%
int main()
{
  yylex();
  return 0;
}
int yywrap()
{
  return 1;
}
void special_word(char* s)
{
    char c1,c2;
    c1 = s[strlen(s)-1];
    c2 = s[strlen(s)-2];
    for(int i=2;i<strlen(s);i++)
    {
      s[i]=s[i-2];
    }
    s[0]=c2;
    s[1]=c1;
    for(int i=strlen(s)-1;i>=0;i--)
    {
    cout<<s[i];
    }
}
void word(char* s)
{
  for(int i=strlen(s)-1;i>=0;i--)
  {
    if(s[i]=='z')
      cout<<"a";
    else if(s[i]=='Z')
      cout<<"A";
    else
    cout<<(char)(s[i]+1);
  }
}
void space()
{
cout<<" ";
}
void number(char* s)
{
  int n= strlen(s);
  for(int i=n/2+1;i<n;i++)
  {
    cout<<s[i];
  }
  cout<<s[n/2];
  for(int i=0;i<n/2;i++)
  {
    cout<<s[i];
  }
}
void other_character(char* s)
{
  cout<<s;
}
