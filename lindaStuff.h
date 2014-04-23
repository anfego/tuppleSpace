#ifndef LINDASTUFF_H
#define LINDASTUFF_H

#include <stdlib.h>
#include <string.h>
#include <vector>

#include "/nfshome/rbutler/public/courses/pp6430/mpich3i/include/mpi.h"
#include "lindaContact.h"

#define PP_SUCCESS      	1
#define PP_FAIL         	-11	 // an error occurred
#define PP_EXHAUSTION   	3
#define PP_NO_MORE_WORK 	4
#define PP_HANDLE_SIZE 		6
#define PP_FINALIZE_TAG		666
#define PP_PUT_TAG			999
#define PP_RSV_TAG			555
#define PP_GET_TAG			333
#define PP_WILDCARD_TYPE	0
#define PP_DATA_NO_FOUND	0
#define PP_DATA_RESERVED	1

using namespace std;

class lindaStuff
{
private:
	struct node
	{
		int type;
		bool reserved;
		void* memory;
		int size;
		int target;
		int id;
	};
	int key;
	LindaContact lindaHandler[PP_HANDLE_SIZE];
	vector<node> myNodes;
	void printData(int);
	void printAllData();
	int numRsv;
	

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
	int 	allocate(LindaContact & putHandler);
	void 	store(void *work_unit_buf, int &index);
	bool 	reserver(LindaContact & rsvHandler);
	void 	reserver(int reserve_buf[], int handle[]);
	void 	taker(int index, void * work_unit_buf);

	void rsvRequest(MPI_Comm & RQ_COMM);
	int findEmptyHandler();
	void storeHandler(int index, LindaContact & rsqHandler);

};
#endif