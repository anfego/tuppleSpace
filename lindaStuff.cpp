
#include "lindaStuff.h"
#include "/nfshome/rbutler/public/courses/pp6430/mpich3i/include/mpi.h"




	// Default ructor
	lindaStuff::lindaStuff()
	{
		server = 0;
	}
	// Constructor
	lindaStuff::lindaStuff( int am_server )
	{
		server = am_server;
	}

	lindaStuff::~lindaStuff()
	{
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

	int lindaStuff::store(int &type, int &size, void* work_unit_buf)
	{
		int resp = malloc(tempNode.memory, size);
		if (resp == 0)//if no space
		{
			return PP_FAIL;
		}
		else
		{
			memcpy(resp, work_unit_buf, size);
			node tempNode;
			tempNode.type = type;
			tempNode.size = size;
			tempNode.reserved = false;

			tempNode.memory = resp;
			myNodes.push_back(tempNode);
			resp = NULL;
		}
		return PP_SUCCESS;
	}

	
	// handle[0]/*.rank*/			= picked_server; 
	// handle[1]/*.ID*/			= resp.ID;
	// handle[2]/*.size_of_work*/	= resp.size;
	// handle[3]/*.type_of_work*/	= resp.type;
	// int reserve_buf, int handle
	int lindaStuff::reserver(int reserve_buf[], int handle[])
	{

		int i = 1;

		if(reserve_buf[0] == 0) //then just take first one
		{
			for (; i < myNodes.size(); ++i)
			{
				if ( !myNodes[i].reserved )
				{
					myNodes[i].reserved = true;
				}
			}
			if(i == reserve_buf[0])
				return -1;//NOT_FOUND
			else
				return i;
		}
		else
		{	// go while particular not found
			for (; i < myNodes.size(); ++i)
			{
				if ( !myNodes[i].reserved )
				{
					for (int j = 1; j < reserve_buf[0]; ++i)
					{
						if(myNodes[i].type == reserve_buf[j])
							myNodes[i].reserved = true;
					}
				}
			}
			if(i == reserve_buf[0])
				return -1;//NOT_FOUND
			else
				return i;
		}
	}
	