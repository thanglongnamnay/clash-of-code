// ios::eof example
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <cstring>
using namespace std;
int main () {
	for(int i = 0; i < 10; i++)
		for (int j = 0; j < 6; j++){
			cout << "i = " << i << " j = " << j <<endl;
			if(j == 3) break;
		}
  return 0;
}