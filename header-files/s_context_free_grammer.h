#include <bits/stdc++.h>

using namespace std;

class s_production
{
  private:
    string left_side;
    vector<string> right_parts;
    set<string> right_variables;
  public:
    s_production()
    { }

    s_production(string production)
    {
      extract(production);
    }

    void extract(string production)
    {
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
      istringstream f(substr(i,production.length()));
      string temp;
      while(getline(cin,temp,'|'))
      {
        while(temp[0]==' ')
          temp = temp.substr(1,temp.length());
        while(temp[temp.length()-1] == ' ')
          temp = temp.substr(0,temp.length()-1);
        right_parts.push_back(temp);
        istringstream g(temp);
        string temp2;
        while(getline(cin,temp2,' '))
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

    bool is_left_variable(string variable)
    {
      return left_side.compare(variable) == 0;
    }

    bool is_on_right(string variables)
    {
      return right_parts.find(right_parts.begin(),right_parts.end(),variables) != right_parts.end();
    }

    set<string> get_right_variables()
    {
      return this.right_variables;
    }

    string get_left_side()
    {
      return this.left_side;
    }

    string get_string_form()
    {
      string temp = left_side + " -> ";
      for(int i=0;i<right_parts.size();i++)
      {
        temp += right_parts[i] + " | ";
      }
      return temp;
    }

    void print()
    {
      cout<<get_string_form();
    }
};

class s_context_free_grammer
{
  private:
    vector<s_production> productions;
    set<string> variables;
    set<string> terminals;
  public:
    s_context_free_grammer()
    {}

    void add_production(string production)
    {
      s_production temp_production(production);
      productions.push_back(temp_production);
      set<string> right_variables = temp_production.get_right_variables();
      set<string>::iterator it = right_variables.begin();
      while(it != right_variablese.end())
      {
        terminals.insert(*it);
        it++;
      }
      terminals.erase(temp_production.get_left_side());
      variables.insert(temp.production.get_left_side());
    }

    bool is_variable(string variable)
    {
      return variables.find(variable) != variables.end();
    }

    bool is_terminal(string terminal)
    {
      return terminals.find(terminal) != terminals.end();
    }

};
