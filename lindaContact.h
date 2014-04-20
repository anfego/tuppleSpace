#ifndef LINDA_CONTACT_H
#define LINDA_CONTACT_H

#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#define HANDLER_SIZE 100

using namespace std; 

class LindaContact
{

public:
	LindaContact();
	LindaContact(char *);
	LindaContact(int myRank, int dataID, int size_of_work, int type_of_work, int server);
	~LindaContact(){};

	int serializer(char *);
	int deserializer(char *);
	void print();

	int addWorkType(int type_of_work);
	int addServer(int server);
	bool isServerVisited(int server);

	/* data */
	int rq_rank;
	int data_id;
	int size;
	std::vector<int> types;
	std::vector<int> rq_servers;
};



#endif