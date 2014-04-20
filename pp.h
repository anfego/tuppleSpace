#ifndef PP_H
#define PP_H

#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#define PP_SUCCESS      1
#define PP_FAIL         -11  // an error occurred
#define PP_EXHAUSTION   3
#define PP_NO_MORE_WORK 4
#define PP_HANDLE_SIZE 	6
#define PP_FINALIZE_TAG	666
#define PP_PUT_TAG		999
#define PP_RSV_TAG		555
#define PP_GET_TAG		333

/*<sumary>
	int PP_Init()
	    int num_user_types;
	        Any valid integer can be a user type.
	        Wildcards will be described below.
	    int user_types[PP_MAX_USER_TYPES];
	    int server_flag;  // I begin to execute server code if true
	        If this rank becomes a server, it should stop accepting
	        Puts after it has exhausted a gigabyte.
	    Discussion:
	        The user should do MPI_Init and MPI_Finalize.
	</sumary>*/
int PP_Init(int num_user_types, int * user_types, int * am_server_flag);

/*<sumary>

	</sumary>*/
int PP_Finalize();

/*<sumary>
	int PP_Put()
	    void *buf;
	    int length;
	    int type;
	    int target_rank;  // -1 -> available for any rank
	    // int handle[PP_HANDLE_SIZE];  // probably not useful except debugging
	</sumary>*/
int PP_Put(void * buffer, int size, int type );

/*<sumary>
	int PP_Reserve()
		int num_types_in_req;        // <= 0 -> wildcard (any type)
		int types_to_search_for[];   // logically OR'd together
		int *size_of_work_found;
		int *type_of_work_found;
		int handle[PP_HANDLE_SIZE];  // contains all nec info to do a Get later
			int rank where data is
			int unique ID of the data item at that rank
			int size_of_work
			int type_of_work
	</sumary>*/
int PP_Reserve(int num_types_rq, int * types, int * size_found, int type_found, int * handle);

/*<sumary>
	int PP_Get()
		void *buf;
		int handle[];  // from prior Reserve
	</sumary>*/
int PP_Get(void *work_unit_buf, int handler[]);

/*<sumary>
	int PP_Set_problem_done()
		Discussion:
		Causes the return code on calls like Reserve or Get to 
			return PP_NO_MORE_WORK
	</sumary>*/
int PP_Set_problem_done();

/*<sumary>

	</sumary>*/
int PP_Abort(int error);
#endif

