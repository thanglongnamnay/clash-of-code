#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <sstream>

using namespace std;

class StringProcessor
{
public:
    bool stringVectorContain(vector<string> src, const string searchString)
    {
        string standardizedSearchString = standardizedString(searchString);
        vector<string> searchWord = splitString(standardizedSearchString);
        int numberOfWordsContain = 0;
        for (int i = 0; i < searchWord.size(); ++i)
            for (int j = 0; j < src.size(); ++j)
            {
                if (src[j] == searchWord[i])
                {
                    numberOfWordsContain++;
                    break;
                }
            }
        if (numberOfWordsContain == searchWord.size()) return true;
        return false;
    };
    string standardizedString (const string originalString)
    {
        string resultString = "";
        const string validChars = "0123456789abcdefghijklmnopqrstuvwxyz ";
        for (int i = 0; i < originalString.length(); ++i)
        {
            if (validChars.find(originalString[i]) < validChars.length())
                resultString += originalString[i];
            else
                resultString += " ";
        }
        return resultString;
    };
    vector<string> splitString(const string originalString)
    {
        int i = 0;
        vector<string> newStrings;
        stringstream ss(originalString);
        string word;
        while (getline(ss, word, ' '))
        {
            if (word.length())
                newStrings.push_back(word);
        }
        return newStrings;
    };
};

class FileProcessor
{
public:
    FileProcessor(const string src)
    {
        source = src;
    };
    bool fileContain(const string searchString)
    {
        StringProcessor sp;
        return sp.stringVectorContain(readFileToStrings(), searchString);
    };
private:
    string source;
    vector<string> readFileToStrings()
    {
        vector<string> resultStrings;
        string data;
        StringProcessor sp;
        ifstream file(source.c_str(), ios::in);
        while (!file.eof())
        {
            file >> data;
            stringstream ss(sp.standardizedString(data));
            while(getline(ss, data, ' '))
            {
                resultStrings.push_back(data);
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
            i++;
        }
        return resultPath;
    };
    vector<string> getFilesNamesInFolder(const string source = ".")
    {
        vector<string> resultFilesNames;
        const string validName = "0123456789abcdefghijklmnopqrstuvwxyz";
        const DWORD MAX_SIZE_SCAN = 1024;
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
            if (data.nFileSizeLow > MAX_SIZE_SCAN*1024 || data.nFileSizeHigh> 0)
            {
                continue;
            }
            string fileName = data.cFileName;
            if (validName.find(char(tolower(fileName[0]))) < validName.length())
            {
                vector<string> temp = getFilesNamesInFolder(path + "\\" + fileName);
                if (temp.empty())
                {
                    resultFilesNames.push_back(path + "\\" + fileName);

#ifdef DEBUG
                    cout << path + "\\" + fileName;
                    system("cls");
#endif
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
    if (argv[1] == NULL)
    {
        cerr << "No search value" << endl;
        return 0;
    }
    string searchString = argv[1];
    string searchFolder = argv[2]? argv[2] : ".";
    FolderProcessor folderProcessor;
    vector<string> filesFullNames = folderProcessor.getFilesNamesInFolder(searchFolder);
    for (int i = 0; i < filesFullNames.size(); ++i)
    {
        FileProcessor fileProcessor(filesFullNames[i]);
        if (fileProcessor.fileContain(searchString))
            cout << filesFullNames[i] << endl;
    }
    return 0;
}
