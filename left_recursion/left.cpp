#include <bits/stdc++.h>
using namespace std;

map<string,vector<string> > productions;
map<string,vector<string> > exit_vars;
map<string,bool> is_left_recursive;

int main()
{
  ifstream fin;
  fin.open("input.txt");
  int n;
  fin>>n;
  fin.clear();
  fin.ignore();
  for(int i=0;i<n;i++)
  {
    string str;
    getline(fin,str);
    cout<<str<<endl;
    string start_symbol ="";
    start_symbol += str[0];
    string temp = str.substr(3,str.length());
    istringstream f(temp);
    string x;
    is_left_recursive[start_symbol]=false;
    while(getline(f,x,'|'))
    {
      if(x.length() != 1)
        productions[start_symbol].push_back(x);
      else
        exit_vars[start_symbol].push_back(x);
      if(x[0]==start_symbol[0])
        is_left_recursive[start_symbol]=true;
    }
  }

  map<string,vector<string> > ::iterator it = productions.begin();
  map<string,vector<string> > new_grammer;
  while( it != productions.end())
  {
    if(is_left_recursive[it->first] == false)
    {
      new_grammer[it->first] = it->second;
      for(int i=0;i<exit_vars[it->first].size();i++)
      {
        new_grammer[it->first].push_back(exit_vars[it->first][i]);
      }
    }
    else
    {
      string left_sym = "";
      left_sym += it->first + "\'";
      for(int i=0;i<exit_vars[it->first].size();i++)
      {
        new_grammer[it->first].push_back(exit_vars[it->first][i]+left_sym);
      }
      for(int i=0;i<it->second.size();i++)
      {
        new_grammer[left_sym].push_back(it->second[i].substr(1,it->second[i].length())+left_sym);
      }
      new_grammer[left_sym].push_back("NULL");
    }
    it++;
  }
  cout<<"\nprinting new grammer\n";
  it = new_grammer.begin();
  cout<<endl;
  while(it!=new_grammer.end())
  {
    cout<<it->first<<"->";
    for(int i=0;i<it->second.size();i++)
    {
      cout<<it->second[i]<<" | ";
    }
    cout<<endl;
    it++;
  }
  return 0;
}
