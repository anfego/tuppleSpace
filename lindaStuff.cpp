#include "lindaStuff.h"

using namespace lindaStuff;
using namespace std;

lindaStuff(arguments)
{
	any_types = false;
}

~lindaStuff()
{
	
}
void lindaStuff::setType(int type)
{//myPerm is vector of vectors
if(type != 0)
	{	
		myTypes.push_back(type);
		vector<int> temp;
		temp.push_back(type);
		myPerm.push_back(temp);
		int oldSize = myPerm.size();
		for(int i=0; i < oldSize - 1; ++i)
		{
			myPerm.push_back(myPerm[i]);//copy all existing vectors
		}

		int newSize = myPerm.size();
		//start right after pure new element and add new element to all copied before
		for(int i=oldSize; i < newSize; ++i)
		{
			(myPerm[i]).push_back(type);//copy all existing
		}
	}
	else
		any_types = true;
}


int lindaStuff::put(int type, int size)
{
	node tempNode;
	tempNode.type = type;
	int resp = malloc(tempNode.memory, size);
	if (resp == 0)//if no space
	{
		return -1;
	}
	else
		myNodes.push_back(tempNode);
	return 0;
}

void * lindaStuff::get(int type)
{
	if(any_types) //then just take first one
	{
		if(myNodes.size() != 0)
		{	
			void* ptr = (myNodes[0]).memory;
			myNodes[i].erase();//note thgat memory is still allocated
			return ptr;
		}
	}
	else
	for (int i = 0; i < myNodes.size(); ++i)
	{
		if( (myNodes[i]).type == type)
		{
			void* ptr = (myNodes[i]).memory;
			myNodes[i].erase();//note thgat memory is still allocated
			return ptr;
		}
	}


	return NULL;
}



