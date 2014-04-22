#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


#include "/nfshome/rbutler/public/courses/pp6430/mpich3i/include/mpi.h"

#include "pp.h"

#define WORK            1
#define ANSWER          2
#define MASTER_RANK     0
#define dbgprintf(flag,...) mydbgprintf(flag,__LINE__,__VA_ARGS__)

int my_world_rank = -1;

void mydbgprintf(int flag, int linenum, const char *fmt, ...)
{
    char *s;
    va_list ap;

    if ( ! flag)
        return;
    va_start( ap, fmt );
    vasprintf(&s, fmt, ap);
    va_end(ap);
    printf("%d: %d:  %s",my_world_rank,linenum,s);
    /* fflush(stdout); */ /* already did setbuf */
    free(s);
}

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
    rc = MPI_Init(NULL,NULL);
    rc = MPI_Comm_rank(MPI_COMM_WORLD,&my_world_rank);
    rc = MPI_Comm_size(MPI_COMM_WORLD,&num_world_ranks);
  
    if (my_world_rank >= (num_world_ranks-num_servers))
        {
            printf("nservers = %d\n",num_servers);
            am_server_flag = 1;
        }
    else
        am_server_flag = 0;
  
    
    rc = PP_Init(num_user_types,user_types,am_server_flag);  // servers stay until end
    
    if(!am_server_flag)
    {
                if ( my_world_rank == MASTER_RANK )
                {
                    num_answers = 0;
                    for (i=0; i < num_work_units; i++)
                    {
                        memset(work_unit_buf,'X',work_unit_size);
                        if (work_unit_size >= 18)
                            sprintf(work_unit_buf,"workunit %d",i);
                        printf("Storing: %s\n", work_unit_buf);
                        rc = PP_Put( work_unit_buf, work_unit_size, WORK, -1); 
                        if (rc != PP_SUCCESS)
                        {
                            dbgprintf( 1, "**** failed: put work_unit %d  rc %d\n", i, rc );
                            PP_Abort(-1);
                        }
                    }
                    dbgprintf(1,"all work submitted after %f secs\n",MPI_Wtime()-start_job_time);
                }
                int temp = 3;
                rc = PP_Reserve(1,&temp,&work_len,&work_type,work_handle);
                if ( rc == PP_EXHAUSTION )
                {
                    dbgprintf( 1, "done by exhaustion\n" );
                    
                }
                else if ( rc == PP_NO_MORE_WORK )
                {
                    dbgprintf( 1, "done by no more work\n" );
                    
                }
                else if (rc < 0)
                {
                    dbgprintf( 1, "** reserve failed, rc = %d\n", rc );
                    PP_Abort(-1);
                }
        }
    PP_Finalize();
    printf("DONE\n");
    // free(work_unit_buf);
    


    return 0;
}
