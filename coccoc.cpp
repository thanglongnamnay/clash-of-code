#include <iostream>
#include <fstream>
#include <set>
#include <deque>
#include <windows.h>
#include <sstream>
#include <thread>
using namespace std;

class myString : public string
{
public:
    myString() : string() {};
    myString(string s) : string(s) {};
    myString(char* c) : string(c) {};
    myString(int i, char c) : string(i , c) {};
    myString getMyString()
    {
        return *this;
    };
    myString removePunctuation()
    {
        string resultString = "";
        myString validChars = string("0123456789abcdefghijklmnopqrstuvwxyz ");
        for (int i = 0; i < getMyString().length(); ++i)
        {
            if (validChars.contain(tolower(getMyString()[i])))
                resultString += getMyString()[i];
            else
                resultString += " ";
        }
        return resultString;
    };
    bool contain(char c)
    {
        int i = 0;
        myString s = getMyString();
        int len = s.length();
        while (i < len && s[i] != c)
            ++i;
        if (i == len) return false;
        return true;
    }
    set<string> splitString()
    {
        int i = 0;
        set<string> newStrings;
        stringstream ss(getMyString());
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
    bool fileContain(set<string> searchWord, string src)
    {
        source = src;
        pair<int, int> minmax = stringLengthInterval(searchWord);
        int minLen = minmax.first;
        int maxLen = minmax.second;
        set<string> fileContent = stringsInFile(maxLen, minLen);
        for (set<string>::iterator it = searchWord.begin(); it != searchWord.end(); ++it)
        {
            if (fileContent.find(*it) == fileContent.end()) return false; //not contain 1 of search words
        }
        return true;
    };
private:
    myString source;
    set<string> stringsInFile(int maxLen, int minLen) //filter the strings lengths bewtween min and max
    {
        set<string> resultStrings;
        ifstream file(source.c_str(), ios::in);
        if (!file.is_open()) return resultStrings;
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
                    temp = "";
                }
                else
                    temp = "";
                ++i;
            }
            if (temp.length() >= minLen && temp.length() <= maxLen)
            {
                resultStrings.insert(temp);
            }
        }
        file.close();
        return resultStrings;
    };
    pair<int, int> stringLengthInterval(set<string> ss) //get min and max lengths of a string
    {
        set<string>::iterator max = ss.begin();
        set<string>::iterator min = max;
        for (set<string>::iterator i = ++ss.begin(); i != ss.end(); ++i)
        {
            if ((*i).length() > (*max).length())
                max = i;
            if ((*i).length() < (*min).length())
                min = i;
        }
        return pair<int, int>((*min).length(), (*max).length());
    };
};

class FolderProcessor
{
public:
    string standardizedPath(const string path) //delete some useless chars in path
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
    bool getFilesNamesInFolder(string path, deque<string> &resultFilesNames)
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

void processAndPrintResult(set<string> searchWords, deque<string> &filesFullNames, bool isBack)
{
    FileProcessor fileProcessor;
    while(filesFullNames.size())
    {
        string name;
        if (isBack)
        {
            name = filesFullNames.back();
            filesFullNames.pop_back();
        }
        else
        {
            name = filesFullNames.front();
            filesFullNames.pop_front();
        }
        if(fileProcessor.fileContain(searchWords, name))
        {
            cout << name + "\n";
        }
    }
}

int main(int argc, char *argv[])
{
    if (argv[1] == NULL)
    {
        cerr << "No search value" << endl;
        return 0;
    }
    myString searchString(argv[1]);
    set<string> searchWords = searchString.removePunctuation().splitString();
    
    string searchFolder = argv[2]? argv[2] : ".";
    FolderProcessor folderProcessor;
    string path = folderProcessor.standardizedPath(searchFolder);
    deque<string> filesFullNames;
    folderProcessor.getFilesNamesInFolder(path, filesFullNames);
    int namesSize = filesFullNames.size();
    thread back(processAndPrintResult, searchWords, ref(filesFullNames), true);
    thread front(processAndPrintResult, searchWords, ref(filesFullNames), false);
    back.join();
    front.join();
    return 0;
}
