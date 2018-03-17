#include <bits/stdc++.h>
#include "s_string.h"

using namespace std;

class s_production;
class s_context_free_grammer;
vector<set<string> > follow(s_context_free_grammer);
void print(vector<s_production> );

class s_production
{
public:
    string left_side;
    vector<string> right_parts;
    set<string> right_variables;

    s_production()
    {
    }

    s_production(string production)
    {
      extract(production);
    }

    s_production(const s_production &p)
    {
      this->left_side = p.left_side;
      this->right_parts = p.right_parts;
      this->right_variables = p.right_variables;
    }

    void extract(string production)
    {
      production = remove_extra_spaces(production);
      right_variables.clear();
      right_parts.clear();
      left_side = "";
      int i=0;
      while(production[i] != '-')
      {
        if(production[i]==' ')
          i++;
        else
          left_side += production[i++];
      }
      i += 2;
      istringstream f(production.substr(i,production.length()));
      string temp;
      while(getline(f,temp,'|'))
      {
        temp = remove_extra_spaces(temp);
        right_parts.push_back(temp);
        istringstream g(temp);
        string temp2;
        while(getline(g,temp2,' '))
        {
          if(temp2!= "" && temp2!=" ")
            right_variables.insert(temp2);
        }
      }
      remove_null();
    }

    void refresh()
    {
      this->right_variables.clear();
      for(int i=0;i<right_parts.size();i++)
      {
        istringstream g(right_parts[i]);
        string temp2;
        while(getline(g,temp2,' '))
        {
          if(temp2!= "" && temp2!=" ")
            right_variables.insert(temp2);
        }
      }
    }

    bool is_symbol_on_right(string variable)
    {
      return right_variables.find(variable) != right_variables.end();
    }

    bool is_valid()
    {
      if(this->left_side == "" || this->left_side == " " || this->right_parts.size() == 0)
        return false;
      return true;
    }

    bool is_left_variable(string variable)
    {
      return left_side.compare(variable) == 0;
    }

    bool is_part_on_right(string variables)
    {
      return (find(right_parts.begin(),right_parts.end(),variables) != right_parts.end());
    }

    bool are_same(s_production production2)
    {
      if(this->left_side != production2.left_side)
        return false;
      vector<string> right_parts2 = production2.right_parts;
      int count = 0;
      for(int i=0;i<right_parts.size();i++)
      {
        if(find(right_parts2.begin(),right_parts2.end(),right_parts[i]) != right_parts2.end())
          count++;
      }
      if(count != right_parts.size())
        return false;
      count = 0;
      for(int i=0;i<right_parts2.size();i++)
      {
        if(find(right_parts.begin(),right_parts.end(),right_parts2[i]) != right_parts.end())
          count++;
      }
      if(count != right_parts2.size())
        return false;
      return true;
    }

    string get_string_form()
    {
      string temp = left_side + " -> ";
      for(int i=0;i<right_parts.size();i++)
      {
        temp += right_parts[i] + " | ";
      }
      temp = temp.substr(0,temp.length()-1);
      return temp;
    }

    void insert_on_right(string variables)
    {
      while(variables[0]==' ')
        variables = variables.substr(1,variables.length());
      while(variables[variables.length()-1] == ' ')
        variables = variables.substr(0,variables.length()-1);
      right_parts.push_back(variables);
      istringstream g(variables);
      string temp2;
      while(getline(g,temp2,' '))
      {
        if(temp2!= "" && temp2!=" ")
          right_variables.insert(temp2);
      }
      ////cout<<"\nbefore removing null\n";
      //print();
      remove_null();
      refresh();
    }

    bool delete_(string variables)
    {
      vector<string>::iterator var_itr = find(right_parts.begin(),right_parts.end(),variables);
      if(var_itr == right_parts.end())
        return false;
      right_parts.erase(var_itr);
      refresh();
        return true;
    }

    void remove_null()
    {
      for(int i=0;i<right_parts.size();i++)
      {
        if(get_ith_part(right_parts[i],1,' ') == "#" && right_parts[i].size()>1)
          right_parts[i] = remove_ith_part(right_parts[i],1,' ');
        if(get_last_part(right_parts[i],' ') == "#" && right_parts[i].size() > 1)
          right_parts[i] = remove_last_part(right_parts[i],' ');
      }
    }

    void all_combinations(string str,vector<string> replacements,vector<int> positions,int current_position,vector<string> &result)
    {
      if(current_position >= positions.size())
      {
        result.push_back(str);
        return;
      }
      for(int i=0;i<replacements.size();i++)
      {
        string new_string = put_at_index(str,replacements[i],positions[current_position],' ');
        all_combinations(new_string,replacements,positions,current_position+1,result);
      }
    }

    void replace_with(s_production p)
    {
      //cout<<"\nin replace with\n";
      s_production new_p;
      new_p.left_side = this->left_side;
      for(int i=0;i<this->right_parts.size();i++)
      {
        vector<int> positions;
        string str = this->right_parts[i];
        while(true)
        {
          int index = find_index_of(str,p.left_side,' ');
          if(index == -1)
            break;
          positions.push_back(index);
          str = put_at_index(str,"#",index,' ');
        }
        str = this->right_parts[i];
        vector<string> combinations;
        all_combinations(str,p.right_parts,positions,0,combinations);
        for(int j=0;j<combinations.size();j++)
          new_p.insert_on_right(combinations[j]);
      }
      *this = new_p;

      // vector<string> parts_of_p = p.right_parts;
      // s_production new_production;
      // new_production.left_side = this->left_side;
      // bool change_happen = false;
      //
      // for(int i=0;i<right_parts.size();i++)
      // {
      //   // here
      //   // A -> B B B
      //   // B -> a
      //   vector<string> parts = get_parts(right_parts[i],' ');
      //   int index = find(parts.begin(),parts.end(),p.left_side) - parts.begin();
      //   if(index == parts.size())
      //   {
      //     new_production.insert_on_right(right_parts[i]);
      //     continue;
      //   }
      //   change_happen = true;
      //   for(int j=0;j<parts_of_p.size();j++)
      //   {
      //     string temp_right_side = "";
      //     for(int k=0;k<index;k++)
      //       temp_right_side += parts[k] + " ";
      //     temp_right_side += parts_of_p[j] + " ";
      //     for(int k = index+1;k<parts.size();k++)
      //       temp_right_side += parts[k]+" ";
      //     new_production.insert_on_right(temp_right_side);
      //   }
      // }
      // new_production.refresh();
      // if(change_happen)
      //   new_production.replace_with(p);   // recursive implementation
      // *this = new_production;
    }

    vector<s_production> remove_immediate_left_recursion()
    {
      s_production p1,p2;
      p1.left_side = this->left_side;
      p2.left_side = this->left_side+"1";
      bool flag = true;
      for(int i=0;i<right_parts.size();i++)
      {
        if(get_ith_part(right_parts[i],1,' ') == left_side)
        {
          flag = false;
          p2.insert_on_right(remove_ith_part(right_parts[i],1,' ')+" "+left_side+"1");
        }
        else
        {
          p1.insert_on_right(right_parts[i]+" "+left_side+"1");
        }
      }
      if(flag)
      {
        vector<s_production> vec;
        vec.push_back(*this);
        return vec;
      }
      p2.insert_on_right("#");          // # corresponds to NULL
      vector<s_production> vec;
      vec.push_back(p1);
      vec.push_back(p2);
      return vec;
    }

    vector<s_production> left_factor()
    {
      vector<s_production> vec;
      map<string,vector<string> > common_prefix_strings; // map<  prefix  , vector containing all parts with prefix >

      //cout<<"\nRight parts are\n";

      for(int i=0;i<right_parts.size();i++)
      {
        //cout<<"\n-"<<right_parts[i]<<"-";
        common_prefix_strings[get_ith_part(right_parts[i],1,' ')].push_back(right_parts[i]);
      }
      //cout<<endl;
      s_production production1;
      vector<s_production> result;
      map<string,vector < string> > :: iterator c_p_s_itr = common_prefix_strings.begin();
      production1.left_side = this->left_side;
      int count = 1;

      while(c_p_s_itr != common_prefix_strings.end())
      {
        string local_max_prefix = (*c_p_s_itr).first;
        int l = 1;
        vector<string> local_right_parts = (*c_p_s_itr).second;
        for(int length=2; local_right_parts.size() > 1 ;length++)
        {
          string temp_prefix = local_right_parts[0].substr(0,length);
          //cout<<"\nprefix is -"<<temp_prefix<<"-\n";
          bool same_prefix_upto_length = true;
          for(int i=1;i<local_right_parts.size();i++)
          {
          //  //cout<<"\nLocal right parts "<<i<<" = "<<local_right_parts[i]<<endl;
            if( length >= local_right_parts[i].length() || local_right_parts[i].substr(length-1,local_right_parts[i].length()) != temp_prefix)
            {
              //cout<<"\nLoop is breaking due to -"<<local_right_parts[i].substr(0,length)<<"- substring\n";
              same_prefix_upto_length = false;
              break;
            }
          }

          if(same_prefix_upto_length == false)
          {
            l = length - 1;
            break;
          }
        }
        if(local_right_parts.size() == 1)
        {
          production1.insert_on_right(local_right_parts[0]);
          c_p_s_itr++;
          continue;
        }

        string prefix = local_right_parts[0].substr(0,l);

        production1.insert_on_right(prefix + " " + this->left_side + itos(count));
        s_production temp_production;
        temp_production.left_side = this->left_side + itos(count);

        for(int i=0;i<local_right_parts.size();i++)
        {
          if( l+1 < local_right_parts[i].length())
            temp_production.insert_on_right(local_right_parts[i].substr(l+1,local_right_parts[i].length()));
          else
            temp_production.insert_on_right("#");   // put NULL production on right
        }
        result.push_back(temp_production);
        c_p_s_itr++;
        count++;
      }

      vec.push_back(production1);   // first production
      if(result.size() == 0)
      {
        return vec;
      }
      for(int i=0;i<result.size();i++)
      {
        vector<s_production> ans = result[i].left_factor();   // recursive left factoring
        for(int j=0;j<ans.size();j++)
          vec.push_back(ans[j]);
      }
      return vec;
    }

    void print()
    {
        cout<<get_string_form();
    }

    s_production& operator = (const s_production& p)
    {
      if(this != &p)
      {
        this->left_side = p.left_side;
        this->right_parts = p.right_parts;
        this->right_variables = p.right_variables;
      }
      return *this;
    }
};

class s_context_free_grammer
{

 public:
    vector<s_production> productions;
    set<string> variables;
    set<string> terminals;
    string start_variable;
    vector<set<string> > all_firsts;

    s_context_free_grammer()
    {}

    s_context_free_grammer(string production)
    {
      add_production(production);
    }

    s_context_free_grammer(const s_context_free_grammer& g)
    {
      *this  = g;
    }

    void add_production(string production)
    {
      s_production temp_production(production);
      productions.push_back(temp_production);
      set<string> right_variables = temp_production.right_variables;
      set<string>::iterator it = right_variables.begin();
      while(it != right_variables.end())
      {
      	if(!is_a_variable(*it))
        terminals.insert(*it);
        it++;
      }
      terminals.erase(temp_production.left_side);
      variables.insert(temp_production.left_side);
    }

    void add_productions(vector<s_production> productions)
    {
      for(int i=0;i<productions.size();i++)
        add_production(productions[i]);
    }

    void add_production(s_production p)
    {
      string temp = p.get_string_form();
      add_production(temp);
    }

    s_production get_production_with(string var_name)
    {
      for(int i=0;i<productions.size();i++)
        if(productions[i].left_side == var_name)
          return productions[i];
      return *(new s_production());
    }

    int get_production_number(string start_var)
    {
      for(int i=0;i<productions.size();i++)
        if(productions[i].left_side == start_var)
          return i+1;
      return -1;
    }

    void remove_production(string production)
    {
      s_production p(production);
      remove_production(p);
    }

    void remove_production(s_production p)
    {
      for(int i=0;i<productions.size();i++)
      {
        if(productions[i].are_same(p))
        {
          productions.erase(productions.begin()+i);
          break;
        }
      }
      refresh_grammer();
    }

    void remove_productions_starting_with(string variable)
    {
      for(int i=0;i<productions.size();i++)
      {
        if(productions[i].left_side == variable)
        {
          productions.erase(productions.begin()+i);
          break;
        }
      }
    }

    bool is_a_variable(string variable)
    {
      return variables.find(variable) != variables.end();
    }

    bool is_a_terminal(string terminal)
    {
      return terminals.find(terminal) != terminals.end();
    }

    void refresh_grammer()
    {
      terminals.clear();
      variables.clear();
      for(int i=0;i<productions.size();i++)
      {
        set<string> right_variables = productions[i].right_variables;
        set<string>::iterator it = right_variables.begin();
        while(it != right_variables.end())
        {
          terminals.insert(*it);
          it++;
        }
        terminals.erase(productions[i].left_side);
        variables.insert(productions[i].left_side);
      }
    }

    void print()
    {
      cout<<"\n******************************************************\n";
      for(int i=0;i<productions.size();i++)
      {
        cout<<"\n"<<i+1<<") ";
        productions[i].print();
      }
      cout<<"\n\n******************************************************\n";
      cout<<"variables : [ ";
      set<string>::iterator var_itr = variables.begin();
      while( var_itr != variables.end())
      {
        cout<<*var_itr<<" ";
        var_itr++;
      }
      cout<<" ]\nTerminals : [ ";
      set<string>:: iterator ter_itr = terminals.begin();
      while(ter_itr != terminals.end())
      {
        cout<<*ter_itr<<" ";
        ter_itr++;
      }
      cout<<"]\n******************************************************\n";
    }

    s_context_free_grammer& operator = (const s_context_free_grammer& g)
    {
      if(this != &g)
      {
        this->productions = g.productions;
        this->variables = g.variables;
        this->terminals = g.terminals;
        this->start_variable = g.start_variable;
        this->all_firsts = g.all_firsts;
      }
      return *this;
    }

    s_context_free_grammer remove_left_recursion()
    {
      s_context_free_grammer result = *this;
      for(int i=0;i<result.productions.size();i++)
      {
        for(int j=1;j<=i;j++)
        {
          for(int k=0;k<result.productions[j].right_parts.size();k++)
          {
            string left_most = get_ith_part(result.productions[j].right_parts[k],1,' ');
            if(result.get_production_number(left_most) >= 1 && result.get_production_number(left_most) <= i)
            {
                s_production temp_prod = result.get_production_with(left_most);
                result.productions[j].replace_with(temp_prod);
                k=-1;
            }
          }
        }
        for(int j=0;j<=i;j++)
        {
          vector<s_production> vec = result.productions[j].remove_immediate_left_recursion();
          result.productions[j] = vec[0];
          for(int k=1;k<vec.size();k++)
            result.add_production(vec[k]);
        }
      }
      return result;
    }

    vector<set< string> > first()
    {
      vector< set< string> > result;
      for( set<string>::iterator it = variables.begin(); it!= variables.end() ; it++)
      {
        set<string> temp;
        s_production prod = get_production_with(*it);
        if(prod.is_part_on_right("#"))
        {
          temp.insert("#");
        }
        for(int i=0;i<prod.right_parts.size();i++)
        {
          ////cout<<"\nfor\n";
          bool change_happen = true;
          while(change_happen)
          {
            ////cout<<"\nChange happen\n";
            change_happen = false;
            if(is_a_terminal(get_ith_part(prod.right_parts[i],1,' ')))
            {
              ////cout<<"\ninserting\n";
              temp.insert(get_ith_part(prod.right_parts[i],1,' '));
              ////cout<<"\ninserted\n";
            }
            else if(get_ith_part(prod.right_parts[i],1,' ') != prod.left_side)
            {
              // if not same
              //  //cout<<"\nreplacing\n";
              //prod.print();
              ////cout<<"\nwith\n";
              //get_production_with(get_ith_part(prod.right_parts[i],1,' ')).print();
              ////cout<<endl;
              prod.replace_with(get_production_with(get_ith_part(prod.right_parts[i],1,' ')));
              ////cout<<"\nreplaced\n";
              change_happen = true;
            }
          }
        }
        result.push_back(temp);
      }
      return result;
    }

    void generate_all_firsts()
    {
      this->all_firsts = first();
      ////cout<<"\nsize of all first internally = "<<all_firsts.size()<<endl;
    }
};

vector<set<string> > follow(s_context_free_grammer g)
{
  ////cout<<"\nIn follow\n";
  ////cout<<"\nIn follow\n";
	vector< set<string> > result1,result2,result;
	vector<string> vars = to_vector(g.variables);
  ////cout<<"\nVars found\n";
  ////cout<<"\nVars found\n";
	vector<string> terms = to_vector(g.terminals);
  ////cout<<"\nterms found\n";
  ////cout<<"\nterms found\n";
	vector< set<string> > firsts_ = g.first();
  ////cout<<"\nfirst found\n";
  ////cout<<"\nfirst found\n";
	////cout<<"\nvars = "<<vars.size();
	////cout<<"\nterms = "<<terms.size();
	////cout<<"\nfirsts = "<<firsts_.size();
	////cout<<"\nFirst are \n";
	//print(firsts_);

	// loop1
	////cout<<"\nloop1\n";
	////cout<<"\nloop1\n";
	for(int i=0;i<vars.size();i++)
	{
		set<string> local_set;
		if(vars[i]==g.start_variable)
    {
        local_set.insert("$");
    }
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

	////cout<<"\nloop2\n";
	////cout<<"\nloop2\n";
	// loop2
  // cout<<"\nAfter loop1\n";
  // print(result1);
  // cout<<endl;
  for(int i=0;i<vars.size();i++)
  {
    set<string> x;
    result2.push_back(x);
  }
  //  result2.push_back(new set<string> () );
	bool change_happen = true;
	int count = 7;
	while(count--){
	change_happen = false;

	for(int i=0;i<vars.size();i++)
	{
		// cout<<"\n-----------------------------------------------------------------------------------\n";
		// cout<<"\nvar = "<<vars[i];
		set<string> local_set = result2[i];
		if(i==0)
			local_set.insert("$");
		for(int j=0;j<g.productions.size();j++)
		{
			////cout<<"\nproduction = ";
			//g.productions[j].print();
			vector<string> production_parts = g.productions[j].right_parts;
			// search for var[i]
			for(int k=0;k<production_parts.size();k++)
			{
				//now search
				vector<string> zero_parts = get_parts(production_parts[k],' ');
				int index = find(zero_parts.begin(),zero_parts.end(),vars[i]) - zero_parts.begin();
				////cout<<"\nindex = "<<index;
				////cout<<"\nzero_parts size = "<<zero_parts.size();
				if( index == zero_parts.size())
					continue;		// not found

				if( index + 1 >= zero_parts.size() )
				{
					// condition satisfied
					////cout<<"\nThere is nothing to right (condition satisfied)\n";

					// find index of variable at left side
					index = find(vars.begin(),vars.end(),g.productions[j].left_side) - vars.begin();
					set<string> temp_set = result1[index];
					temp_set.erase("#");
					local_set = merge_(local_set,temp_set);
					////cout<<"\nSo new local set is \n";
					//print(local_set);
					continue;		//done
				}

				// check if first of right contains NULL (#)
				// index of variable at index+1
				string next_var = zero_parts[index+1];
				if(g.is_a_terminal(next_var))
				{
					////cout<<"\nNext is terminal !\n";
					// terminal (can't be NULL)
					continue;
				}
				////cout<<"\nNext variable is : "<<next_var<<endl;
				int v_index = find(vars.begin(),vars.end(),next_var) - vars.begin();
				////cout<<"\nSize of result1 = "<<result1.size()<<endl;
				////cout<<"\nv_index = "<<v_index;
				// not find its first
				set<string> v_first = firsts_[v_index];
				set<string>::iterator xxx = v_first.find("#");
				if(xxx == v_first.end())
				{
					////cout<<"\nNext variable's first doesn't contain #\n";
					continue;
				}
        // next var can be null ( # )
				//merge it with local set
				////cout<<"\nlocal size = "<<local_set.size()<<" result1[v_index].size() = "<<result1[v_index].size()<<endl;
        v_index = find(vars.begin(),vars.end(),g.productions[j].left_side) - vars.begin();
        // cout<<"\nCurrent variable : "<<vars[i]<<endl;
        // cout<<"Left side var is : "<<vars[v_index]<<endl;
        // g.productions[j].print();
        // cout<<endl;
				set<string> temp_set = result1[v_index];
				temp_set.erase("#");
        // cout<<"Result1["<<v_index<<"] is \n";
        // print(temp_set);
        // cout<<"\nLocal set before merging : \n";
        // print(local_set);
				local_set = merge_(temp_set,local_set);
        // cout<<"\nLocal set after merging : ";
        // print(local_set);
        // cout<<"\nOld result1 is : \n";
        // print(result1[v_index]);
        result1[v_index] = local_set;
        // cout<<"\nNew result1 is : \n";
        // print(result1[v_index]);
				////cout<<"\nNext variable's first contains # (condition satisfied)\n";
				////cout<<"\nSo new local set is \n";
				//print(local_set);
				////cout<<"\nwhich is result of merging\n";
				//print(result1[v_index]);
			}
		}
		result2[i]= local_set;

	}
	////cout<<"\nResult 1 \n";
	//print(result1);
	////cout<<"\nresult2\n";
	//print(result2);


	result.clear();
	for(int i=0;i<vars.size();i++)
		result.push_back(merge_(result1[i],result2[i]));
	result1 = result;
  result2 = result;
	//result2.clear();
	}
	return result1;
	//cout<<"\nafter loop2\n";
	//cout<<"\nafter loop2\n";
	//last
	for(int i=0;i<vars.size();i++)
	{
		result.push_back(merge_(result1[i],result2[i]));
	}
	return result;
}

void print(vector<s_production> p)
{
  cout<<endl;
  for(int i=0;i<p.size();i++)
  {
    cout<<i+1<<")\t";
    p[i].print();
    cout<<endl;
  }
  cout<<endl;
}
