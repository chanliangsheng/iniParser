#include <iostream>
#include <iniParser.h>

using namespace std;
using namespace iniParser;

int main()
{
	auto a = Parseini("D:/vs_project/iniParser/ini_example.txt");

	cout << a.str() << endl;


	return 0;
}
