#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <windows.h>
 
using namespace std;

bool read_directory(string name, vector<string> &v)
{
	const string validName = "0123456789abcdefghijklmnopqrstuvwxyz";
	int length = 0;
	string path = name + "\\*";
    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile(path.c_str(), &data)) == INVALID_HANDLE_VALUE) return false;
    if ((hFind = FindFirstFile(path.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
        	string fileName = data.cFileName;
            if (validName.find(fileName[0]) < validName.length()){
            	if (!read_directory(name + "\\" + fileName, v)){
           	 		v.push_back(name + "\\" + fileName);
	            }
            } 
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
    return true;
}


int main(int argc, char *argv[]) {
	vector<string> fileNames;
	string data;
	cout << argv[1] << endl;
	
	cout << read_directory(".\\sub1", fileNames) << endl;
	for (int i = 0; i < fileNames.size(); i++){
		fstream readStream;
		cout << fileNames.at(i) << endl;
		readStream.open(fileNames.at(i).c_str(), ios::in);
		while (!readStream.eof()){
			readStream >> data;
			cout << data << " ";
		}
		cout << endl;
		readStream.close();
	}
	return 0;
}
