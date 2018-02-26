#include <bits/stdc++.h>
#include "s_string.h"

using namespace std;

class s_production
{
public:
    string left_side;
    vector<string> right_parts;
    set<string> right_variables;

    /**
     * costructors
     */

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

    /**
     * IMPORTANT methods
     *
     * 1) extract(string)
     * 2) parse()    [ checks consistency ]
     * 3) refresh()
     */

   /**
    * [extract description]
    *
    * saves the $production in own data structure
    *
    *
    *
    *     ***************** ***************************** WARNING **************************************************
          extract() with string parameter will first clear the production and then construct productions from string
          **********************************************************************************************************
    */

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
    }

    /**
     * [refresh description]
     *
     *  refreshes local set of $right_variables and $right_parts
     *  called everytime internally when change happens to production
     *  1) when something is inserted on right
     *  2) when something is removed or deleted
     *  3) when left side is altered
     */

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

    /**
     * boolean functions
     *
     * 1) bool is_symbol_on_right(string)
     * 2) bool is_left_variable(string)
     * 3) bool is_part_on_right(string)
     * 4) bool are_same(production)
     */

    /*
            ********************* CAUTION ************************
            PLEASE CALL refresh() before is_symbol_on_right(string)
            ******************************************************
    */

    /**
     * [is_symbol_on_right description]
     *
     * $this = A -> D B | c
     * is_symbol_on_right(D), is_symbol_on_right(c), is_symbol_on_right(B) returns true
     * is_symbol_on_right(A), is_symbol_on_right(x), is_symbol_on_right(D B) returns false
     */
    bool is_symbol_on_right(string variable)
    {
      return right_variables.find(variable) != right_variables.end();
    }

    /**
     * [is_left_variable description]
     *
     * $this = A -> D B | c
     * only is_left_variable(A) returns true
     */

    bool is_left_variable(string variable)
    {
      return left_side.compare(variable) == 0;
    }

    /**
     * [is_part_on_right description]
     *
     * $this = A -> A B | C D
     *
     * is_part_on_right(A B),is_part_on_right(C D) returns true
     */

    bool is_part_on_right(string variables)
    {
      return (find(right_parts.begin(),right_parts.end(),variables) != right_parts.end());
    }

    /**
     * [are_same description]
     *
     * compares $this with $production2 and returns true if same otherwise false
     *
     * $this = A -> A b | B c
     * $production2 = A -> B c | A b
     * returns : true
     */

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

    /**
     * get methods
     *
     * 1) string get_string_form()
     */

     /**
      * [get_string_form description]
      * returns string form of production
      */
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

    /**
     * set - insert methods
     *
     * 1) insert_on_right (string)
     */

    void insert_on_right(string variables)
    {
      while(variables[0]==' ')
        variables = variables.substr(1,variables.length());
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
      refresh();
    }

    /**
     * clear-delete methods
     *
     * 1) bool delete_(string)
     * 2) bool remove_part(string)
     */

    /**
     * [delete_ description]
     * remove part 'variables' on right if exists
     * A -> A B | a b
     * delete_(A B) will result to production A -> a b
     */
    bool delete_(string variables)
    {
      vector<string>::iterator var_itr = find(right_parts.begin(),right_parts.end(),variables);
      if(var_itr == right_parts.end())
        return false;
      right_parts.erase(var_itr);
      refresh();
        return true;
    }

    /**
     * special methods
     *
     * 1) replace_with(s_production)
     * 2) replace(string,string)
     */

    /**
     * [replace_with description]   -- recursive
     *
     * $this = A -> B C
     * $p = B -> a | b
     * result : $this = A -> a C | b C
     */

    void replace_with(s_production p)
    {
      vector<string> parts_of_p = p.right_parts;
      s_production new_production;
      new_production.left_side = this->left_side;
      bool change_happen = false;

      for(int i=0;i<right_parts.size();i++)
      {
        vector<string> parts = get_parts(right_parts[i],' ');
        int index = find(parts.begin(),parts.end(),p.left_side) - parts.begin();
        if(index == parts.size())
        {
          new_production.insert_on_right(right_parts[i]);
          continue;
        }
        change_happen = true;
        for(int j=0;j<parts_of_p.size();j++)
        {
          string temp_right_side = "";
          for(int k=0;k<index;k++)
            temp_right_side += parts[k] + " ";
          temp_right_side += parts_of_p[j] + " ";
          for(int k = index+1;k<parts.size();k++)
            temp_right_side += parts[k]+" ";
          new_production.insert_on_right(temp_right_side);
        }
      }
      new_production.refresh();
      if(change_happen)
        new_production.replace_with(p);   // recursive implementation
      *this = new_production;
    }

    /**
     * [remove_immediate_left_recursion description]
     *
     * $this = A -> A a | b
     * result: return vector contains following productions
     * A -> b A1
     * A1 -> a A | #                 [ '#' corresponds to NULL or empty string ]
     */

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
      p2.insert_on_right("#");
      vector<s_production> vec;
      vec.push_back(p1);
      vec.push_back(p2);
      return vec;
    }

    /**
     * [left_factor description]      -- recursive
     * $this = A -> a B | a C
     * returns vector of following productions
     * A -> a A1
     * A1 -> B | C
     */

    vector<s_production> left_factor()
    {
      vector<s_production> vec;
      map<string,vector<string> > common_prefix_strings; // map<  prefix  , vector containing all parts with prefix >

      // put all right_parts in map according to first leftmost variable
      for(int i=0;i<right_parts.size();i++)
      {
        common_prefix_strings[get_ith_part(right_parts[i],1,' ')].push_back(right_parts[i]);
      }

      // now process each vector in map separately
      // and store max length prefex for each entry in map in a new vector

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
          // checking if all strings have prefix of length 'length'
          string temp_prefix = local_right_parts[0].substr(length-1,local_right_parts[0].length());
          bool same_prefix_upto_length = true;
          for(int i=1;i<local_right_parts.size();i++)
          {
            if( length >= local_right_parts[i].length() || local_right_parts[i].substr(0,length) != temp_prefix)
            {
              // not all strings have not same common prefix upto length 'length'
              // stop here
              same_prefix_upto_length = false;
              break;
            }
          }

          if(same_prefix_upto_length == false)
          {
            // stop here and save the max_common_prefix length for the group
            l = length - 1;
            break;
          }
        }

        // make productions accordingly

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

    /**
     * print production METHODS
     *
     * 1) print()
     */

    void print()
    {
        cout<<get_string_form();
    }

    /**
     * operators overloaded
     * 1) =
     */

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
//
// class s_context_free_grammer
// {
//   /**
//    *********************************************
//    *                  IMPORTANT
//    *********************************************
//    *
//    *  [ USE refresh_grammer() METHOD AFTER ANY UPDATE OR CHANGE IN GRAMMER ]
//    */
//
//   private:
//     vector<s_production> productions;
//     set<string> variables;
//     set<string> terminals;
//   public:
//
//     /**
//      * constructors
//      *
//      * 1) s_context_free_grammer()
//      * 2) s_context_free_grammer(string)
//      * 3) s_context_free_grammer(const s_context_free_grammer&)
//      */
//
//     s_context_free_grammer()
//     {}
//
//     s_context_free_grammer(string production)
//     {
//       add_production(production);
//     }
//
//     s_context_free_grammer(const s_context_free_grammer& g)
//     {
//       *this  = g;
//     }
//
//     /**
//      * data set methods
//      *
//      * 1) add_production(string)
//      * 2) add_production(s_production)
//      */
//
//     void add_production(string production)
//     {
//       s_production temp_production(production);
//       productions.push_back(temp_production);
//       set<string> right_variables = temp_production.get_right_variables();
//       set<string>::iterator it = right_variables.begin();
//       while(it != right_variables.end())
//       {
//         terminals.insert(*it);
//         it++;
//       }
//       terminals.erase(temp_production.get_left_side());
//       variables.insert(temp_production.get_left_side());
//     }
//
//     void add_production(s_production p)
//     {
//       string temp = p.get_string_form();
//       add_production(temp);
//     }
//
//     /**
//      *  data get methods
//      *
//      * 1) set<string> get_terminals()
//      * 2) set<string> get_variables()
//      * 3) vector<s_production> get_productions()
//      */
//
//     set<string> get_terminals()
//     {
//       return this->terminals;
//     }
//
//     set<string> get_variables()
//     {
//       return this->variables;
//     }
//
//     vector<s_production> get_productions()
//     {
//       return this->productions;
//     }
//
//     /**
//      * data delete methods
//      *
//      * 1) remove_production(string)
//      * 2) remove_production(s_production)
//      * 3) remove_productions_starting_with(string)
//      */
//
//     void remove_production(string production)
//     {
//       s_production p(production);
//       remove_production(p);
//     }
//
//     void remove_production(s_production p)
//     {
//       for(int i=0;i<productions.size();i++)
//       {
//         if(productions[i].are_same(p))
//         {
//           productions.erase(productions.begin()+i);
//           break;
//         }
//       }
//       refresh_grammer();
//     }
//
//     void remove_productions_starting_with(string variable)
//     {
//       for(int i=0;i<productions.size();i++)
//       {
//         if(productions[i].get_left_side() == variable)
//         {
//           productions.erase(productions.begin()+i);
//           break;
//         }
//       }
//     }
//
//     /**
//      * condition check methods
//      *
//      * 1) bool is_variable(string)
//      * 2) bool is_terminal(string)
//      */
//
//
//     bool is_a_variable(string variable)
//     {
//       return variables.find(variable) != variables.end();
//     }
//
//     bool is_a_terminal(string terminal)
//     {
//       return terminals.find(terminal) != terminals.end();
//     }
//
//     /**
//      *  OTHER METHODS
//      *
//      *  1) refresh_grammer()
//      */
//
//     void refresh_grammer()
//     {
//       terminals.clear();
//       variables.clear();
//       for(int i=0;i<productions.size();i++)
//       {
//         set<string> right_variables = productions[i].get_right_variables();
//         set<string>::iterator it = right_variables.begin();
//         while(it != right_variables.end())
//         {
//           terminals.insert(*it);
//           it++;
//         }
//         terminals.erase(productions[i].get_left_side());
//         variables.insert(productions[i].get_left_side());
//       }
//     }
//
//     /**
//      *  printing grammer
//      *
//      * 1) print() [ refresh_grammer() recommended ]
//      */
//
//     void print()
//     {
//       cout<<"\n******************************************************\n";
//       for(int i=0;i<productions.size();i++)
//       {
//         cout<<"\n"<<i+1<<") ";
//         productions[i].print();
//       }
//       cout<<"\n\n******************************************************\n";
//       cout<<"variables : [ ";
//       set<string>::iterator var_itr = variables.begin();
//       while( var_itr != variables.end())
//       {
//         cout<<*var_itr<<" ";
//         var_itr++;
//       }
//       cout<<" ]\n";
//
//       cout<<"Terminals : [ ";
//       set<string>:: iterator ter_itr = terminals.begin();
//       while(ter_itr != terminals.end())
//       {
//         cout<<*ter_itr<<" ";
//         ter_itr++;
//       }
//       cout<<" ]\n";
//
//       cout<<"******************************************************\n";
//     }
//
//     /**
//      * operator overloaded
//      *
//      * 1) =
//      */
//
//     s_context_free_grammer& operator = (const s_context_free_grammer& g)
//     {
//       if(this != &g)
//       {
//         this->productions = g.productions;
//         this->variables = g.variables;
//         this->terminals = g.terminals;
//       }
//       return *this;
//     }
//
// };

// s_context_free_grammer left_factor(s_context_free_grammer grammer)
// {
//   vector<s_production> productions = grammer.get_productions();
//   for(int i=0;i<productions.size();i++)
//   {
//     set<string> right_parts_of_production = productions[i].get_right_parts();
//     set<string>::iterator itr = right_parts_of_production.begin();
//     while( itr != right_parts_of_production.end() )
//     {
//       itr++;
//     }
//   }
// }
