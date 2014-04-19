/*
Author: 	Anfego
Date:		04/11/14
Class: 		CSCI 6430
Professor:	Dr. Buttler
University: MTSU
Desciption:	
	This program is an implementation of a MPI tupple space
	Support for:
		PP_Init()
		PP_Put()
		PP_Get()
		PP_Reserve()
		PP_Finalize()



*/
#include <vector>          // stack
#include <utility>
#include <pthread.h>
#include <iostream>
#include <stack>

#include "/nfshome/rbutler/public/courses/pp6430/mpich3i/include/mpi.h"
#include "pp.h"
#include "lindaStuff.h"

using namespace std;


/*<sumary>
	</sumary>*/
lindaStuff lindaSpace;

/*<sumary>
    int num_user_types;
    Any valid integer can be a user type.
	Wildcards will be described below.
    int user_types[PP_MAX_USER_TYPES];
    int server_flag;  // I begin to execute server code if true
    Discussion:
	The user should do MPI_Init and MPI_Finalize.
	</sumary>*/
int PP_Init(int num_user_types, int * user_types, int * am_server_flag)
{
	int server = *am_server_flag;
	int my_inter_rank;
	int my_side_rank;
	int my_world_rank;
	int world_size;
	int my_side_size;
	int inter_size;
	int other_side_size;
	int other_side_leader;
	
	MPI_Comm_dup(MPI_COMM_WORLD, &(lindaSpace.WORLD_COMM_DUP));
	MPI_Comm_rank(lindaSpace.WORLD_COMM_DUP,&my_world_rank);
	MPI_Comm_size(lindaSpace.WORLD_COMM_DUP,&world_size);
	
	MPI_Comm_split(MPI_COMM_WORLD, *am_server_flag, 0,&(lindaSpace.MY_SIDE_COMM));
	MPI_Comm_rank(lindaSpace.MY_SIDE_COMM,&my_side_rank);
	MPI_Comm_size(lindaSpace.MY_SIDE_COMM,&my_side_size);

	char * work_unit_buf = (char *) malloc(work_unit_size);

	// store usefull information
	lindaSpace.my_world_rank = my_world_rank;
	lindaSpace.my_side_rank = my_side_rank;
	
	lindaSpace.world_size = world_size;
	lindaSpace.my_side_size = my_side_size;
	if (server ==0)
	{
		lindaSpace.other_side_leader = my_side_size; 		// assing as leader of servers the next
															// rank after app ranks.
	}
	else
	{
		lindaSpace.other_side_leader = 0; 					// Leader of app is Rank 0;
	}
	MPI_Intercomm_create(
		lindaSpace.MY_SIDE_COMM,
		0,
		lindaSpace.WORLD_COMM_DUP,
		lindaSpace.other_side_leader,
		666,
		&(lindaSpace.INTER_COMM));

	MPI_Comm_rank(lindaSpace.INTER_COMM,&my_inter_rank);
	MPI_Comm_size(lindaSpace.INTER_COMM,&inter_size);
	MPI_Comm_remote_size(lindaSpace.INTER_COMM,&other_side_size);
	lindaSpace.other_side_size = other_side_size;


	if(server == 0)
	{
		return PP_SUCCESS;
	}

	vector<int> uTypes;
	// create stack for user types 
	int done = 0;
	int mpi_flag = 0;
	int type_rec;
	int size; //size of coming data
	MPI_Status status;

	
	while(!done)
	{
		// checks if there is a "to finish"
		MPI_Iprobe(lindaSpace.other_side_leader, 666, lindaSpace.INTER_COMM, &mpi_flag, &status);
		if (mpi_flag == 1)		// if true there is a message, PP_Finalize
		{
			MPI_Recv(&done, 1, MPI_INT, lindaSpace.other_side_leader, 666, lindaSpace.INTER_COMM, &status);
		}
		else if( mpi_flag == 2 )//received PUT
		{
			memset(work_unit_buf, '\0', work_unit_size);
			size = 0;
			type_rec = 0;
			//end of clear
			MPI_Recv(&type_rec, 1, MPI_INT, lindaSpace.other_side_leader, 666, lindaSpace.INTER_COMM, &status);
			// if(type_rec == good rec) - receive
			MPI_Recv(&size, 1, MPI_INT, lindaSpace.other_side_leader, 666, lindaSpace.INTER_COMM, &status);
			MPI_Recv(work_unit_buf, 1, size, lindaSpace.other_side_leader, 666, lindaSpace.INTER_COMM, &status);
			resp = store();
			MPI_Recv(resp, 1, MPI_INT, lindaSpace.other_side_leader, 666, lindaSpace.INTER_COMM, &status);

		}
	}
	printf("tuppleSpace Exit\n");

	return PP_SUCCESS;	
}
/*<sumary>
	</sumary>*/

int PP_Finalize()
{
	MPI_Status status;
	//Send "To finish" signal
	int done = 1;
	int my_side_rank;
	int my_inter_rank;

	// wait until all app's get until this point
	MPI_Barrier(lindaSpace.MY_SIDE_COMM);
	if(lindaSpace.am_i_server() == false)
	{
		if(lindaSpace.my_side_rank == 0) 			// if Im rank 0 in app side do Bcast 
		{
			// Sends to every server a "to finish" signal
			for(int i = 0; i < lindaSpace.other_side_size; i++)
			{
				MPI_Send(&done,1, MPI_INT, i, 666, lindaSpace.INTER_COMM);
			}
		}

	}	
	// printf("Im out: %d\n",my_inter_rank);
	
	MPI_Barrier(lindaSpace.WORLD_COMM_DUP);			//Wait for all processes hit this point

	// free LindaSpace communicators
	MPI_Comm_free(&(lindaSpace.INTER_COMM));
	MPI_Comm_free(&(lindaSpace.MY_SIDE_COMM));
	MPI_Comm_free(&(lindaSpace.WORLD_COMM_DUP));
	if (lindaSpace.my_world_rank == 0)
	{
		printf("PP_Finalize = PP_SUCCESS\n");
	}
	return PP_SUCCESS;
}
/*<sumary>
	</sumary>*/

int PP_Put()
{
	//generate random
	// send to server type
	//send to server size
	//send to server data
	return PP_SUCCESS;
}
/*<sumary>
	</sumary>*/

int PP_Reserve()
{
	return PP_SUCCESS;
}
/*<sumary>
	</sumary>*/

int PP_Get()
{
	return PP_SUCCESS;
}
/*<sumary>
	</sumary>*/

int PP_Set_problem_done()
{
	return PP_SUCCESS;
}
/*<sumary>
	</sumary>*/

int PP_Abort()
{
	return PP_SUCCESS;
}

