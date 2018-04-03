#include <bits/stdc++.h>

using namespace std;

vector<string> get_parts(string, char);
string remove_ith_part(string, int, char);
string get_ith_part(string, int, char);
void reverse(char, int);
char* itoa(int, char*, int);
vector<string> to_vector( set<string>);
string itos(int);
int stoi(string);
string remove_extra_spaces(string);
set<string> merge_(set<string>,set<string>);
void print(set<string>);
void print(vector<set<string> > );
string print(stack<string> );
int find_index_of(string str,string part,char delimeter);
string put_after_index(string,int,char);

string put_after_index(string str,string part,int index,char delimeter)
{
  string result = "";
  vector<string> parts = get_parts(str,delimeter);
  for(int i=0;i<parts.size();i++)
  {
    if(i == index)
      result += part+ delimeter;
      result += parts[i] + delimeter;
  }
  return result.substr(0,result.length()-1);
}

string put_at_index(string str,string part,int index,char delimeter)
{
  string result = "";
  vector<string> parts = get_parts(str,delimeter);
  for(int i=0;i<parts.size();i++)
  {
    if(i+1 == index)
      result += part+ delimeter;
    else
      result += parts[i] + delimeter;
  }
  return result.substr(0,result.length()-1);
}

vector<string> get_parts(string str,char delimeter)
{
  istringstream f(str);
  vector<string> parts;
  string temp;
  while(getline(f,temp,delimeter))
  {
    parts.push_back(temp);
  }
  return parts;
}

int find_index_of(string str,string part,char delimeter)
{
  vector<string> parts = get_parts(str,delimeter);
  for(int i=0;i<parts.size();i++)
  {
    if(parts[i] == part)
      return i+1;
  }
  return -1;
}

string remove_ith_part(string str,int i,char delimeter)
{
  istringstream f(str);
  string result = "";
  string temp;
  int count = 1;
  while(getline(f,temp,delimeter))
  {
    if(count != i)
      result += temp + " ";
    count++;
  }
  result = result.substr(0,result.length()-1);
  return result;
}

string remove_last_part(string str,char delimeter)
{
  vector<string> parts = get_parts(str,delimeter);
  string result = "";
  for(int i=0;i<parts.size()-1;i++)
    result += parts[i] + " ";
  result = result.substr(0,result.size()-1);
  return result;
}

string get_ith_part(string str,int i, char delimeter)
{
  istringstream f(str);
  string temp;
  int count = 1;
  while(getline(f,temp,delimeter))
  {
    if(count == i)
      return temp;
    count++;
  }
  return "";
}

string get_last_part(string str,char delimeter)
{
  istringstream f(str);
  string temp;
  string result = "";
  while(getline(f,temp,delimeter))
  {
    result = temp;
  }
  return result;
}

// from geeksforgeeks
//

void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}

char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

string itos(int num)
{
  char* str = new char[100];
  itoa(num,str,10);
  return string(str);
}

 int stoi(string num)
 {
   return atoi(num.c_str());
 }

string remove_extra_spaces(string str)
{
  string temp = "";
  bool flag = false;
  for(int i=0;i<str.length();i++)
  {
    if(str[i] == ' ' && flag)
      continue;
    if(str[i] == ' ' && ! flag)
    {
      temp += ' ';
      flag = true;
      continue;
    }
    temp += str[i];
    flag = false;
  }
  while(temp[0]==' ')
    temp = temp.substr(1,temp.length());
  while(temp[temp.length()-1]==' ')
    temp = temp.substr(0,temp.length()-1);
  return temp;
}

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
    cout<<i+1<<".\t";
		print(s[i]);
    cout<<endl;
	}

}

string print(stack<string> s)
{
	if(s.empty())
		return "";
	string x = s.top();
	s.pop();
	return x+print(s);
}
