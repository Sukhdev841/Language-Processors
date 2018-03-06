#include "../../header-files/s_context_free_grammer.h"

vector<string> to_vector( set<string> s)
{
	vector<string> vec;
	set<string>::iterator it = s.begin();
	while(it!=s.end())
	{
		vec.push_back(*it);
		it++;
	}
	return vec;
}

set<string> merge_(set<string> s1,set<string> s2)
{
	if(s1.size() == 0)
		return s2;
	if(s2.size() == 0)
		return s1;
	set<string> s3;
	set<string>::iterator it = s1.begin();
	while(it!=s1.end())
	{
		s3.insert(*it);
		it++;
	}
	it = s2.begin();
	while(it!=s2.end())
	{
		s3.insert(*it);
		it++;
	}
	return s3;
}

void print(set<string> s)
{	
		cout<<endl;
		set<string>::iterator it = s.begin();
		while(it!=s.end())
		{
			cout<<*it<<" ";
			it++;
		}
		cout<<endl;
}

void print(vector<set<string> >s)
{
	cout<<endl;
	for(int i=0;i<s.size();i++)
	{
		print(s[i]);
	}
	
}


vector<set<string> > follow(s_context_free_grammer g)
{
	vector< set<string> > result1,result2,result;
	vector<string> vars = to_vector(g.variables);
	vector<string> terms = to_vector(g.terminals);
	vector< set<string> > firsts_ = g.first();
	cout<<"\nvars = "<<vars.size();
	cout<<"\nterms = "<<terms.size();
	cout<<"\nfirsts = "<<firsts_.size();
	cout<<"\nFirst are \n";
	print(firsts_);
	
	// loop1
	cout<<"\nloop1\n";
	cout<<"\nloop1\n";
	for(int i=0;i<vars.size();i++)
	{
		set<string> local_set;
		if(i==0)
			local_set.insert("$");
		for(int j=0;j<g.productions.size();j++)
		{
			vector<string> production_parts = g.productions[j].right_parts;
			// search for var[i]
			for(int k=0;k<production_parts.size();k++)
			{
				//now search
				vector<string> zero_parts = get_parts(production_parts[k],' ');
				int index = find(zero_parts.begin(),zero_parts.end(),vars[i]) - zero_parts.begin();
				if( index == zero_parts.size() || index+1 >= zero_parts.size()) 
					continue;		// not found or no next variable
				// index of variable at index+1
				string next_var = zero_parts[index+1];
				if(g.is_a_terminal(next_var))
				{
					local_set.insert(next_var);		// if next var is a terminal
					continue;
				}
				int v_index = find(vars.begin(),vars.end(),next_var) - vars.begin();
				// not find its first
				set<string> v_first = firsts_[v_index];
				//merge it with local set
				v_first.erase("#");
				local_set = merge_(v_first,local_set);
			}
		}
		result1.push_back(local_set);
	}
	
	cout<<"\nloop2\n";
	cout<<"\nloop2\n";
	// loop2
	
	bool change_happen = true;
	int count = 10;
	while(count--){
	change_happen = false;
	
	for(int i=0;i<vars.size();i++)
	{
		cout<<"\n-----------------------------------------------------------------------------------\n";
		cout<<"\nvar = "<<vars[i];
		set<string> local_set;
		if(i==0)
			local_set.insert("$");
		for(int j=0;j<g.productions.size();j++)
		{
			cout<<"\nproduction = ";
			g.productions[j].print();
			vector<string> production_parts = g.productions[j].right_parts;
			// search for var[i]
			for(int k=0;k<production_parts.size();k++)
			{
				//now search
				vector<string> zero_parts = get_parts(production_parts[k],' ');
				int index = find(zero_parts.begin(),zero_parts.end(),vars[i]) - zero_parts.begin();
				cout<<"\nindex = "<<index;
				cout<<"\nzero_parts size = "<<zero_parts.size();
				if( index == zero_parts.size()) 
					continue;		// not found
					
				if( index + 1 >= zero_parts.size() ) 
				{
					// condition satisfied
					cout<<"\nThere is nothing to right (condition satisfied)\n";
					
					// find index of variable at left side
					index = find(vars.begin(),vars.end(),g.productions[j].left_side) - vars.begin();
					set<string> temp_set = result1[index];
					temp_set.erase("#");
					local_set = merge_(local_set,temp_set);
					cout<<"\nSo new local set is \n";
					print(local_set);
					continue;		//done
				}
				
				// check if first of right contains NULL (#)
				// index of variable at index+1
				string next_var = zero_parts[index+1];
				if(g.is_a_terminal(next_var))
				{
					cout<<"\nNext is terminal !\n";
					// terminal (can't be NULL)
					continue;
				}
				cout<<"\nNext variable is : "<<next_var<<endl;
				int v_index = find(vars.begin(),vars.end(),next_var) - vars.begin();
				cout<<"\nSize of result1 = "<<result1.size()<<endl;
				cout<<"\nv_index = "<<v_index;
				// not find its first
				set<string> v_first = firsts_[v_index];
				set<string>::iterator xxx = v_first.find("#");
				if(xxx == v_first.end())
				{
					cout<<"\nNext variable's first doesn't contain #\n";
					continue;
				}
				//merge it with local set
				cout<<"\nlocal size = "<<local_set.size()<<" result1[v_index].size() = "<<result1[v_index].size()<<endl;
				set<string> temp_set = result1[v_index];
				temp_set.erase("#");
				local_set = merge_(temp_set,local_set);
				cout<<"\nNext variable's first contains # (condition satisfied)\n";
				cout<<"\nSo new local set is \n";
				print(local_set);
				cout<<"\nwhich is result of merging\n";
				print(result1[v_index]);
			}
		}
		result2.push_back(local_set);
		
	}
	cout<<"\nResult 1 \n";
	print(result1);
	cout<<"\nresult2\n";
	print(result2);


	result.clear();
	for(int i=0;i<vars.size();i++)
		result.push_back(merge_(result1[i],result2[i]));
	result1 = result;
	result2.clear();
	}
	return result1;
	cout<<"\nafter loop2\n";
	cout<<"\nafter loop2\n";
	//last
	for(int i=0;i<vars.size();i++)
	{
		result.push_back(merge_(result1[i],result2[i]));
	}
	return result;
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
  cout<<"\nGot follow size = "<<firsts.size();
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
