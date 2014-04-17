#ifndef LINDASTUFF_H
#define LINDASTUFF_H

#include "/nfshome/rbutler/public/courses/pp6430/mpich3i/include/mpi.h"


class lindaStuff
{
private:
	int type;
	bool reserved;
	int reserved_by;
	int data;
	MPI_Comm LINDA_COMM;
	
	// Methods
	int set_linda_comm(const MPI_Comm comm_world);

public:
	// Default constructor
	lindaStuff();
	// Constructor
	lindaStuff(const MPI_Comm comm_world);
	~lindaStuff();
	int getType();
	bool isReserved();
	
	

	/* data */
};
#endif