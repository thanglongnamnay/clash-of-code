#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
 
using namespace std;

class fileProcessor {
	public:
		vector<string> readFileToStrings(string source = ".") {
			vector<string> resultStrings;
			string data;
			ifstream file(source.c_str(), ios::in);
			while (!file.eof()) {
				file >> data;
				resultStrings.push_back(data);
			}
			file.close();
			return resultStrings;
		}
		vector<string> getFilesNamesInFolder(string source = ".") {
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
	            if (validName.find(fileName[0]) < validName.length()){
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

class stringProcessor {
	public:
		bool stringVectorContain(vector<string> src, string searchString) {
			vector<string> searchWord = splitString(searchString);
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
	private:
		vector<string> splitString(string originalString) {
			int i = 0;
			vector<string> newStrings;
			while (originalString.length()) {
				newStrings.push_back(originalString.substr(0, originalString.find(" ")));
				if (originalString.find(" ") <= originalString.length())
					originalString = originalString.substr(originalString.find(" ") + 1);
				else originalString = "";
			}
			return newStrings;
		}
};

int main(int argc, char *argv[]) {
	fileProcessor fp;
	stringProcessor sp;
	vector<string> filesNames;
	string searchString = argv[1];
	string searchFolder = (argv[2] != NULL)? argv[2] : ".";
	cout << searchFolder << endl;
	filesNames = fp.getFilesNamesInFolder(searchFolder);
	for (int i = 0; i < filesNames.size(); ++i){
		bool fileContain = sp.stringVectorContain(fp.readFileToStrings(filesNames.at(i)), searchString);
		if (fileContain) cout << filesNames.at(i) << endl;
	}
	return 0;
}
