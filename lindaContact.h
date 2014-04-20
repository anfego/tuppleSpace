#ifndef LINDA_CONTACT_H
#define LINDA_CONTACT_H

#include <string>

#define HANDLER_SIZE 100

using namespace std; 

class LindaContact
{

public:
	LindaContact();
	LindaContact(char *);
	LindaContact(int myRank, int dataID, int size_of_work, int type_of_work);
	~LindaContact(){};

	int serializer(char *);
	int deserializer(char *);
	void print();

	/* data */
	int ownerRank;
	int dataID;
	int size;
	int type;
};



#endif