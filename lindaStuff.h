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
	
	// Methods

public:
	// Default constructor
	int server;
	MPI_Comm LINDA_COMM;
	MPI_Comm INTER_COMM;
	lindaStuff();
	// Constructor
	lindaStuff(const MPI_Comm comm_world, const MPI_Comm comm_inter, int am_server);
	// Destructor
	~lindaStuff();
	int set_linda_comm(const MPI_Comm comm_world, const MPI_Comm comm_inter);
	int getType();
	bool isReserved();
	int get_comm_inter(MPI_Comm *comm_in);
	int get_comm_linda(MPI_Comm *comm_in);
	int am_i_server();
	void set_as_server();


	
	

	/* data */
};
#endif