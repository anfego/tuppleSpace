

#include "lindaContact.h"

using namespace std;

LindaContact::LindaContact()
{
	rq_rank = 0;
	data_id = 0;
	size = 0;
	used = 0
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
	target_rank = -1;
	location_rank = -1;
	LindaContact::addWorkType(type_of_work);
	LindaContact::addServer(server);
	used = 1; 
	
}

LindaContact::LindaContact(int myRank, int target, int size_of_work, int type_of_work)
{
	rq_rank = myRank;
	target_rank = target;
	location_rank = -1;
	size = size_of_work;
	data_id = 0;
	LindaContact::addWorkType(type_of_work);
}
LindaContact::LindaContact(int myRank)
{
	rq_rank = myRank;
	target_rank = -1;
	location_rank = -1;
	size = 0;
	data_id = 0;
}


int LindaContact::addWorkType(int type_of_work)
{
	types.push_back(type_of_work);
	return types.size();
}
int LindaContact::numWorkTypes()
{
	return types.size();
}
void LindaContact::clcWorkTypes()
{
	types.clear();
}

bool LindaContact::addServer(int server)
{
	if( !isServerVisited(server))
	{
		rq_servers.push_back(server);
		return true;
	}
	return false;
}

bool LindaContact::isServerVisited(int server)
{
	std::vector<int>::iterator it;
	it = find(rq_servers.begin(), rq_servers.end(),server);
	if (it != rq_servers.end())
	{
		return true;
	}
	return false;
}
int LindaContact::numServerVisited()
{
	return rq_servers.size();
}

int LindaContact::serializer(char * buf)
{
			//	 rR    id   sz   lR  tR nT T&S
	sprintf(buf, "%d   %d   %d   %d   %d   ", rq_rank, data_id, size, location_rank, target_rank);
	serializeTypeVector(buf+strlen(buf));
	serializeServerVector(buf+strlen(buf));

	return strlen(buf);
}

int LindaContact::deserializer(char * serial)
{
	int n_types;
	int n_servers;
	
	char temp[100];
	memset(temp,'\0',100*sizeof(char));
	//			  rR  id sz lR tR nT T&S
	sscanf(serial,"%d %d %d %d %d %d %99[0-9 ]s", &rq_rank, &data_id, &size, &location_rank, &target_rank, &n_types, temp);
	// printf("to deserialize %s\n", temp );
	types = deserializeVector(n_types,temp);
	
	
	sscanf(temp,"%d %99[0-9 ]s", &n_servers, temp);
	// printf("to deserialize %s\n", temp );
	rq_servers = deserializeVector(n_servers,temp);
	
	return 0;
}

void LindaContact::print()
{
	char buf[HANDLER_SIZE];
	memset(buf,'\0',HANDLER_SIZE*sizeof(char));
	serializer(buf);
	printf("\tLinda Handler: rK  Id  Sz  lR  tR   Ty   Sr\n");
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
	memset(buf,'\0',strlen(buf)*sizeof(char));
	memcpy(buf,serial,strlen(serial)*sizeof(char));
	
	my_vector.push_back(element);
	// printf("element pushed %d\n",element);
	if (n > 1)
	{
		rec_Vector = deserializeVector(n-1,buf);
		total_Vector.reserve( my_vector.size() + rec_Vector.size() );
		total_Vector.insert( total_Vector.end() , my_vector.begin(), my_vector.end() );
		total_Vector.insert( total_Vector.end() , rec_Vector.begin(), rec_Vector.end() );
		my_vector = total_Vector;

	}
	return my_vector;
}