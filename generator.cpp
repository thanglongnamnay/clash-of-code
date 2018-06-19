#include <iostream> 
#include <fstream>   
#include <cstring>
#include <string>

using namespace std;
string toString(const int n) {	
	char *intStr;
	intStr = new char;
	itoa(n, intStr, 10);
	return string("adsf");
}
string randomString(int len) {
	const string testString[] = {"hello", "books", "book", "many", "thing", "things"};
	const string space = " ";
	string resultString = "";
	for (int i = 0; i < len; ++i) {
		int r = rand() % 6;
		resultString += testString[r];
		int p = rand() % 3;
		if (p > 0) resultString += ",";
		int k = rand() % 6;
		if (k > 2) resultString += space;
	}
	return resultString;
}
int main () {
	srand(time(NULL));
	for (int i = 0; i < 150; i++){
		ofstream file;
		cout << "i = " << i << " - ";
		string filePath = "d:\\clash of code test\\generatedFile" + toString(i);
		file.open(filePath.c_str(), ios::out);
		string rs = randomString(5);
		cout << rs << endl;
		file << rs;
		file.close();
	}
  return 0;
}
