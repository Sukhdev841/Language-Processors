#include "../../header-files/s_context_free_grammer.h"

int main()
{
  cout<<"\nEnter grammer:\n";
  int n;
  cin>>n;
  cin.clear();
  cin.ignore();
  s_context_free_grammer grammer;
  for(int i=0;i<n;i++)
  {
    string str;
    getline(cin,str);
    grammer.add_production(str);
  }
  grammer.print();
  s_context_free_grammer g2 = grammer.remove_left_recursion();
  g2.print();

  return 0;
}
