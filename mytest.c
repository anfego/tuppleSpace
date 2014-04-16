#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "pp.h"

#define WORK            1
#define ANSWER          2
#define MASTER_RANK     0
#define dbgprintf(flag,...) mydbgprintf(flag,__LINE__,__VA_ARGS__)

int my_world_rank = -1;

int main(int argc, char *argv[])
{
    int rc, i, done, num_servers, work_unit_size, am_server_flag;
    int num_world_ranks, num_work_units, num_answers;
    int work_type, work_handle[PP_HANDLE_SIZE], work_len;
    int num_handled_by_me;
    // int num_user_types, user_types[PP_MAX_USER_TYPES];
  
    int req_types[2];
    int num_user_types = 2;
    int user_types[2] = {WORK,ANSWER};

    char *work_unit_buf;

    double temptime, time_for_fake_work;
    double start_job_time, end_put_time, start_work_time, end_work_time;
    double total_work_time, total_reserve_time, total_get_time;

    num_servers = 1;
    work_unit_size = 0;    /* default */
    num_work_units = 4;    /* default */
    time_for_fake_work = 0.0;  /* default */
    total_work_time = 0.0;
    total_get_time  = 0.0;
    total_reserve_time = 0.0;
    num_handled_by_me = 0;
    work_unit_buf = (char *) malloc(work_unit_size);

    for (i=1; i < argc; i++)  /* start at 1 */
    {        
        if (strcmp(argv[i],"-nservers") == 0)
            num_servers = atof(argv[++i]);
        else if (strcmp(argv[i],"-n") == 0)
            num_work_units = atoi(argv[++i]);
        else if (strcmp(argv[i],"-s") == 0)
            work_unit_size = atoi(argv[++i]);
        else if (strcmp(argv[i],"-t") == 0)
            time_for_fake_work = atof(argv[++i]);
        else
        {
            printf("unrecognized cmd-line arg at %d :%s:\n",i,argv[i]);
            exit(-1);
        }
    }
    int temp;
    am_server_flag = 0;
    rc = PP_Init(num_user_types,user_types,&am_server_flag);  // servers stay until end
    rc = temp;
    printf("TEMP: %d\n", temp);


    return 0;
}