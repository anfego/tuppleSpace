tuppleSpace
===========

Creation of a tupple space using MPI

Implement a parallel library (libpp.a) that provides to the user the
interface outlined below, which supports the concept of a distributed
shared data space.  Along with the library, supply a pp.h file which
user programs can include, and which provides all necessary function
prototypes, defines e.g. PP_SUCCESS, PP_HANDLE_SIZE, etc.

Implement in C/C++.

Use MPI (mpich3) for the communication mechanism.


Any function may return any of these values:
    PP_SUCCESS 
    PP_FAIL  // an error occurred
    PP_EXHAUSTION
    PP_NO_MORE_WORK


int PP_Init()
    int num_user_types;
        Any valid integer can be a user type.
        Wildcards will be described below.
    int user_types[PP_MAX_USER_TYPES];
    int server_flag;  // I begin to execute server code if true
    Discussion:
        The user should do MPI_Init and MPI_Finalize.

int PP_Finalize()

int PP_Put()
    void *buf;
    int length;
    int type;
    int target_rank;  // -1 -> available for any rank
    // int handle[PP_HANDLE_SIZE];  // probably not useful except debugging

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

int PP_Get()
    void *buf;
    int handle[];  // from prior Reserve

int PP_Set_problem_done()
    Discussion:
        Causes the return code on calls like Reserve or Get to 
        return PP_NO_MORE_WORK

int PP_Abort()
    int code;
    Discussion:
        May print debug info before invoking MPI_Abort(MPI_COMM_WORLD,code);

A number of ranks will be designated as 'servers".
So, running the program might be like this:
    mpiexec -f somehostsfile -n 8 ./pgmname -nservers 3
-nservers is a required argument to specify the number of servers.

