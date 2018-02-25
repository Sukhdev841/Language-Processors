#include <bits/stdc++.h>

using namespace std;

class s_production
{
  private:
    bool production_is_consistent;
    string left_side;
    vector<string> right_parts;
    set<string> right_variables;

    /*
    //
    //  local private functions
    //
    //
    */

    bool s_production_error()
    {
      if( production_is_consistent == false)
      {
        printf("\nError : Production is not consistent.\n/
                    Solution: Please call parse() and refresh() methods in order before using any s_production class functions./
                    \nReturning NULL\n");
        return true;
      }
      return false;
    }

  public:

    /*
    //
    //  constructors
    //
    //
    */
    s_production()
    {
      production_is_consistent = false;
    }

    s_production(string production)
    {
      extract(production);
    }

    /*
    //
    //  important functions
    //
    //
    */

    /*
                          ***************** WARNING ***********************
        extract() with string parameter will first clear the production and then construct productions
        from string
    */

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
      parse();
    }


    void parse()
    {
      if(this->left_side == "" || this->left_side==NULL)
        printf("\nError: (parse) left side of production is NULL or empty.\n");
      if(this->right_variables.size() == 0)
        printf("\nError: (parse) Nothing inserted on right side of production.\n");
      this->production_is_consistent = true;
    }

    bool refresh()
    {
      this->right_variables.clear();
      for(int i=0;i<right_parts.size();i++)
      {
        for(int j=0;j<right_parts[i].length();j++)
          this->right_variables.insert(right_parts[i][j]);
      }
    }


    /*
    //
    //  boolean functions
    //
    //
    */

    /*
            ************ ALERT ************
            PLEASE CALL refresh() before is_symbol_on_right(string)
    */
    bool is_symbol_on_right(string variable)
    {
      if(s_production_error())
        return false;
      return right_variables.find(variable) != right_variables.end();
    }

    bool is_left_variable(string variable)
    {
      if(s_production_error())
        return false;
      return left_side.compare(variable) == 0;
    }

    bool is_part_on_right(string variables)
    {
      if(s_production_error())
        return false;
      return right_parts.find(right_parts.begin(),right_parts.end(),variables) != right_parts.end();
    }

    /*
    //
    //  get methods
    //
    //
    */

    set<string> get_right_variables()
    {
      if(s_production_error())
        return NULL;
      return this.right_variables;
    }

    string get_left_side()
    {
      if(s_production_error())
        return NULL;
      return this.left_side;
    }

    string get_string_form()
    {
      if(s_production_error())
        return NULL;
      string temp = left_side + " -> ";
      for(int i=0;i<right_parts.size();i++)
      {
        temp += right_parts[i] + " | ";
      }
      return temp;
    }

    /*
    //
    //  set and insert methods
    //
    //
    */

    void set_left_side(string variable)
    {
      this->left_side = variable;
    }

    void insert_on_right(string variables)
    {
      while(variables[0]==' ')
        variables = variables.substr(1,temp.length());
      while(variables[variables.length()-1] == ' ')
        variables = variables.substr(0,variables.length()-1);
      right_parts.push_back(variables);
      istringstream g(variables);
      string temp2;
      while(getline(cin,temp2,' '))
      {
        if(temp2!= "" && temp2!=" ")
          right_variables.insert(temp2);
      }
    }

    /*
    //
    //  clear and delete functions
    //
    //
    */

    void clear_left_side()
    {
      this->left_side = "";
      this->production_is_consistent = false;
    }

    void clear_right_side()
    {
      this->right_parts.clear();
      this->right_variables.clear();
      this->production_is_consistent= false;
    }

    void clear_production()
    {
      clear_right_side();
      clear_left_side();
    }

    bool delete_(string variables)
    {
      vector<string>::iterator var_itr = find(right_parts.begin(),right_parts.end(),variables);
      if(var_itr == right_parts.end())
        return false;
      right_parts.erase(var_itr);
      this->production_is_consistent= false;
        return true;
    }

    bool remove_all_entries(string variable)
    {
      vector<string>::iterator var_itr = right_parts.begin();
      stack<vector<string>::iterator > itr_stack;
      bool flag = false;
      while(var_itr != right_parts.end())
      {
        for(int i=0;i<(*var_itr).length();i++)
        {
          if((*var_itr)[i]==variable)
          {
            flag = true;
            this->production_is_consistent= false;
            itr_stack.push_back(var_itr);
          }
        }
        var_itr++;
      }
      int count = 0;
      while(!itr_stack.empty())
      {
        right_parts.erase(itr_stack.pop()-count);
        count++;
      }
      return flag;
    }


    /*
    //
    //  printing
    //
    //
    */

    void print()
    {
      if(!s_production_error())
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

    s_context_free_grammer(string production)
    {
      add_production(production);
    }

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
