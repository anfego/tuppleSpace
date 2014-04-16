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


using namespace std;
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
	MPI_Comm comm_world_dup;
	MPI_Comm_dup(MPI_COMM_WORLD, &comm_world_dup);

	// get user types
	if(*am_server_flag == 0)
	{
		return PP_SUCCESS;
	}
	else
	{
		printf("I am a server %d\n",*am_server_flag);
		vector<int> uTypes;
		// create stack for user types 
		for (int i = 0; i < num_user_types; ++i)
		{
		}
	}
	return PP_SUCCESS;	
}
int PP_Finalize()
{
	return PP_SUCCESS;
}
int PP_Put()
{
	return PP_SUCCESS;
}
int PP_Reserve()
{
	return PP_SUCCESS;
}
int PP_Get()
{
	return PP_SUCCESS;
}
int PP_Set_problem_done()
{
	return PP_SUCCESS;
}
int PP_Abort()
{
	return PP_SUCCESS;
}

