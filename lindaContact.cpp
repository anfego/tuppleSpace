

#include "lindaContact.h"

using namespace std;

LindaContact::LindaContact()
{
	rq_rank = 0;
	data_id = 0;
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
	data_id = dataID;
	size = size_of_work;
	LindaContact::addWorkType(type_of_work);
	LindaContact::addServer(server);
	
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
int LindaContact::serializer(char * buf)
{
	sprintf(buf, "%d %d %d ", rq_rank, data_id, size);
	serializeTypeVector(buf+strlen(buf));
	serializeServerVector(buf+strlen(buf));

	return strlen(buf);
}

int LindaContact::deserializer(char * serial)
{
	int n_types;
	char temp[100];
	sscanf(serial,"%d %d %d %d %99[0-9 ]s", &rq_rank, &data_id, &size, &n_types, temp);
	printf("to deserialize %s\n", temp );
	types = deserializeVector(n_types,temp);
	return 0;
}

void LindaContact::print()
{
	char buf[HANDLER_SIZE];
	memset(buf,'\0',HANDLER_SIZE*sizeof(char));
	serializer(buf);
	printf("\tLinda Handler: %s\n",buf);
}

int LindaContact::serializeTypeVector(char * buf)
{
	int len = 0;
	char serial [100];
	memset(serial,'\0',100*sizeof(char));
	// add first the number of types
	sprintf(serial,"%d ",types.size());
	for (std::vector<int>::iterator i = types.begin(); i != types.end(); ++i)
	{
		len = strlen(serial);
		sprintf(serial+len,"%d ",*i);
	}
	len = strlen(serial);
	memcpy(buf,serial,len*sizeof(char));
	return len;

}
int LindaContact::serializeServerVector(char * buf)
{
	int len = 0;
	char serial[100];
	memset(serial,'\0',100*sizeof(char));
	// add first the number of servers
	sprintf(serial,"%d ",rq_servers.size());
	for (std::vector<int>::iterator i = rq_servers.begin(); i != rq_servers.end(); ++i)
	{
		len = strlen(serial);
		sprintf(serial+len,"%d ",*i);
	}
	len = strlen(serial);
	memcpy(buf,serial,len*sizeof(char));
	return len;

}
std::vector<int> LindaContact::deserializeVector(int n, char *buf)
{
	std::vector<int> my_vector;
	std::vector<int> rec_Vector;
	std::vector<int> total_Vector;
	int element;
	char serial[100];
	memset(serial,'\0',100*sizeof(char));
	sscanf(buf,"%d %100[0-9 ]s", &element, serial);
	my_vector.push_back(element);
	printf("element pushed %d\n",element);
	if (n > 1)
	{
		rec_Vector = deserializeVector(n-1,serial);
		total_Vector.reserve( my_vector.size() + rec_Vector.size() );
		total_Vector.insert( total_Vector.end() , my_vector.begin(), my_vector.end() );
		total_Vector.insert( total_Vector.end() , rec_Vector.begin(), rec_Vector.end() );
		my_vector = total_Vector;

	}
	return my_vector;
}