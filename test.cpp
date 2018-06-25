// thread example
#include <iostream>       // std::cout
#include <thread>         // std::thread
 using namespace std;
class Test{
public:
	bool condition(string s) {
		if (s.length() > 4) return true;
		return false;
	}
	
	bool con2(bool(&con)(string s)){
		if(con("asdf")) return 1;
		return 0;
	}
	
	void run(){
		con2(condition);
	}
};
int main() 
{
	Test t;
	t.run();
  return 0;
}