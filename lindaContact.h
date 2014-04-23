#ifndef LINDA_CONTACT_H
#define LINDA_CONTACT_H

#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#define HANDLER_SIZE 1000

using namespace std; 

class LindaContact
{

public:
	LindaContact();
	LindaContact(char *);
	LindaContact(int myRank);
	LindaContact(int myRank, int dataID, int size_of_work, int type_of_work, int server);
	LindaContact(int myRank, int target, int size_of_work, int type_of_work);
	~LindaContact(){};

	int serializer(char *);
	int deserializer(char *);
	void print();

	int addWorkType(int type_of_work);
	int numWorkTypes();
	void clcWorkTypes();

	bool addServer(int server);
	bool isServerVisited(int server);
	int numServerVisited();
	int serializeTypeVector(char *);
	int serializeServerVector(char *);
	std::vector<int> deserializeVector(int, char *);
	/* data */
	int rq_rank;
	int data_id;
	int size;
	int location_rank;
	int target_rank;
	std::vector<int> types;
	std::vector<int> rq_servers;
	int used;
};



#endif