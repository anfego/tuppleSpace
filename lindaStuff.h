#ifndef LINDASTUFF_H
#define LINDASTUFF_H

#include "/nfshome/rbutler/public/courses/pp6430/mpich3i/include/mpi.h"
#include <vector>

struct node
{
	int type;
	void* memory;
};

class lindaStuff
{
private:
	int type;
	bool reserved;
	int reserved_by;
	int data;
	bool any_types;
	std::vector <node> myNodes;
	std::vector <int>myTypes;
	std::vector <std::vector <int>>myPerm;
	MPI_Comm LINDA_COMM;

	// Methods
	int set_linda_comm(const MPI_Comm comm_world);

public:
	// Default constructor
	lindaStuff();
	// Constructor
	lindaStuff(const MPI_Comm comm_world);

	lindaStuff();
	~lindaStuff();
	int getType();
	bool isReserved();
	int put(int type, int size);
	void* get(int type);
	void setType(int type);

	/* data */
} lindaSpace;
#endif