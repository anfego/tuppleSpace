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

#include "pp.h"

/*<sumary>
    int num_user_types;
    Any valid integer can be a user type.
	Wildcards will be described below.
    int user_types[PP_MAX_USER_TYPES];
    int server_flag;  // I begin to execute server code if true
    Discussion:
	The user should do MPI_Init and MPI_Finalize.
	</sumary>*/
int PP_Init(int num_user_types, int user_types, int * am_server_flag)
{

	while(not end)
	{
		if (connection)
		{
			check type 
			if(SET)
			{
				try to allocate memory
				if(successfull)
					send "OK"
				else //malloc returned 0
					send "No Space"
			}
			if(GET)
			{
				check whether it exists in local machine
				if(found)
					send
				else
				{
					broadcast in server scope request for a information and start waiting for all responces
					if(all not found)
						send "NOT FOUND"
					else
						send a rank of server that has information
						OR
						get information from the server and transmit to APP
				}
			}
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

