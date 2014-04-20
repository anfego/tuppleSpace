#ifndef LINDA_CONTACT_H
#define LINDA_CONTACT_H

#include <string>

using namespace std; 

class LindaContact
{
	int ownerRank;
	int dataID;
	int size;
	int type;

public:
	LindaContact();
	LindaContact(int myRank, int dataID, int size_of_work, int type_of_work);
	~LindaContact();

	int serializer(char *);
	int deserializer(char *);
	void print();

	/* data */
};



#endif