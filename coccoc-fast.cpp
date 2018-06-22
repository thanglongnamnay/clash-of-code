#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <sstream>
#include <set>
#include <time.h>
using namespace std;

class myString : public string
{
public:
    myString() : string() {};
    myString(string s) : string(s) {};
    myString(int i, char c) : string(i , c) {};
    myString getThis()
    {
        return *this;
    };
    myString removePunctuation()
    {
        string resultString = "";
        const string validChars = "0123456789abcdefghijklmnopqrstuvwxyz ";
        for (int i = 0; i < getThis().length(); ++i)
        {
            if (validChars.find(getThis()[i]) < validChars.length())
                resultString += getThis()[i];
            else
                resultString += " ";
        }
        return resultString;
    };
    set<string> splitString()
    {
        int i = 0;
        set<string> newStrings;
        stringstream ss(getThis());
        string word;
        while (getline(ss, word, ' '))
        {
            if (word.length())
                newStrings.insert(word);
        }
        return newStrings;
    };
};
class FileProcessor
{
public:
    void setSource(string src)
    {
        source = src;
    }
    bool fileContain(set<string> searchWord)
    {
    	int maxLen = minMaxStringLength(searchWord, true);
    	int minLen = minMaxStringLength(searchWord, false);
        set<string> fileContent = readFileToStrings(maxLen, minLen);
        for (set<string>::iterator it = searchWord.begin(); it != searchWord.end(); ++it)
        {
            if (fileContent.find(*it) == fileContent.end()) return false;
        }
        return true;
    };
    myString source;
    set<string> readFileToStrings(int maxLen, int minLen = 1)
    {
        set<string> resultStrings;
        ifstream file(source.c_str(), ios::in);
        if (!file.is_open()) return resultStrings;
        //cout << "Checking:" << source << endl;
        while (!file.eof())
        {
            string data;
            file >> data;
            int i = 0;
            string temp = "";
            while (i < data.length())
            {
                char c = tolower(data[i]);
                if (c >= '0' && c <= '9' || c >= 'a' && c <= 'z')
                    temp += data[i];
                else if (temp.length() >= minLen && temp.length() <= maxLen)
                {
                    resultStrings.insert(temp);
            		//cout << "temp1:" << temp <<endl;
            		temp = "";
                }
                else 
            		temp = "";
                ++i;
            }
            if (temp.length() >= minLen && temp.length() <= maxLen)
            {
          		//cout << "temp3:" << temp <<endl;
                resultStrings.insert(temp);
            }
        }
        file.close();
        //for (set<string>::iterator i = resultStrings.begin(); i != resultStrings.end(); ++i)
        	//cout << *i << endl;
        return resultStrings;
    };
    int minMaxStringLength(set<string> ss, bool isMax) {
		set<string>::iterator m = ss.begin();
		for (set<string>::iterator i = ++ss.begin(); i != ss.end(); ++i) {
			if ((*i).length() > (*m).length() && isMax) 
				m = i;
			else if ((*i).length() < (*m).length() && !isMax) 
				m = i;
		}
		return (*m).length();
	};
};

class FolderProcessor
{
public:
    string standardizedPath(const string path)
    {
        const string invalidChars = "*?\"|";
        string resultPath = "";
        int i = 0;
        while (i < path.length())
        {
            if (invalidChars.find(path[i]) >= invalidChars.length())
            {
                resultPath += path[i];
            }
            ++i;
        }
        return resultPath;
    };
    bool getFilesNamesInFolder(const string path = ".", vector<string> &resultFilesNames)
    {
        string pattern = path + "\\*";
        WIN32_FIND_DATA data;
        HANDLE hFind = FindFirstFile(pattern.c_str(), &data);
        if (hFind == INVALID_HANDLE_VALUE)
        {
            return false; //path is not a folder
        }
        do
        {
            string fileName = data.cFileName;
            char c = tolower(fileName[0]);
            if (c >= '0' && c <= '9' || c >= 'a' && c <= 'z')
            {
                if (data.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
                {
                    resultFilesNames.push_back(path + "\\" + fileName);
                }
                else getFilesNamesInFolder(path + "\\" + fileName, resultFilesNames);
            }
        }
        while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    };
};

int main(int argc, char *argv[])
{
    clock_t t;
    t = clock();
    if (argv[1] == NULL)
    {
        cerr << "No search value" << endl;
        return 0;
    }
    string searchString = argv[1];
    myString searchWord(searchString);
    set<string> searchWords = searchWord.removePunctuation().splitString();
    string searchFolder = argv[2]? argv[2] : ".";
    FolderProcessor folderProcessor;
    string path = folderProcessor.standardizedPath(searchFolder);
    vector<string> filesFullNames;
    folderProcessor.getFilesNamesInFolder(path, filesFullNames);
    int namesSize = filesFullNames.size();
    FileProcessor fileProcessor;
    for (int i = 0; i < namesSize; ++i)
    {
        fileProcessor.setSource(filesFullNames[i]);
        if (fileProcessor.fileContain(searchWords))
            cout << filesFullNames[i] << endl;
    }
    t = clock() - t;
    cout << "Done in " << (float(t))/CLOCKS_PER_SEC << "s" << endl;
    return 0;
}
