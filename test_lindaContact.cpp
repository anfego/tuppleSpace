#include "lindaContact.h"
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
	char data[] = "1 2 3 3 6 3 9 1 4";
	LindaContact linda(data);
	linda.print();
	return 0;
}