#include <bits/stdc++.h>

using namespace std;

vector<string> get_parts(string, char);
string remove_ith_part(string, int, char);
string get_ith_part(string, int, char);
void reverse(char, int);
char* itoa(int, char*, int);
string itos(int);

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
