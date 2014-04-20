

#include "lindaContact.h"

using namespace std;

LindaContact::LindaContact()
{
	rq_rank = 0;
	dataID = 0;
	size = 0;
	LindaContact:: addWorkType(0);
	LindaContact:: addServer(0);
}
LindaContact::LindaContact(char * serial)
{
	deserializer(serial);
}
LindaContact::LindaContact(int myRank,int dataID, int size_of_work, int type_of_work, int server)
{	
	rq_rank = myRank;
	dataID = dataID;
	size = size_of_work;
	LindaContact:: addWorkType(type_of_work);
	LindaContact:: addServer(server);
	
}

int LindaContact::serializer(char * buf)
{
	sprintf(buf, "%d %d %d %d",rq_rank,dataID,size,types[0]);
	return strlen(buf);
}

int LindaContact::addWorkType(int type_of_work)
{
	types.push_back(type_of_work);
	return types.size();
}

int LindaContact::addServer(int server)
{
	rq_servers.push_back(server);
	return rq_servers.size();
}

bool LindaContact::isServerVisited(int server)
{
	bool visited = false;
	std::vector<int>::iterator it;
	it = find(rq_servers.begin(), rq_servers.end(),server);
	if (it == rq_servers.end())
	{
		visited = true;
	}
	return visited;
}

int LindaContact::deserializer(char * serial)
{
	int type;
	sscanf(serial,"%d %d %d %d\0", &rq_rank, &dataID, &size, &type);
	addWorkType(type);
	return 0;
}

void LindaContact::print()
{
	char buf[HANDLER_SIZE];
	serializer(buf);
	printf("\tLinda Handler: %s\n",buf);
}
