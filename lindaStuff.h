#ifndef LINDASTUFF_H
#define LINDASTUFF_H

#include <stdlib.h>
#include <string.h>
#include <vector>

#include "/nfshome/rbutler/public/courses/pp6430/mpich3i/include/mpi.h"
#include "lindaContact.h"

using namespace std;

class lindaStuff
{
private:
	int type;
	bool reserved;
	int reserved_by;
	int data;
	//TODO: Add target suport
	struct node
	{
		int type;
		bool reserved;
		void* memory;
		int size;
		int index;
		int target;
	};
	int index;
	vector<node> myNodes;
	void printData(int);
	void printAllData();
	

public:
	// Default constructor
	int server;
	
	int my_world_rank;
	int my_side_rank;
	

	int world_size;
	int my_side_size;
	int other_side_size;

	int other_side_leader;

	MPI_Comm MY_SIDE_COMM;
	MPI_Comm WORLD_COMM_DUP;
	MPI_Comm INTER_COMM;

	lindaStuff();
	// Constructor
	lindaStuff( int am_server );
	// Destructor
	~lindaStuff();
	int 	getType();
	bool 	isReserved();
	int 	get_comm_inter(MPI_Comm *comm_in);
	int 	get_comm_linda(MPI_Comm *comm_in);
	int 	am_i_server();
	void 	set_as_server();
	int 	allocate(int size, int type);
	void 	store(void *work_unit_buf, int &index);
	void 	reserver(int reserve_buf[], int handle[]);
	bool 	reserver(LindaContact & rsvHandler);
	void 	taker(int index, void * work_unit_buf);
	int 	getLocalIndex(int index);

};
#endif