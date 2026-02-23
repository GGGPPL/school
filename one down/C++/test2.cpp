/* getchar example : typewriter */
#include <iostream>
#include<vector>
#include <string>

#define fir(d) for (int i = 0; i < d; i++)

using namespace std;

char* string2char(string str)
{
	char p[100];
	fir(str.length())
	{
		p[i] = str[i];
	}
	p[str.length()] = '\0';
	return p;
}

int main ()
{
  string p="abc";
  cout << string2char(p) << endl;
  return 0;
}