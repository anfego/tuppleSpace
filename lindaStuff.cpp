
#include "lindaStuff.h"
#include "/nfshome/rbutler/public/courses/pp6430/mpich3i/include/mpi.h"



using namespace std;

	// Default ructor
	lindaStuff::lindaStuff()
	{
		server = 0;
		key = 0;
		numRsv = 0;

	}
	// Constructor
	lindaStuff::lindaStuff( int am_server )
	{
		server = am_server;
		key = 0;
		numRsv = 0;
	}

	lindaStuff::~lindaStuff()
	{
		for (int i = 0; i < myNodes.size(); ++i)
		{
			free(myNodes[i].memory);
		}
	}
	int lindaStuff::getType()
	{
	}
	bool lindaStuff::isReserved()
	{
	}

	int lindaStuff::get_comm_inter(MPI_Comm * comm_in)
	{
		*comm_in = WORLD_COMM_DUP;
		return 0;
	}

	int lindaStuff::get_comm_linda(MPI_Comm * comm_in)
	{
		*comm_in = MY_SIDE_COMM;
		return 0;
	}
	int lindaStuff::am_i_server()
	{
		return server;
	}
	void lindaStuff::set_as_server()
	{
		server = 1;
	}

	int lindaStuff::allocate(LindaContact & putHandler)
	{
		node tempNode;
		// Allocates size +1 to ensure NULL ended
		tempNode.memory = (char *)malloc((putHandler.size+1)*sizeof(char));
		if (tempNode.memory != NULL)//if no space
		{
			memset(tempNode.memory,'\0',(putHandler.size+1)*sizeof(char));
			tempNode.type = putHandler.types[0];
			tempNode.size = putHandler.size;
			tempNode.target = putHandler.target_rank;
			tempNode.reserved = false;
			tempNode.id = key++;
			myNodes.push_back(tempNode);
			// printf("\tMemory allocated: %s\n\tNode size: %d\n",(char *)tempNode.memory, myNodes.size());
			return tempNode.id;//index
		}
		return -1;
	}
	void lindaStuff::store(void *work_unit_buf, int &index)
	{
		// store data in allocated memory
		memcpy(myNodes[index].memory, work_unit_buf, myNodes[index].size*sizeof(char));
		// Ensure Null Ended Data
		// memset(myNodes[index].memory+myNodes[index].size,'\0',1);
		printData(index);
	}


	bool lindaStuff::reserver(LindaContact & rsvHandler)
	{
		// printf("\tmyNodes size: %d | %d\n",myNodes.size(),rsvHandler.types.size());
		for (int i = 0; i < myNodes.size(); ++i)
		{
			
			for (int j = 0; j < rsvHandler.types.size(); ++j)
			{
				// printf("\tquery type: %d agains %d\n", rsvHandler.types[j], myNodes[i].type);
				// printf("\tquery rank: %d agains %d\n", rsvHandler.rq_rank, myNodes[i].target);
				//TODO: Add target validation
				if (!myNodes[i].reserved && 
					( rsvHandler.types[j] == 0 || myNodes[i].type == rsvHandler.types[j]) &&
					(rsvHandler.rq_rank == myNodes[i].target || myNodes[i].target == -1))
				{
					// printf("reserved\n");
					myNodes[i].reserved = true;
					rsvHandler.location_rank = my_side_rank;
					rsvHandler.size = myNodes[i].size;
					rsvHandler.clcWorkTypes();
					rsvHandler.addWorkType(myNodes[i].type);
					rsvHandler.data_id = myNodes[i].id;
					return true;

				}	

			}
		}
		// not found
		return false;
	}
	
	int lindaStuff::taker(int id, void * work_unit_buf)
	{
		int index = findNodeById(id);
		if (index < 0)
		{
			return PP_FAIL;
		}
		memcpy(work_unit_buf, myNodes[index].memory, myNodes[index].size);
		myNodes.erase(myNodes.begin()+index);
		return PP_SUCCESS;
	}

	void lindaStuff::printData(int index)
	{
		printf("\t%d :Node %s\n", my_side_rank, (char *)myNodes[index].memory);
	}
	void lindaStuff::printAllData()
	{
		printf("\tAll Data\n\tServer: %d\n",my_side_rank);
		for (int i = 0; i < myNodes.size(); ++i)
		{
			printData(i);
		}
	}
	void lindaStuff::rsvRequest(MPI_Comm &RQ_COMM)
	{
		char rq_buf[HANDLER_SIZE];
		memset(rq_buf,'\0',HANDLER_SIZE*sizeof(char));
		MPI_Status status;
		if (RQ_COMM == INTER_COMM)
		{
			// printf("NUM RQ: %d\n",++numRsv);
			++numRsv;
		}
			
		
		MPI_Recv(&rq_buf, HANDLER_SIZE, MPI_CHAR, MPI_ANY_SOURCE, PP_RSV_TAG, RQ_COMM, &status);
		
		LindaContact rsvHandler(rq_buf);
		int pp_error = PP_FAIL;
		if( !reserver(rsvHandler) )
		{	
			//go to next server
			if (my_side_size > 1)
			{
				int next_server = (my_side_rank+1)%my_side_size;
				// printf("RSV next server: %d my_side_size %d\n", next_server, my_side_size);
				// serialize the rq and send it out
				memset(rq_buf,'\0',HANDLER_SIZE*sizeof(char));
				int req_size = rsvHandler.serializer(rq_buf);
				MPI_Send(rq_buf,req_size,MPI_CHAR, next_server, PP_RSV_TAG, MY_SIDE_COMM);
			}
		}
		else
		{
			// The element is in this server
			rsvHandler.location_rank = my_side_rank;
			memset(rq_buf,'\0',HANDLER_SIZE*sizeof(char)); 
			int req_size = rsvHandler.serializer(rq_buf);
			// printf("element found sending back--->%d\n",rsvHandler.rq_rank);
			rsvHandler.print();
			MPI_Send(rq_buf, req_size, MPI_CHAR, rsvHandler.rq_rank, PP_RSV_TAG, INTER_COMM);
		}
	}
	int lindaStuff::findEmptyHandler()
	{
		for (int i = 0; i < PP_MAX_RQ; ++i)
		{
			// printf("array %d: %d\n", i, lindaHandler[i].used);
			if (lindaHandler[i].used == 0)
			{
				return i;
			}
		}
		return -1;
	}
	void lindaStuff::storeHandler(int index, LindaContact & rqHandler)
	{
		lindaHandler[index].used = 1;
		lindaHandler[index].rq_rank = rqHandler.rq_rank;
		lindaHandler[index].data_id = rqHandler.data_id;
		lindaHandler[index].location_rank = rqHandler.location_rank;
		lindaHandler[index].target_rank = rqHandler.target_rank;
		lindaHandler[index].size = rqHandler.size;
		lindaHandler[index].types = rqHandler.types;
		lindaHandler[index].rq_servers = rqHandler.rq_servers;
	}
	int lindaStuff::getData(void * bufferOut, int index)
	{
		char reqHandler_str[HANDLER_SIZE];
		memset(reqHandler_str,'\0',HANDLER_SIZE*sizeof(char));
		MPI_Status status;
		int req_size = lindaHandler[index].serializer(reqHandler_str);
		MPI_Send(reqHandler_str,req_size,MPI_CHAR, lindaHandler[index].location_rank, PP_GET_TAG, INTER_COMM);
		int get_status;
		MPI_Recv(&get_status, 1, MPI_INT, lindaHandler[index].location_rank, PP_GET_TAG, INTER_COMM, &status);
		
		if (get_status >= 0)
		{
			// data ready	
			MPI_Recv((char *)bufferOut, lindaHandler[index].size, MPI_CHAR, lindaHandler[index].location_rank, PP_GET_TAG, INTER_COMM, &status);
			// printf("DataIN: %s\n",(char *)bufferOut);
			return PP_SUCCESS;
		}
		return PP_FAIL;


	
	}
	int lindaStuff::findNodeById(int id)
	{
		for (int i = 0; i < myNodes.size(); ++i)
		{
			if (myNodes[i].id == id)
			{
				return i;
			}
		}
		return -1;
	}
	int lindaStuff::getRequest(MPI_Comm & RQ_COMM)
	{
		char rq_buf[HANDLER_SIZE];
		memset(rq_buf,'\0',HANDLER_SIZE*sizeof(char));
		MPI_Status status;
		
		MPI_Recv(&rq_buf, HANDLER_SIZE, MPI_CHAR, MPI_ANY_SOURCE, PP_GET_TAG, RQ_COMM, &status);

		LindaContact getHandler(rq_buf);
		// printf("PP_Get:\n");
		// getHandler.print();
		/*void taker(int index, void * work_unit_buf)*/
		// error = taker(getHandler.data_id, work_unit_buf);
		int index;
		index = findNodeById(getHandler.data_id);
		MPI_Send(&index,1, MPI_INT, getHandler.rq_rank, PP_GET_TAG, RQ_COMM);
		if (index < 0)
		{
			return PP_FAIL;
		}
		else 
		{
			// printf("DATA OUT %s\n", (char *)myNodes[index].memory );
			MPI_Send((char *)myNodes[index].memory, getHandler.size, MPI_CHAR, getHandler.rq_rank, PP_GET_TAG, RQ_COMM);
			myNodes.erase(myNodes.begin()+index);
			return PP_SUCCESS;
		}
	}
	int lindaStuff::putRequest()
	{
		char rq_buf[HANDLER_SIZE];

		memset(rq_buf,'\0',HANDLER_SIZE*sizeof(char));

		MPI_Status status;
		
		MPI_Recv(rq_buf, HANDLER_SIZE, MPI_CHAR, MPI_ANY_SOURCE, PP_PUT_TAG, INTER_COMM, &status);
			
		LindaContact putHandler(rq_buf);
		int put_status = allocate(putHandler);

		MPI_Send(&put_status,1, MPI_INT, putHandler.rq_rank, PP_PUT_TAG, INTER_COMM);
		//get the datapp
		if (put_status != PP_FAIL)
		{
			// work_unit_buf = (char *)malloc(putHandler.size*sizeof(char));
			MPI_Recv((char *)myNodes[myNodes.size()-1].memory, putHandler.size, MPI_CHAR, putHandler.rq_rank, PP_PUT_TAG, INTER_COMM, &status);
				//put in allocated space
				// lindaSpace.store(work_unit_buf, resp);
				// free(work_unit_buf);
			return PP_SUCCESS;
		}
		return PP_FAIL;
	}
	