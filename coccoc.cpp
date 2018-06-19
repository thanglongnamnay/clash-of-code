#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <sstream>
 
using namespace std;

class stringProcessor {
	public:
		bool stringVectorContain(vector<string> src, const string searchString) {
			string standardizedSearchString = standardizedString(searchString);
			vector<string> searchWord = splitString(standardizedSearchString);
			int numberOfWordsContain = 0;
			for (int i = 0; i < searchWord.size(); ++i)
				for (int j = 0; j < src.size(); ++j) {
					if (src.at(j) == searchWord.at(i)) {
						numberOfWordsContain++;
						break;
					}
				}
			if (numberOfWordsContain == searchWord.size()) return true;
			return false;
		}
		string standardizedString (const string originalString) {
			string resultString = "";
			const string validChars = "0123456789abcdefghijklmnopqrstuvwxyz ";
			for (int i = 0; i < originalString.length(); ++i) {
				if (validChars.find(originalString[i]) < validChars.length())
					resultString += originalString[i];
				else 
					resultString += " ";
			}
			return resultString;
		}
		vector<string> splitString(const string originalString) {
			int i = 0;
			vector<string> newStrings;
			stringstream ss(originalString);
			string word;
			while (getline(ss, word, ' ')) {
				if (word.length())
					newStrings.push_back(word);
			}
			return newStrings;
		}
};

class fileProcessor {
	public:
		vector<string> readFileToStrings(const string source = ".") {
			vector<string> resultStrings;
			string data;
			stringProcessor sp;
			ifstream file(source.c_str(), ios::in);
			while (!file.eof()) {
				file >> data;
				stringstream ss(sp.standardizedString(data));
				while(getline(ss, data, ' ')){
					resultStrings.push_back(data);
				}
			}
			file.close();
			return resultStrings;
		}
		vector<string> getFilesNamesInFolder(const string source = ".") {
			vector<string> resultFilesNames;
			const string validName = "0123456789abcdefghijklmnopqrstuvwxyz";
			int length = 0;
			string path = source + "\\*";
		    WIN32_FIND_DATA data;
		    HANDLE hFind;
		    if ((hFind = FindFirstFile(path.c_str(), &data)) == INVALID_HANDLE_VALUE) 
				return vector<string>();
	        do {
	        	string fileName = data.cFileName;
	            if (validName.find(char(tolower(fileName[0]))) < validName.length()){
	            	vector<string> temp = getFilesNamesInFolder(source + "\\" + fileName);
	            	if (temp.size()	== 0){
	           	 		resultFilesNames.push_back(source + "\\" + fileName);
		            } else {
            			resultFilesNames.insert(resultFilesNames.end(), temp.begin(), temp.end());
            		}
	            } 
	        } while (FindNextFile(hFind, &data) != 0);
	        FindClose(hFind);
		    return resultFilesNames;
		}
};

int main(int argc, char *argv[]) {
	fileProcessor fp;
	stringProcessor sp;
	vector<string> filesNames;
	if (argv[1] == NULL) {
		cerr << "No search value" << endl;
		return 0;
	}
	string searchString = argv[1];
	string searchFolder = (argv[2] != NULL)? argv[2] : ".";
	
	filesNames = fp.getFilesNamesInFolder(searchFolder);
	for (int i = 0; i < filesNames.size(); ++i){
		bool fileContain = sp.stringVectorContain(fp.readFileToStrings(filesNames.at(i)), searchString);
		if (fileContain) cout << filesNames.at(i) << endl;
	}
	return 0;
}
