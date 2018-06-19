#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <sstream>
using namespace std;

string standardizedString (const string originalString) {
	string resultString = "";
	const string validChars = "0123456789abcdefghijklmnopqrstuvwxyz ";
	for (int i = 0; i < originalString.length(); ++i) {
		if (validChars.find(originalString[i]) < validChars.length())
			resultString += originalString[i];
	}
	return resultString;
}

vector<string> splitString(const string originalString) {
	int i = 0;
	vector<string> newStrings;
	stringstream ss(originalString);
	string word;
	while (getline(ss, word, ' ')) {
		cout << "\'" <<word <<"\'" << endl;
		if (word.length())
			newStrings.push_back(word);
	}
	return newStrings;
}

int main() {
	cout << splitString(standardizedString("bookx.sdfgdfg. ! . . . ? asdf "))[1];
	
	return 0;
}