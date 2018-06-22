#include <iostream>
#include <set>
#include <map>
#include <time.h>
#include <windows.h>
using namespace std;

string randomString(int len) {
	string resultString = "";
	const string validChars = "0123456789abcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < len; ++i) {
		resultString += validChars[rand()%validChars.length()];
	}
	return resultString;
}
int maxStringLength(set<string> ss) {
	set<string>::iterator max = ss.begin();
	for (set<string>::iterator i = ++ss.begin(); i != ss.end(); ++i) {
		if ((*i).length() > (*max).length()) max = i;
	}
	return (*max).length();
}
int main ()
{
	clock_t t;
	const int max = 100;
	set<string> ss;
	map<string, int> ms;
	Sleep(100);
	t = clock();
	for (int i = 0; i < max; ++i) {
		ss.insert(randomString(rand()%10+1));
	}
	
	for (set<string>::iterator i = ss.begin(); i != ss.end(); ++i) {
		cout << *i << endl;
	}
	cout << maxStringLength(ss);
    t = clock() - t;
    cout << "Done in " << (float(t))/CLOCKS_PER_SEC << "s" << endl;
}
