#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
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

class FileProcessor {
	public:
		FileProcessor(const myString src) {
			source = src;
		};
		bool fileContain(myString searchString) {
			vector<myString> fileContent = readFileToStrings();
			vector<myString> searchWord = searchString.removePunctuation().splitString();
			int numberOfWordsContain = 0;
			for (int i = 0; i < searchWord.size(); ++i)
				for (int j = 0; j < fileContent.size(); ++j) {
					if (fileContent[j] == searchWord[i]) {
						numberOfWordsContain++;
						break;
					}
				}
			if (numberOfWordsContain == searchWord.size()) return true;
			return false;
		};
	private:
		myString source;
		vector<myString> readFileToStrings() {
			vector<myString> resultStrings;
			myString data;
			ifstream file(source.c_str(), ios::in);
			while (!file.eof()) {
				file >> data;
				stringstream ss(data.removePunctuation());
				while(getline(ss, data, ' ')){
					resultStrings.push_back(data);
				}
			}
			file.close();
			return resultStrings;
		};
};

class FolderProcessor {
	public:
		string standardizedPath(const string path) {
			const string invalidChars = "*?\"|";
			string resultPath = "";
			int i = 0;
			while (i < path.length()) {
				if (invalidChars.find(path[i]) >= invalidChars.length()) {
					resultPath += path[i];
				}
				i++;
			}
			return resultPath;
		};
		vector<string> getFilesNamesInFolder(const string source = ".") {
			vector<string> resultFilesNames;
			const string validName = "0123456789abcdefghijklmnopqrstuvwxyz";
			const DWORD MAX_SIZE_SCAN = 1024;
			int length = 0;
			string path = standardizedPath(source);
			string pattern = path + "\\*";
		    WIN32_FIND_DATA data;
		    HANDLE hFind;
		    if ((hFind = FindFirstFile(pattern.c_str(), &data)) == INVALID_HANDLE_VALUE) {
		    	return vector<string>(); //path is not a folder
    		}
	        do {
				if (data.nFileSizeLow > MAX_SIZE_SCAN*1024 || data.nFileSizeHigh> 0) {
					continue;
				}
	        	string fileName = data.cFileName;
	            if (validName.find(char(tolower(fileName[0]))) < validName.length()){
	            	vector<string> temp = getFilesNamesInFolder(path + "\\" + fileName);
	            	if (temp.empty()){
	           	 		resultFilesNames.push_back(path + "\\" + fileName);
	           	 		
						#ifdef DEBUG
					   		cout << path + "\\" + fileName;
							system("cls");
						#endif
		            } else if (temp[0] != "."){
            			resultFilesNames.insert(resultFilesNames.end(), temp.begin(), temp.end());
            		}
	            } 
	        } while (FindNextFile(hFind, &data) != 0);
	        FindClose(hFind);
	        if (!resultFilesNames.empty()) //something in the folder
				return resultFilesNames;
				
			resultFilesNames.push_back("."); //empty folder
			return resultFilesNames;
		};
};

int main(int argc, char *argv[]) {
	if (argv[1] == NULL) {
		cerr << "No search value" << endl;
		return 0;
	}
	string searchString = argv[1];
	string searchFolder = argv[2]? argv[2] : ".";
	FolderProcessor folderProcessor;
	vector<string> filesFullNames = folderProcessor.getFilesNamesInFolder(searchFolder);
	for (int i = 0; i < filesFullNames.size(); ++i) {
		FileProcessor fileProcessor(filesFullNames[i]);
		if (fileProcessor.fileContain(searchString))
	 		cout << filesFullNames[i] << endl;
	}
	return 0;
}
