#include "../../header-files/s_context_free_grammer.h"

int main()
{
  string input;
  getline(cin,input);
  s_production a(input);
  //a.print();
  //cout<<endl;
  getline(cin,input);
  s_production b(input);
  //b.print();
  //cout<<endl;
  a.replace_with(b);
  a.print();
  cout<<endl;
  return 0;
}
