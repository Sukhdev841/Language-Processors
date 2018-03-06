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

int** parse_table(vector<set<string> > follows,s_context_free_grammer g)
{
	cout<<"\n---------------------------------------------------------------------------------------\n";
	cout<<"\n---------------------------------------------------------------------------------------\n";
	cout<<"\n---------------------------------------------------------------------------------------\n";
	cout<<"\n                               	     PARSE TABLE\n";
	for(int i=0;i<g.productions.size();i++)
	{
		cout<<i+1<<") ";
		g.productions[i].print();
		cout<<endl;
	}
	
	int **arr = new int*[g.variables.size()];
	
	for(int i=0;i<g.variables.size();i++)
	{
		arr[i] = new int[g.terminals.size()];
		for(int j=0;j<g.terminals.size();j++)
			arr[i][j] = 0;
	}
	
	vector<string> vars = to_vector(g.variables);
	set<string> temp = g.terminals;
	temp.erase("#");
	temp.insert("$");
	vector<string> terms = to_vector(temp);
	map<string,int> var_index,term_index;
	cout<<endl;
	for(int i=0;i<vars.size();i++)
	{
		cout<<vars[i]<<" = "<<i<<endl;
		var_index[vars[i]]=i;
	}
	cout<<endl;
	for(int i=0;i<terms.size();i++)
	{
		cout<<terms[i]<<" = "<<i<<endl;
		term_index[terms[i]]=i;
	}
		
		
	vector<set<string> > firsts_ = g.first();
	
	for(int i=0;i<g.productions.size();i++)
	{
		int left_index = var_index[g.productions[i].left_side];
		vector<string> right_parts = g.productions[i].right_parts;
		cout<<endl;
		g.productions[i].print();
		cout<<endl;
		cout<<endl;
		for(int j=0;j<right_parts.size();j++)
		{
			string var_zero = get_ith_part(right_parts[j],1,' ');
			if(g.is_a_terminal(var_zero) && var_zero!="#")
			{
				cout<<"var_zero : "<<var_zero<<" is a terminal\n";
				cout<<10*(i+1)+j+1<<" is added to M["<<g.productions[i].left_side<<"]["<<var_zero<<"] due to ";
				cout<<g.productions[i].left_side<<" -> "<<right_parts[j]<<endl;
				arr[left_index][term_index[var_zero]] = 10*(i+1)+j+1;
			}
			else if(var_zero == "#")
			{
				set<string> local_follow = follows[left_index];
					set<string>::iterator it = local_follow.begin();
					while( it!=local_follow.end())
					{
						if(*it != "#")
						{
							cout<<"it = "<<*it<<endl;
							cout<<10*(i+1)+j+1<<" is added to M["<<g.productions[i].left_side<<"]["<<*it<<"] due to ";
							cout<<g.productions[i].left_side<<" -> "<<right_parts[j]<<endl;
							arr[left_index][term_index[*it]]=10*(i+1)+j+1;
						}
						it++;
					}	
			}
			else if(g.is_a_variable(var_zero))
			{
				cout<<"var_zero : "<<var_zero<<" is not a terminal\n";
				
				set<string> local_firsts = firsts_[var_index[var_zero]];
				set<string>::iterator it = local_firsts.begin();
				while( it!=local_firsts.end())
				{
					cout<<"in local firsts\n";
					if(*it != "#")
					{
						cout<<"it = "<<*it<<endl;
						cout<<10*(i+1)+j+1<<" is added to M["<<g.productions[i].left_side<<"]["<<*it<<"] due to ";
						cout<<g.productions[i].left_side<<" -> "<<right_parts[j]<<endl;
						arr[left_index][term_index[*it]]=10*(i+1)+j+1;
					}
					it++;
				}
				//setp 3
				if(local_firsts.find("#") != local_firsts.end())
				{
					cout<<"\nvar zero's first also contains NULL\n";
					set<string> local_follow = follows[left_index];
					it = local_follow.begin();
					while( it!=local_follow.end())
					{
						if(*it != "#")
						{
							cout<<"it = "<<*it<<endl;
							cout<<10*(i+1)+j+1<<" is added to M["<<g.productions[i].left_side<<"]["<<*it<<"] due to ";
							cout<<g.productions[i].left_side<<" -> "<<right_parts[j]<<endl;
							arr[left_index][term_index[*it]]=10*(i+1)+j+1;
						}
						it++;
					}
				}
				
			}
		}
	}
	cout<<endl;
	cout<<"\t";
	for(int j=0;j<g.terminals.size();j++)
		cout<<terms[j]<<"\t";
	cout<<endl;
	for(int i=0;i<g.variables.size();i++)
	{
		cout<<vars[i]<<"\t";
		for(int j=0;j<g.terminals.size();j++)
			cout<<arr[i][j]<<"\t";
		cout<<endl;
	}
	cout<<endl;
	return arr;
}

string print(stack<string> s)
{
	if(s.empty())
		return "";
	string x = s.top();
	s.pop();
	return x+print(s);
}

bool parse(string input,int **arr,s_context_free_grammer g)
{
	///
	cout<<"\ninput = "<<input<<endl;
	vector<string> input_parts = get_parts(input,' ');
	vector<string> vars = to_vector(g.variables);
	set<string> temp = g.terminals;
	temp.erase("#");
	temp.insert("$");
	vector<string> terms = to_vector(temp);
	map<string,int> var_index,term_index;
	cout<<endl;
	for(int i=0;i<vars.size();i++)
	{
		cout<<vars[i]<<" = "<<i<<endl;
		var_index[vars[i]]=i;
	}
	cout<<endl;
	for(int i=0;i<terms.size();i++)
	{
		cout<<terms[i]<<" = "<<i<<endl;
		term_index[terms[i]]=i;
	}
	
	///
	stack<string> st;
	st.push("$");
	st.push(g.productions[0].left_side);
	//cout<<"size = "<<input_parts.size()<<endl;
	cout<<"\nSTACK\t\t\tINPUT\t\t\t\tOUTPUT\n";
	for(int i=0;i<input_parts.size();i++)
	{
		string stack_string = print(st);
		string input_remaining = "";
		for(int x=i;x<input_parts.size();x++)
			input_remaining += input_parts[x];
		
		cout<<stack_string<<setw(30-stack_string.length())<<input_remaining;
		
		if(g.is_a_terminal(st.top()) || st.top()=="$")
		{
			if(st.top() == input_parts[i])
				st.pop();
			else
				return false;
		}
		else
		{
			//cout<<"\nAccessing "<<var_index[st.top()]<<" "<<term_index[input_parts[i]];
			int val = arr[var_index[st.top()]][term_index[input_parts[i]]];
			int col = val%10;
			int row = val/10;
			st.pop();
			s_production temp_pr = g.productions[row-1];
			string part = temp_pr.right_parts[col-1];
			cout<<setw(30-input_remaining.length())<<temp_pr.left_side<<" -> "<<part<<endl;
			vector<string> parts = get_parts(part,' ');
			for(int j=parts.size()-1;j>=0;j--)
			{
				if(parts[j]!="#")
					st.push(parts[j]);
				//cout<<"\npusing part "<<parts[j];
				
			}
			i--;
				
			//find prodction using col , row
			
		}
		cout<<endl;
	}
	return true;
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
  
	int **t =  parse_table(firsts,grammer);
	cout<<"\nEnter string to parse : ";
	string input;
	cin.clear();
	//cin.ignore();
	getline(cin,input);
	if(parse(input,t,grammer))
	{
		cout<<"\nString parsed\n";
	}
	else
		cout<<"\nString not parsed\n";

  return 0;
}
