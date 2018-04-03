#include "../../header-files/s_context_free_grammer.h"

map<string,s_production> productions_map;
s_context_free_grammer grammer;

vector<s_context_free_grammer> dfa_states;
vector<string> variables;
vector<string> terminals;
map<string,int> var_index;
int states_count = 0;
vector<vector<int> > dfa;
vector<vector<string> > action_table;

s_production put_dot(s_production,int);
s_context_free_grammer find_itemsets(vector<s_production>);
int find_dot_position(s_production);
s_context_free_grammer clousre(s_context_free_grammer);
bool compare_kernels(vector<s_production>,vector<s_production>);
s_production remove_dot(s_production p);
vector<s_production> propagate_dot(vector<s_production> v) ;
void print_lr();
vector<s_production>  generate_action_table();
void action(int i,string var,string val);
bool parse(string,vector<s_production>);

int main()
{
	int n;
  string temp;
  cin>>n;
  cin.clear();
  cin.ignore();
  for(int i=0;i<n;i++)
  {
    getline(cin,temp);
    s_production x(temp);
    productions_map[x.left_side] = x;
    grammer.add_production(temp);
  }
  getline(cin,temp);
  grammer.start_variable = temp;

  variables = to_vector(grammer.variables);
  terminals = to_vector(grammer.terminals);
  //terminals.push_back("$");
  for(int i=0;i<terminals.size();i++)
    variables.push_back(terminals[i]);
  for(int i=0;i<variables.size();i++)
    var_index[variables[i]]=i;
  grammer.print();
  //cout<<"\nStarting symbol = "<<grammer.start_variable<<endl;

	//s_context_free_grammer g2 = grammer.remove_left_recursion();
	s_context_free_grammer g2 = grammer;
	//cout<<"\nWithout left recursion\n";
	//g2.print();
	g2.start_variable = "E'";
	cout<<"\nStart variable of grammer : "<<g2.start_variable<<endl;
	vector<set<string> > firsts = g2.first();
	cout<<"\nFirsts are\n";
	print(firsts);
	vector<set<string> > follows = follow(g2);
	cout<<"\nFollows\n";
	print(follows);

	vector<s_production> v;
	grammer.print();
  v.push_back(grammer.productions[0]);

  s_context_free_grammer tg = find_itemsets(v);
  //tg.print();
  print_lr();
	vector<s_production> r_ps = generate_action_table();
	string tt;
	getline(cin,tt);
	if(parse(tt,r_ps))
	{
		cout<<"\nString is parsed\n";
	}
	else
		cout<<"\nString not parsed\n";
  return 0;
}

s_context_free_grammer find_itemsets(vector<s_production> p)
{
  s_context_free_grammer g;
  map<string,int> already_present;
  for(int i=0;i<p.size();i++)
  {
    int position = find_dot_position(p[i]);
    s_production new_p = remove_dot(p[i]);
    s_production kernel = put_dot(new_p,position+1);
    for(int j=0;j<kernel.right_parts.size();j++)
    {
      s_production temp_p;
      temp_p.left_side = kernel.left_side;
      temp_p.insert_on_right(kernel.right_parts[j]);
      g.add_production(temp_p);
    }
    //already_present[p[i].left_side]=1;      // check this out
  }
  dfa_states.push_back(g);  //push only kernels
  s_context_free_grammer complete_set = clousre(g);
  map<string,vector<s_production> > new_kernels;
  for(int i=0;i<complete_set.productions.size();i++)
  {
    int position = find_dot_position(complete_set.productions[i]);
    string next_var = get_ith_part(complete_set.productions[i].right_parts[0],position+1,' ');
    if( next_var == "" || next_var == " ")
      continue;
    new_kernels[next_var].push_back(complete_set.productions[i]);
  }
  vector<int> jumps(variables.size(),-1);
  // put dummy in dfa
  dfa.push_back(jumps);
  int my_state = states_count;
  for(map<string,vector<s_production> >::iterator it = new_kernels.begin();it!=new_kernels.end();it++)
  {
    // first check if state already exists with the kernel
    bool state_already_exists = false;
    for(int i=0;i<dfa_states.size();i++)
    {
      vector<s_production> p_kernels = propagate_dot(it->second);
      if(compare_kernels(dfa_states[i].productions,p_kernels))
      {
        state_already_exists = true;
        jumps[var_index[it->first]]=i;
        cout<<"\nState : "<<my_state<<" on "<<it->first<<" goes to state : "<<i<<endl;
        break;
      }
    }
    if(state_already_exists)
      continue;
    jumps[var_index[it->first]]=++states_count;
    cout<<"\nState : "<<my_state<<" on "<<it->first<<" goes to state : "<<states_count<<endl;
    find_itemsets(it->second);
  }
  for(int i=0;i<jumps.size();i++)
    dfa[my_state][i]=jumps[i];
  return complete_set;
}

bool compare_kernels(vector<s_production> k1,vector<s_production> k2)
{
  // cout<<"\nComparing kernels\n";
  // for(int i=0;i<k1.size();i++)
  // {
  //   k1[i].print();
  //   cout<<endl;
  // }
  // cout<<"\nWith\n";
  // for(int i=0;i<k2.size();i++)
  // {
	// 	cout<<"\nk2\n";
  //   k2[i].print();
  //   cout<<endl;
  // }
  for(int i=0;i<k1.size();i++)
  {
    bool flag = true;
    for(int j=0;j<k2.size();j++)
      if(k1[i].are_same(k2[j]))
      {
        flag = false;
        break;
      }
    if(flag)
      return false;
  }
  for(int i=0;i<k2.size();i++)
  {
    bool flag = true;
    for(int j=0;j<k1.size();j++)
      if(k2[i].are_same(k1[j]))
      {
        flag = false;
        break;
      }
    if(flag)
      return false;
  }
  return true;
}

s_context_free_grammer clousre(s_context_free_grammer g)    // g = kernels
{
  map<string,int> already_present;
  for(int i=0;i<g.productions.size();i++)
  {
    //g.productions[i].print();
    int index = find_dot_position(g.productions[i]);
    //cout<<"\ndot index = "<<index<<endl;
    for(int j=0;j<g.productions[i].right_parts.size();j++)
    {
      string part = get_ith_part(g.productions[i].right_parts[j],index+1,' ');
      if(part == "" || part ==" ")
        continue;
      else if(!already_present[part] && !grammer.is_a_terminal(part))
      {
        //cout<<endl<<part<<" is [OK]\n";
        already_present[part] = true;
        s_production x = put_dot(productions_map[part],1);
        for(int k=0;k<x.right_parts.size();k++)
        {
          s_production xx;
          xx.left_side = x.left_side;
          xx.insert_on_right(x.right_parts[k]);
          g.add_production(xx);
        }
      }
      // else
      // {
      //   if(grammer.is_a_terminal(part))
      //     cout<<endl<<part<<" is a terminal.\n";
      //   else
      //     cout<<endl<<part<<" is already present.\n";
      // }
    }
  }
  return g;
}

s_production put_dot(s_production p,int position)
{
  s_production temp;
  temp.left_side = p.left_side;
  vector<string> right_parts = p.right_parts;
  for(int i=0;i<right_parts.size();i++)
  {
    string temp_string="";
    vector<string> small_parts = get_parts(right_parts[i],' ');
    for(int j=1;j<position && j-1 < small_parts.size();j++)
      temp_string += small_parts[j-1]+" ";
    temp_string += ". ";
    for(int j=position-1;j<small_parts.size();j++)
      temp_string += small_parts[j]+" ";
    temp_string = temp_string.substr(0,temp_string.length()-1);
    temp.insert_on_right(temp_string);
  }
  return temp;
}

int find_dot_position(s_production p)
{
  //cout<<"\nFinding dot in \n";
  //p.print();
  //cout<<endl;
  int max_ = -1;
  //cout<<"right parts size = "<<p.right_parts.size()<<endl;
    for(int i=0;i<p.right_parts.size();i++)
    {
      vector<string> small_parts = get_parts(p.right_parts[i],' ');
      for(int j=0;j<small_parts.size();j++)
      {
        //cout<<"\nsmall parts["<<j<<"] = "<<small_parts[j];
        if(small_parts[j] == ".")
        {
          if(max_ < j+1)
            max_ = j+1;
          break;
        }
      }
    }
    if(max_ == -1)
    {
      //  cout<<"\nmax = -1\n";
        return 0;
    }
    return max_;
}

vector<s_production> propagate_dot(vector<s_production> v)      // singal productions
{
  vector<s_production> result;
  for(int i=0;i<v.size();i++)
  {
    int index = find_dot_position(v[i]);
    //cout<<"\nold production is \n";
    //cout<<"\nindex = "<<index<<endl;
    //v[i].print();
    //cout<<endl;
    s_production new_p = remove_dot(v[i]);
    new_p = put_dot(new_p,index+1);
  //  cout<<"\nnew productions is \n";
    //new_p.print();
    result.push_back(new_p);
  }
  return result;
}

s_production remove_dot(s_production p)
{
  int index = find_dot_position(p);
  s_production t_p;
  t_p.left_side = p.left_side;
  for(int i=0;i<p.right_parts.size();i++)
  {
    int index = find_index_of(p.right_parts[i],".",' ');
    string str = remove_ith_part(p.right_parts[i],index,' ');
    t_p.insert_on_right(str);
  }
  return t_p;
}

void print_lr()
{
  // cout<<endl<<"   ";
  // for(int i=0;i<variables.size();i++)
  //   cout<<variables[i]<<" ";
  cout<<endl;
  for(int i=0;i<dfa.size();i++)
  {
    cout<<i<<" ";
    for(int j=0;j<dfa[i].size();j++)
    {
      if(dfa[i][j]!=-1)
      {
        cout<<dfa[i][j]<<"["<<variables[j]<<"]  ";
      }
    }
    cout<<endl;
  }
  cout<<endl;
  for(int i=0;i<dfa_states.size();i++)
  {
    cout<<"State : "<<i<<"\n";
		s_context_free_grammer temp_g = clousre(dfa_states[i]);
    print(temp_g.productions);
  }
  cout<<endl;
}

vector<s_production> generate_action_table()
{
	//s_context_free_grammer new_grammer = grammer.remove_left_recursion();
	s_context_free_grammer new_grammer = grammer;
	vector<set<string> > follows = follow(new_grammer);
  action_table.resize(states_count+1);
	map<string,int> var_index2;
	vector<string> new_vars = to_vector(new_grammer.variables);
	for(int i=0;i<new_vars.size();i++)
		var_index2[new_vars[i] ]=i;
  for(int i=0;i<states_count+1;i++)
  {
    action_table[i].resize(terminals.size()+1,"-");
  }

	vector<s_production> reducing_productions;

  for(int i=0;i<dfa_states.size();i++)
  {
    s_context_free_grammer temp_g = clousre(dfa_states[i]);
    for(int j=0;j<temp_g.productions.size();j++)
    {
      int index = find_dot_position(temp_g.productions[j]);
      string next_var = get_ith_part(temp_g.productions[j].right_parts[0],index+1,' ');
      if(next_var == "" || next_var == " ")
      {
        if(temp_g.productions[j].left_side == grammer.start_variable)
          action(i,"$","A");
        else
        {
            int var_i = var_index2[temp_g.productions[j].left_side];
            set<string> local_f = follows[var_i];
            for(set<string>::iterator it=local_f.begin();it!=local_f.end();it++)
						{
							// first check if this production is already present in reduce temp_set
							bool flag = false;
							for(int k=0;k<reducing_productions.size();k++)
							{
								if(reducing_productions[k].left_side == temp_g.productions[j].left_side && reducing_productions[k].right_parts[0] == temp_g.productions[j].right_parts[0])
								{
									flag = true;
									action(i,*it,"R"+itos(k+1));
									break;
								}
							}
							if(!flag)
							{
								s_production p1;
								p1.left_side = temp_g.productions[j].left_side;
								p1.insert_on_right(temp_g.productions[j].right_parts[0]);
								reducing_productions.push_back(p1);
								action(i,*it,"R"+itos(reducing_productions.size()));
							}
						}
        }
      }
      else
      {
          char temp_ch[100];
          itoa(dfa[i][var_index[next_var]],temp_ch,10);
          string xx(temp_ch);
          xx = "S"+xx;
          action(i,next_var,xx);
      }
    }
  }
  cout<<endl;
  cout<<"    ";
	terminals.push_back("$");
  for(int i=0;i<terminals.size();i++)
    cout<<terminals[i]<<"\t\t\t";
  cout<<endl<<endl;
  for(int i=0;i<action_table.size();i++)
  {
    cout<<i<<" : ";
    for(int j=0;j<action_table[i].size();j++)
      cout<<action_table[i][j]<<"\t\t\t";
    cout<<endl;
  }
	cout<<"\nReducing productions : \n";
	vector<s_production> reducing_productions_result;
	for(int i=0;i<reducing_productions.size();i++)
	{
		cout<<i+1<<".\t";
		reducing_productions_result.push_back(remove_dot(reducing_productions[i]));
		remove_dot(reducing_productions[i]).print();
		cout<<endl;
	}
	return reducing_productions_result;
}

void action(int i,string var,string val)
{
  if(var == "$")
  {
    action_table[i][5] = val;
    return;
  }
  else if(grammer.is_a_terminal(var))
  {
      action_table[i][var_index[var]-terminals.size()+1] = val;
  }
}

int print(stack<int> s)
{
	if(s.empty())
		return -1;
	int temp = s.top();
	s.pop();
	print(s);
	cout<<temp<<" ";
}

bool parse(string input,vector<s_production> r_ps)
{
	// terminals already contain "$"
	map<string,int> l_ter_index;
	for(int i=0;i<terminals.size();i++)
	{
		l_ter_index[terminals[i]]=i;
	}
	vector<string> input_parts = get_parts(input,' ');
	stack<int> stk;
	stk.push(0);
	int ip = 0;
	while(true)
	{
		 cout<<endl;
		// print(stk);
		// cout<<endl;
		// cout<<stk.top()<<" "<<l_ter_index[input_parts[ip]]<<endl;
		//cout<<"\t\t";
		for(int i=ip;i<input_parts.size();i++)
		{
		   cout<<input_parts[i];
		}
		cout<<"\t\t\t";
		if(action_table[stk.top()][l_ter_index[input_parts[ip]]][0] == 'S' )
		{
			// case shift
			string temp = action_table[stk.top()][l_ter_index[input_parts[ip]]];
			stk.push(l_ter_index[input_parts[ip]]);
			stk.push(stoi(temp.substr(1,temp.length())));
			ip++;
			cout<<"Shift";
		}
		else if (action_table[stk.top()][l_ter_index[input_parts[ip]]][0] == 'R' )
		{
			//case 2
			string temp = action_table[stk.top()][l_ter_index[input_parts[ip]]];
			int i = stoi(temp.substr(1,temp.length()));
			vector<string> temp_v = get_parts(r_ps[i-1].right_parts[0],' ');
			int count = get_parts(r_ps[i-1].right_parts[0],' ').size();
			string A = r_ps[i-1].left_side;
			while(count--)
			{
				stk.pop();
				stk.pop();
			}
			//cout<<endl;
			r_ps[i-1].print();
			//cout<<endl;
			int new_top_state = stk.top();
			stk.push(-1);
			stk.push(dfa[new_top_state][var_index[A]]);
		}
		else if(action_table[stk.top()][l_ter_index[input_parts[ip]]][0] == 'A')
		{
			cout<<"\nAccept";
			return true;
		}
		else
		{
			cout<<"\nError occured\n";
			break;
		}
	}
	return false;
}
