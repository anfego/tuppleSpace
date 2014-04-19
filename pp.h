#ifndef PP_H
#define PP_H

#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#define PP_SUCCESS      1
#define PP_FAIL         2  // an error occurred
#define PP_EXHAUSTION   3
#define PP_NO_MORE_WORK 4
#define PP_HANDLE_SIZE  6
#define PP_FINALIZE_TAG	66

int PP_Init(int num_user_types, int * user_types, int * am_server_flag);
int PP_Finalize();

int PP_Put();
int PP_Reserve();
int PP_Get();
int PP_Set_problem_done();
int PP_Abort();
#endif

