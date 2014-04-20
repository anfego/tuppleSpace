#ifndef PP_H
#define PP_H

#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#define PP_SUCCESS      1
#define PP_FAIL         2  // an error occurred
#define PP_EXHAUSTION   3
#define PP_NO_MORE_WORK 4
#define PP_HANDLE_SIZE 	6
#define PP_FINALIZE_TAG	666
#define PP_PUT_TAG		999
#define PP_RESV_TAG		555
#define PP_GET_TAG		333

int PP_Init(int num_user_types, int * user_types, int * am_server_flag);
int PP_Finalize();


int PP_Put(void * buffer, int type, int size );
int PP_Reserve(int& num_types, int types[], int &size, int& type, int handle[] );
int PP_Get(void *work_unit_buf, int handler[]);
int PP_Set_problem_done();
int PP_Abort(int error);
#endif

