#include <iostream>
#include <set>
#include <map>
#include <time.h>
#include <windows.h>
#include <vector>
using namespace std;

bool contain(string s, char c) {
	int i = 0;
	int len = s.length();
	cout << (s[0] == c) << endl;
	while (i < len && s[i] != c)
		++i;
	if (i == len) return false;
	return true;
}
int main ()
{
	clock_t t;
	t = clock();
	
	vector<int> vi;
	vi[0] = 10;
	vi[1] = 20;
	cout << vi[0];
    
	t = clock() - t;
    cout << "Done in " << (float(t))/CLOCKS_PER_SEC << "s" << endl;
}
