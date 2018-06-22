#include <iostream>
#include <string>
#include <vector>
#include <sstream>
 
	
using namespace std;

class myString : public string {
	public:
		myString() : string() {};
		myString(string s) : string(s){};
		myString(int i, char c) : string(i , c){};
		myString getThis() {
			return *this;
		};
		myString removePunctuation() {
			string resultString = "";
			const string validChars = "0123456789abcdefghijklmnopqrstuvwxyz ";
			for (int i = 0; i < getThis().length(); ++i) {
				if (validChars.find(getThis()[i]) < validChars.length())
					resultString += getThis()[i];
				else 
					resultString += " ";
			}
			return resultString;
		};
		vector<myString> splitString() {
			int i = 0;
			vector<myString> newStrings;
			stringstream ss(getThis());
			string word;
			while (getline(ss, word, ' ')) {
				if (word.length())
					newStrings.push_back(word);
			}
			return newStrings;
		};
};

int main(int argc, char* argv[])
{
	char *c = "drg.ghj???";
	myString s(100, 'c');
	myString ms(s);
	cout << ms.removePunctuation().splitString()[0];
	return 0;
}
