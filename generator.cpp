#include <iostream> 
#include <fstream>   
#include <cstring>
#include <string>

using namespace std;
string to_string(int n) {	
	char *intStr;
	itoa(n,intStr,10);
	return string(intStr);
}
string randomString(int len) {
	const string testString[] = {"hello", "books", "book", "many", "thing", "things"};
	const string space = " ";
	string resultString = "";
	for (int i = 0; i < len; ++i) {
		int r = rand() % 6;
		resultString += testString[r];
		r = rand() % 6;
		if (r > 2) resultString += space;
	}
	return resultString;
}
int main () {
	srand(time(NULL));
	for (int i = 0; i < 100; i++){
		ofstream file;
		string filePath = "d:\\clash of code test\\generatedFile" + to_string(i);
		file.open(filePath.c_str(), ios::out);
		cout << "i = " << i <<endl;
		file << randomString(5);
		file.close();
	}
  return 0;
}
