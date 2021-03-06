
#include <stdio.h>
#include <string.h>
#include "lindaContact.h"

using namespace std;

LindaContact::LindaContact()
{
	ownerRank = 0;
	dataID = 0;
	size = 0;
	type = 0;
}
LindaContact::LindaContact(char * serial)
{
	deserializer(serial);
}
LindaContact::LindaContact(int myRank, int dataID, int size_of_work, int type_of_work)
{	
	ownerRank = myRank;
	dataID = dataID;
	size = size_of_work;
	type = type_of_work;
}

int LindaContact::serializer(char * buf)
{
	sprintf(buf, "%d %d %d %d",ownerRank,dataID,size,type);
	return strlen(buf);
}
int LindaContact::deserializer(char * serial)
{
	sscanf(serial,"%d %d %d %d\0", &ownerRank, &dataID, &size, &type);
	return 0;
}

void LindaContact::print()
{
	char buf[HANDLER_SIZE];
	serializer(buf);
	printf("\tLinda Handler: %s\n",buf);
}