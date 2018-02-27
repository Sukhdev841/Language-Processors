#include "../../header-files/s_context_free_grammer.h"

vector<set<string> > follow(s_context_free_grammer grammer)
{
  vector<set<string> > result1, result2;
  vector<set<string> > all_firsts = grammer.first();
  set<string> variables = grammer.variables;
  cout<<"\nLoop1\n";
  cout<<"\nLoop1\n";
  for(set<string>::iterator it = variables.begin() ; it != variables.end(); it++)
  {
    set<string> temp;
    if(it == variables.begin())
        temp.insert("$");
    for(int i=0;i<grammer.productions.size();i++)
    {
      vector<string> parts = grammer.productions[i].right_parts;
      for(int pp=0;pp<parts.size();pp++)
      {
        vector<string> parts2 = get_parts(parts[pp],' ');
        int index = -1;
        for(index=0;index<parts2.size();index++)
        {
          if(parts2[index] == *it)
            break;
        }
      
      cout<<"\n*it = "<<*it<<endl;
      //int index = find(parts.begin(),parts.end(),(*it)) - parts.begin();
      cout<<"\nindex = "<<index<<" and productoin parts = "<<parts.size()<<endl;
      grammer.productions[i].print();
      if( index != parts2.size() && index+1 != parts2.size())
      {
        // there is something right to the variable
        if( grammer.is_a_terminal(parts2[index+1]))
        {
          temp.insert(parts2[index+1]);
        }
        else
        {
          //not terminal
          string var_to_right = parts2[index+1];
          int var_to_right_index = 0;//grammer.variables.begin() - grammer.variables.find(var_to_left);
          set<string>::iterator xxx = grammer.variables.begin();
          while( xxx != grammer.variables.end())
          {
            if(*xxx == var_to_right)
            {
              break;
            }
            xxx++;
            var_to_right_index++;
          }
          set<string> first_of_right = all_firsts[var_to_right_index];
          set<string>::iterator set_itr = first_of_right.begin();
          while(set_itr != first_of_right.end())
          {
            temp.insert((*set_itr));
            set_itr++;
          }
        }
      }
    }
    }
    result1.push_back(temp);
  }

  cout<<"\nAFter first loop\n";
   cout<<"\nAFter first loop\n";
   //set<string>::iterator it = variables.begin();

  for(int i=0;i<result1.size();i++)
  {
    //cout<<"\nFollow for variable  loop 1 "<<*it<<endl;
    set<string>::iterator it2 = result1[i].begin();
    while(it2 != result1[i].end() )
    {
      cout<<*it2<<" ";
      it2++;
    }
    cout<<endl;
  }
  cout<<endl;

  for(set<string>::iterator it = variables.begin() ; it != variables.end(); it++)
  {
    set<string> temp;
    for(int i=0;i<grammer.productions.size();i++)
    {
      vector<string> parts = grammer.productions[i].right_parts;
      for(int pp=0;pp<parts.size();pp++)
      {
        vector<string> parts2 = get_parts(parts[pp],' ');
        int index = -1;
        for(index=0;index<parts2.size();index++)
        {
          if(parts2[index] == *it)
            break;
        }
      //int index = parts.begin() - find(parts.begin(),parts.end(),(*it));
      if( index != parts2.size() && index+1 != parts2.size())
      {
        // there is something right to the variable
        if( grammer.is_a_terminal(parts2[index+1]))
        {
          temp.insert(parts2[index+1]);
        }
        else
        {
          //not terminal
          string var_to_left = grammer.productions[i].left_side;
          int var_to_left_index = 0;//grammer.variables.begin() - grammer.variables.find(var_to_left);
          set<string>::iterator xxx = grammer.variables.begin();
          while( xxx != grammer.variables.end())
          {
            if(*xxx == var_to_left)
            {
              break;
            }
            xxx++;
            var_to_left_index++;
          }
          // set<string>::iterator set_itr = grammer.variables.begin();
          // advance(set_itr,var_to_left_index);
          set<string> follow_of_left = result1[var_to_left_index];
          // now put first in temp
          set<string>::iterator set_itr = follow_of_left.begin();
          while(set_itr != follow_of_left.end())
          {
            temp.insert((*set_itr));
            set_itr++;
          }
        }
      }
      }
    }
    result2.push_back(temp);
  }

  cout<<"\nAfter result2\n";
  for(int i=0;i<result2.size();i++)
  {
    //cout<<"\nFollow for variable  loop 1 "<<*it<<endl;
    set<string>::iterator it2 = result2[i].begin();
    while(it2 != result2[i].end() )
    {
      cout<<*it2<<" ";
      it2++;
    }
    cout<<endl;
  }
  cout<<endl;

  // add result2 to reslut1
  for(int i=0;i<result1.size();i++)
  {
    set<string>::iterator it = result2[i].begin();
    while(it!=result2[i].end())
    {
      result1[i].insert(*it);
      it++;
    }
  }
  return result1;
}

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

  grammer.print();


  grammer.generate_all_firsts();
  vector<set<string> > firsts = follow(grammer);
  cout<<endl;
  set<string> variables = grammer.variables;
  set<string>::iterator it = variables.begin();

  for(int i=0;i<firsts.size();i++,it++)
  {
    cout<<"\nFollow for variable "<<*it<<endl;
    set<string>::iterator it2 = firsts[i].begin();
    while(it2 != firsts[i].end() )
    {
      cout<<*it2<<" ";
      it2++;
    }
    cout<<endl;
  }
  cout<<endl;

  return 0;
}
