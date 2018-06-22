#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <windows.h>
#include <sstream>

 
using namespace std;

class StringProcessor {
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
		};
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
		};
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
		};
};

class FileProcessor {
	public:
		FileProcessor(const string src) {
			source = src;
		};
		bool fileContain(const string searchString) {
			StringProcessor sp;
			return sp.stringVectorContain(readFileToStrings(), searchString);
		};
		private:
		string source;
		vector<string> readFileToStrings() {
			vector<string> resultStrings;
			string data;
			StringProcessor sp;
			ifstream file(source.c_str(), ios::in);
			while (!(file.eof() && file.tellg()<1024)) {
				file >> data;
				stringstream ss(sp.standardizedString(data));
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
		bool getFilesNamesInFolder(const string source = ".", queue<string> &filesNames) {
			const string validName = "0123456789abcdefghijklmnopqrstuvwxyz";
			const DWORD MAX_SIZE_SCAN = 1000;
			int length = 0;
			string path = standardizedPath(source);
			string pattern = path + "\\*";
		    WIN32_FIND_DATA data;
		    HANDLE hFind;
		    if ((hFind = FindFirstFile(pattern.c_str(), &data)) == INVALID_HANDLE_VALUE) {
		    	return false; //path is not a folder
			}
		    do { //path is folder
		    	if (data.nFileSizeLow > MAX_SIZE_SCAN*1024 || data.nFileSizeHigh	> 0) {
					continue;
		    	}
		    	string fileName = data.cFileName;
		        if (validName.find(char(tolower(fileName[0]))) < validName.length()){
		        	WIN32_FIND_DATA nextData;
		        	if (FindFirstFile((path + "\\" + fileName + "\\*").c_str(), &nextData) == INVALID_HANDLE_VALUE){
		       	 		filesNames.push(path + "\\" + fileName);
		       	 		//cout << path + "\\" + fileName << endl;
		            } else if (nextData.cFileName != "."){
		    			getFilesNamesInFolder(path + "\\" + fileName, filesNames);
		    		}
		        } 
		    } while (FindNextFile(hFind, &data) != 0);
		    FindClose(hFind);
		    return true;
		};
};

typedef struct {
	bool flag;
	string searchString;
	queue<string> names;
} threadType;

DWORD WINAPI showContainedFiles(LPVOID voidPointer) {
	threadType &fan = *((threadType*)voidPointer);
	while (!(fan.flag == true && fan.names.empty())) {
		//cout << fan.names.front() << endl;
		FileProcessor fileProcessor(fan.names.front());
		if (fileProcessor.fileContain(fan.searchString)){
			cout << "choosed:" << fan.names.front() << endl;
		}
		fan.names.pop();
	}
	return 0;
}

int main(int argc, char *argv[]) {
	if (argv[1] == NULL) {
		cerr << "No search value" << endl;
		return 0;
	}
	string searchFolder = (argv[2] != NULL)? argv[2] : ".";
	FolderProcessor folderProcessor;
	threadType fan;
	fan.flag = false;
	fan.searchString = argv[1];
	fan.names = queue<string>();
	DWORD showContainedFilesID;
	HANDLE threadHandle;
	threadHandle = CreateThread(0, 0, showContainedFiles, &fan, 0, &showContainedFilesID);
	fan.flag = folderProcessor.getFilesNamesInFolder(searchFolder, fan.names);
	WaitForMultipleObjects(1, &threadHandle, 1, INFINITE);
	return 0;
}
