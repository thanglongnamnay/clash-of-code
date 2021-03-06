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
    set<myString> splitString()
    {
        int i = 0;
        set<myString> newStrings;
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
    void setSource(string src) {
    	source = src;
    }
    bool fileContain(set<myString> searchWord)
    {
        set<myString> fileContent = readFileToStrings();
        int fileContentSize = fileContent.size();
        int numberOfWordsContain = 0;
        for (set<myString>::iterator i = searchWord.begin(); i != searchWord.end(); ++i)
        {
            if (fileContent.find(*i) == fileContent.end()) return false;
        }
        return true;
    };
private:
    myString source;
    set<myString> readFileToStrings()
    {
        set<myString> resultStrings;
        string data;
        const string validChars = "0123456789abcdefghijklmnopqrstuvwxyz";
        const int validCharsLen = validChars.length();
        string temp;
        ifstream file(source.c_str(), ios::in);
        if (!file.is_open()) return resultStrings;
        while (!file.eof())
        {
            file >> data;
            int i = 0;
            temp = "";
            while (i < data.length())
            {
                if (validChars.find(data[i]) < validCharsLen)
                {
                    temp += data[i];
                }
                else
                {
                    resultStrings.insert(temp);
                    temp = "";
                }
                ++i;
            }
            if (temp.length())
            {
                resultStrings.insert(temp);
            }
        }
        file.close();
        return resultStrings;
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
    vector<string> getFilesNamesInFolder(const string source = ".")
    {
//        const DWORD MAX_SIZE_SCAN = 1024;
        vector<string> resultFilesNames;
        const string validName = "0123456789abcdefghijklmnopqrstuvwxyz";
        int length = 0;
        string path = standardizedPath(source);
        string pattern = path + "\\*";
        WIN32_FIND_DATA data;
        HANDLE hFind;
        if ((hFind = FindFirstFile(pattern.c_str(), &data)) == INVALID_HANDLE_VALUE)
        {
            return vector<string>(); //path is not a folder
        }
        do
        {
            /*if (data.nFileSizeLow > MAX_SIZE_SCAN*1024 || data.nFileSizeHigh> 0)
            {
                continue;
            }*/
            string fileName = data.cFileName;
            if (validName.find(char(tolower(fileName[0]))) < validName.length())
            {
                vector<string> temp = getFilesNamesInFolder(path + "\\" + fileName);
                if (temp.empty())
                {
                    resultFilesNames.push_back(path + "\\" + fileName);
                }
                else if (temp[0] != ".")
                {
                    resultFilesNames.insert(resultFilesNames.end(), temp.begin(), temp.end());
                }
            }
        }
        while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
        if (!resultFilesNames.empty()) //something in the folder
            return resultFilesNames;
        resultFilesNames.push_back("."); //empty folder
        return resultFilesNames;
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
	    set<myString> searchWords = searchWord.removePunctuation().splitString();
	    string searchFolder = argv[2]? argv[2] : ".";
	    FolderProcessor folderProcessor;
	    vector<string> filesFullNames = folderProcessor.getFilesNamesInFolder(searchFolder);
	    int namesSize = filesFullNames.size();
	    FileProcessor fileProcessor;
	    for (int i = 0; i < namesSize; ++i)
	    {
	        fileProcessor.setSource(filesFullNames[i]);
	        if (fileProcessor.fileContain(searchWords));
	            //cout << filesFullNames[i] << endl;
	    }
    t = clock() - t;
    cout << "Done in " << (float(t))/CLOCKS_PER_SEC << "s" << endl;
    return 0;
}
