#include "../../header-files/s_context_free_grammer.h"

int main()
{
	int n;
  s_context_free_grammer grammer;
  string temp;
  cin>>n;
    cin.clear();
  cin.ignore();
  for(int i=0;i<n;i++)
  {
    getline(cin,temp);
    grammer.add_production(temp);
  }

  // grammer.print();
  // grammer.generate_all_firsts();
  // vector<set<string> > firsts = grammer.all_firsts;
  // cout<<endl;
  // set<string> variables = grammer.variables;
  // set<string>::iterator it = variables.begin();

  // for(int i=0;i<firsts.size();i++,it++)
  // {
  //   cout<<"\nFirst for variable "<<*it<<endl;
  //   set<string>::iterator it2 = firsts[i].begin();
  //   while(it2 != firsts[i].end() )
  //   {
  //     cout<<*it2<<" ";
  //     it2++;
  //   }
  //   cout<<endl;
  // }
  // cout<<endl;

  // string input;
  // getline(cin,input);
  // s_production p(input);
  // p.print();
  // vector<s_production> vec = p.left_factor();
  // s_context_free_grammer g2;
  // g2.add_productions(vec);
  // g2.print();
  
	// string input;
 //  getline(cin,input);
 //  s_production p2(input);
 //  cout<<"\np2 = \n";
 //  p2.print();
 //  vector<s_production> vec2 = p2.remove_immediate_left_recursion();
 //  s_context_free_grammer g3;
 //  g3.add_productions(vec2);
 //  g3.print();

  s_context_free_grammer g3 = grammer.remove_left_recursion();
  g3.print();
  

}