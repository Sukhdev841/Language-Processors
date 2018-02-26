#include "../../header-files/s_context_free_grammer.h"

int main()
{
  // cout<<"\nEnter grammer:\n";
  // int n;
  // cin>>n;
  // cin.clear();
  // cin.ignore();
  // s_context_free_grammer grammer;
  // for(int i=0;i<n;i++)
  // {
  //   string str;
  //   getline(cin,str);
  //   grammer.add_production(str);
  // }
  // grammer.print();
  string str1,str2;
  getline(cin,str1);
  //getline(cin,str2);
  s_production a(str1);
  //s_production b(str2);
  cout<<endl;
  a.print();
  cout<<endl;
  // b.print();
  // cout<<endl;
  vector<s_production> v  = a.left_factor();
  for(int i=0;i<v.size();i++)
  {
    cout<<"\n";
    v[i].print();
  }
  cout<<endl;
  return 0;
}
