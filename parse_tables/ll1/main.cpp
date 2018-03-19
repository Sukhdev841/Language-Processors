#include "../../header-files/s_context_free_grammer.h"

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
	cout<<"size = "<<input_parts.size()<<endl;
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
			///cout<<"\nAccessing "<<var_index[st.top()]<<" "<<term_index[input_parts[i]];
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

	vector<set<string> > firsts;
  grammer.generate_all_firsts();
	cout<<"\nFirst are\n";
	firsts = grammer.all_firsts;
	set<string> variables = grammer.variables;
  set<string>::iterator it = variables.begin();

  for(int i=0;i<firsts.size();i++,it++)
  {
    cout<<"\nFirst for variable "<<*it<<endl;
    set<string>::iterator it2 = firsts[i].begin();
    while(it2 != firsts[i].end() )
    {
      cout<<*it2<<" ";
      it2++;
    }
    cout<<endl;
  }
  cout<<endl;
  firsts = follow(grammer);
  cout<<endl;
  cout<<"\nGot follow size = "<<firsts.size();
  variables = grammer.variables;
  it = variables.begin();

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
