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
#include <random>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#include "/nfshome/rbutler/public/courses/pp6430/mpich3i/include/mpi.h"
#include "pp.h"
#include "lindaStuff.h"
#include "lindaContact.h"

using namespace std;

	struct pass_struct
	{
		int size;
		int num_types;
		int local_index;
	}__attribute__ ((packed));

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
int PP_Init(int num_user_types, int * user_types, int am_server_flag)
{
	srand(time(NULL));
	int server = am_server_flag;
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
	
	MPI_Comm_split(MPI_COMM_WORLD, am_server_flag, 0,&(lindaSpace.MY_SIDE_COMM));
	MPI_Comm_rank(lindaSpace.MY_SIDE_COMM,&my_side_rank);
	MPI_Comm_size(lindaSpace.MY_SIDE_COMM,&my_side_size);
	//work_unit_size IS NOT PASSED !!!!!!!!!!!!!!!!!!!!!
	int work_unit_size = 1000000;

	void * work_unit_buf = malloc(work_unit_size);

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
	pass_struct put_struct;
	MPI_Status status;
	int temp_buf[2];
	int reserve_buf[num_user_types+1];//+1because first is number of elements is array
	int handle[4];
	int index;
	int resp;
	int ack;
	char rq_buf[HANDLER_SIZE];
	memset(rq_buf,'\0',HANDLER_SIZE*sizeof(char));
	while(!done)
	{
		// checks if there is a "to finish"
		mpi_flag = 0;
		MPI_Iprobe(lindaSpace.other_side_leader, PP_FINALIZE_TAG, lindaSpace.INTER_COMM, &mpi_flag, &status);
		if (mpi_flag == 1)		// if true there is a message, PP_Finalize
		{
			MPI_Recv(&done, 1, MPI_INT, lindaSpace.other_side_leader, PP_FINALIZE_TAG, lindaSpace.INTER_COMM, &status);

		}
		mpi_flag = 0;
		MPI_Iprobe(MPI_ANY_SOURCE, PP_PUT_TAG, lindaSpace.INTER_COMM, &mpi_flag, &status);
		if( mpi_flag == 1 )//received PUT
		{
			cout << "I am " << my_side_rank<<" got a put request\n";
			MPI_Recv(rq_buf, HANDLER_SIZE, MPI_CHAR, MPI_ANY_SOURCE, PP_PUT_TAG, lindaSpace.INTER_COMM, &status);
			
			LindaContact putHandler(rq_buf);
			putHandler.print();
			
			// if(type_rec == good rec) - receive
			// resp is index in vector, never can be less than zero
			/*int allocate(int &size, int &type)*/
			resp = lindaSpace.allocate(putHandler.size, putHandler.types[0], putHandler.data_id);

			MPI_Send(&resp,1, MPI_INT, putHandler.rq_rank, PP_PUT_TAG, lindaSpace.INTER_COMM);
			//get the datapp
			if (resp != PP_FAIL)
			{
				MPI_Recv(work_unit_buf, putHandler.size, MPI_CHAR, putHandler.rq_rank, PP_PUT_TAG, lindaSpace.INTER_COMM, &status);
				//put in allocated space
				lindaSpace.store(work_unit_buf, resp);
				// resp = PP_SUCCESS;
				// MPI_Send(&resp,1, MPI_INT, putHandler.rq_rank, PP_PUT_TAG, lindaSpace.INTER_COMM);
			}
		}
		mpi_flag = 0;
		MPI_Iprobe(MPI_ANY_SOURCE, PP_RSV_TAG, lindaSpace.INTER_COMM, &mpi_flag, &status);
		if( mpi_flag == 1)//received reserve
		{
			cout << "I am " << my_side_rank<<" got a reserve request\n";
			MPI_Recv(&rq_buf, HANDLER_SIZE, MPI_CHAR, MPI_ANY_SOURCE, PP_RSV_TAG, lindaSpace.INTER_COMM, &status);
			/*void reserver(int reserve_buf[], int handle[])*/
			LindaContact rsvHandler(rq_buf);
			printf("PP_Reserve:\n");
			rsvHandler.print();
			int pp_error = PP_FAIL;
			if( !lindaSpace.reserver(rsvHandler) )
			{	
				// send out the request to another server
				printf("\tElement NOT found\n");
				// check if all server were visited
				if (rsvHandler.numServerVisited() <= lindaSpace.my_side_size)
				{
					//go to next server
					int next_server = (lindaSpace.my_side_rank+1)%lindaSpace.my_side_size;
					cout << "Next server "<< next_server << endl;
					int last_index = rsvHandler.numServerVisited();
					cout << "Last index "<< last_index << endl;
					rsvHandler.addServer(next_server);
					// serialize the rq and send it out
					memset(rq_buf,'\0',HANDLER_SIZE*sizeof(char));
					int req_size = rsvHandler.serializer(rq_buf);
					MPI_Send(rq_buf,req_size,MPI_CHAR, rsvHandler.rq_servers[last_index], PP_RSV_TAG, lindaSpace.MY_SIDE_COMM);
					cout << "Sent request to  "<< last_index << endl;
					
				}
				else
				{
					// All servers queried no one have the resquest
					cout << "Checked all, but did not find " << endl;
					rsvHandler.location_rank = -1;
					// serialize the rq and send it out to the requester rank
					memset(rq_buf,'\0',HANDLER_SIZE*sizeof(char)); 
					int req_size = rsvHandler.serializer(rq_buf);
					MPI_Send(rq_buf,req_size,MPI_CHAR, rsvHandler.rq_rank, PP_RSV_TAG, lindaSpace.INTER_COMM);
				}
			}
			else
			{
				// The element is in this server
				
				cout << "Element found !!  " << endl;
				rsvHandler.location_rank = lindaSpace.my_side_rank;
				memset(rq_buf,'\0',HANDLER_SIZE*sizeof(char)); 
				int req_size = rsvHandler.serializer(rq_buf);
				MPI_Send(rq_buf,req_size,MPI_CHAR, rsvHandler.rq_rank, PP_RSV_TAG, lindaSpace.INTER_COMM);
			}
			
			
		}
		mpi_flag = 0;
		MPI_Iprobe(MPI_ANY_SOURCE, PP_GET_TAG, lindaSpace.INTER_COMM, &mpi_flag, &status);
		if( mpi_flag == 1)//received get
		{
			MPI_Recv(&rq_buf, HANDLER_SIZE, MPI_CHAR, MPI_ANY_SOURCE, PP_RSV_TAG, lindaSpace.INTER_COMM, &status);

			LindaContact getHandler(rq_buf);
			printf("PP_Get:\n");
			getHandler.print();
			/*void taker(int index, void * work_unit_buf)*/
			lindaSpace.taker(index, work_unit_buf);
			int error = PP_FAIL;
			
			MPI_Send(&error,1, MPI_INT, getHandler.rq_rank, PP_RSV_TAG, lindaSpace.INTER_COMM);
		}
	}
	free(work_unit_buf);
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
				MPI_Send(&done,1, MPI_INT, i, PP_FINALIZE_TAG, lindaSpace.INTER_COMM);
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

int PP_Put(void * buffer, int size, int type, int target)
{
	// Create a Request Handler
	LindaContact reqHandler(lindaSpace.my_side_rank, target, size, type);

	int picked_server = rand()%lindaSpace.other_side_size;
	reqHandler.addServer(picked_server);
	
	int pp_error = -1;
	MPI_Status status;
	// Send the request to the chosen server
	char reqHandler_str[HANDLER_SIZE];
	memset(reqHandler_str,'\0',HANDLER_SIZE*sizeof(char));
	int req_size = reqHandler.serializer(reqHandler_str);
	reqHandler.print();
	// printf("buff out %s\t size: %d \n", reqHandler_str,req_size);
	MPI_Send(reqHandler_str,req_size,MPI_CHAR, picked_server, PP_PUT_TAG, lindaSpace.INTER_COMM);
	
	MPI_Recv(&pp_error, 1, MPI_INT, picked_server, PP_PUT_TAG, lindaSpace.INTER_COMM, &status);

	if (pp_error == PP_FAIL)
	{
		// Retry?
		printf("Retry Server Out of Memory: %d\n",picked_server);

	}
	else
	{
		// Space allocated on the server send data
		MPI_Send(buffer,size,MPI_CHAR, picked_server, PP_PUT_TAG, lindaSpace.INTER_COMM);
		// MPI_Recv(&pp_error, 1, MPI_INT, picked_server, PP_PUT_TAG, lindaSpace.INTER_COMM, &status);
	}


	return PP_SUCCESS;
}
/*<sumary>
	</sumary>*/
int PP_Reserve(int num_types_rq, int * types, int * size_found, int * type_found, int * handle)
{
	// TODO: Fix
	int pp_error = -1;
	MPI_Status status;
	// Create a Request Handler
	LindaContact reqHandler(lindaSpace.my_side_rank);
	// if num_types_rq == 0 do PP_WILDCARD_TYPE
	if( num_types_rq > 0)
		for (int i = 0; i < num_types_rq; ++i)
		{
			int type = *(types+i);
			printf("\t\tTyped to add: %d\n",type);
			reqHandler.addWorkType(type);
		}
	else
		reqHandler.addWorkType(PP_WILDCARD_TYPE);


	printf("num_types: %d\n", reqHandler.numWorkTypes());
	
	int picked_server = rand()%lindaSpace.other_side_size;
	reqHandler.addServer(picked_server);
	reqHandler.print();
	
	// Send the request to the chosen server
	char reqHandler_str[HANDLER_SIZE];
	memset(reqHandler_str,'\0',HANDLER_SIZE*sizeof(char));
	int req_size = reqHandler.serializer(reqHandler_str);

	MPI_Send(reqHandler_str,req_size,MPI_CHAR, picked_server, PP_RSV_TAG, lindaSpace.INTER_COMM);
	
	MPI_Recv(&pp_error, 1, MPI_INT, picked_server, PP_RSV_TAG, lindaSpace.INTER_COMM, &status);
	
	if (pp_error == PP_FAIL)
	{
		// Retry?
		printf("Not found?: %d\n",picked_server);

	}

}
/*<sumary>
	</sumary>*/

int PP_Get(void *work_unit_buf, int handler[])
{
	MPI_Status status;
	MPI_Send(&handler[1],1, MPI_INT, handler[0], 666, lindaSpace.INTER_COMM);
	MPI_Recv(work_unit_buf,1, handler[2], lindaSpace.other_side_leader, 666, lindaSpace.INTER_COMM, &status);

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

int PP_Abort(int error)
{
	printf("PP_ERROR\n");
	MPI_Abort(lindaSpace.WORLD_COMM_DUP, error);
	return error;
}

