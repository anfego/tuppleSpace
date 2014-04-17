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
	int inter_rank;
	MPI_Comm comm_servers, comm_world_dup;
	
	MPI_Comm_dup(MPI_COMM_WORLD, &(lindaSpace.INTER_COMM));
	
	MPI_Comm_split(MPI_COMM_WORLD, *am_server_flag, 0,&(lindaSpace.LINDA_COMM));

	MPI_Comm_rank(lindaSpace.INTER_COMM,&my_inter_rank);
	
	/******Create new intercomm*******/
	MPI_Intercomm_create( lindaSpace.LINDA_COMM/*should contatin local comm*/ , 0/*usually 0*/, MPI_COMM_WORLD/*peer*/, 0/*usually 0*/,  0/*int tag*/,  &lindaSpace.INTER_NEW/*new comm will be stored here*/ );	!
	/******get rank*******/
	printf("Creating intercomm in rank: %d\n",inter_rank);
	
	MPI_Comm_rank( lindaSpace.INTER_NEW, &inter_rank );
	printf("my inter_rank ==>>: %d\n",inter_rank);

	// lindaStuff lindaSpace(comm_world_dup, comm_servers, server);
	printf("my_inter_rank: %d\n",my_inter_rank);
	// get user types
	if(!*am_server_flag)
	{
		return PP_SUCCESS;
	}
	lindaSpace.set_as_server();
	printf("I am a server %d\n", lindaSpace.am_i_server());
	vector<int> uTypes;
	// create stack for user types 
	int done = 0;
	int mpi_flag = 0;
	MPI_Status status;
	for (int i = 0; i < num_user_types; ++i)
	{
		//insert_type(num_user_types[i]);//realized in Ivan1 // if zero would met, then special flag would be triggered in linda class
	}
	
	while(!done)
	{
		MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG, lindaSpace.INTER_COMM, &mpi_flag, &status);
		if (mpi_flag == 1)		// if true there is a message, PP_Finalize
			{
				printf("Server DONE :  %d\n",done);
				MPI_Recv(&done, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, lindaSpace.INTER_COMM, &status);
			}
	}
	printf("Server Exit\n");

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
	MPI_Comm newintercomm;
	int my_inter_rank;

	// wait until all app's get until this point
	MPI_Barrier(lindaSpace.LINDA_COMM);
	if(lindaSpace.am_i_server() == false)
	{
		MPI_Comm_rank(lindaSpace.LINDA_COMM,&my_side_rank);
		printf("my_app_rank: %d\n",my_side_rank);
		MPI_Comm_rank(lindaSpace.INTER_COMM,&my_inter_rank);
		printf("PP_Finalize: my_inter_rank: %d\n",my_inter_rank);
		MPI_Bcast(&done,1, MPI_INT, my_inter_rank, lindaSpace.INTER_COMM);
		if(my_side_rank == 0) 			// if Im rank 0 in app side do Bcast 
		{
			printf("END servers!\n");
			// MPI_Send(&done,1, MPI_INT, 0, MPI_ANY_TAG, lindaSpace.INTER_COMM);
		}
		else
		{
			done = 0;		// waits until rank 0 on app side send the "done" signal
			printf("wait for signal app side\n");
			MPI_Recv(&done, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, lindaSpace.INTER_COMM, &status);

		}
	}	
	printf("Im out: %d\n",my_inter_rank);
	// MPI_Barrier(lindaSpace.INTER_COMM);
	return PP_SUCCESS;
}
/*<sumary>
	</sumary>*/

int PP_Put()
{
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

