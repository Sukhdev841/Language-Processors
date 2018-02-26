#include "s_context_free_grammer.h"

int main()
{
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

  cout<<"\nEnter a production to check: \n";
  string test_production;
  getline(cin,test_production);
  //grammer.remove_production(test_production);

  grammer.remove_productions_starting_with(test_production);
  grammer.refresh_grammer();
  grammer.print();

  return 0;
}
