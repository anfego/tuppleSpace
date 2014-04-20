#include "lindaContact.h"
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
	//              rR  Id  Sz  lR  tR  Ty     Sr
	char data[] = "\t1  2   3   3   2   2 2 1  4 4 5 3 5";
	LindaContact linda(data);
	linda.print();
	if (linda.isServerVisited(4))
	{
		printf("Server In list\n");
	}
	return 0;
}