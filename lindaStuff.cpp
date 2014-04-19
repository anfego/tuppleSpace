
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

	int lindaStuff::reserver(int num_types, int &type, int &size)
	{
		if(myNodes.size() == 0)
			return -1;
		
		int i = 0;

		if(type == 0) //then just take first one
		{
			for (; i < myNodes.size(); ++i)
			{
				if ( !myNodes[i].reserved )
				{
					myNodes[i].reserved;
				}
			}
			if(i == myNodes.size())
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
					for (int j = 0; j < num_types; ++i)
					{
						if(myNodes[i].type == type)
							myNodes[i].reserved;
					}
				}
			}
			if(i == myNodes.size())
				return -1;//NOT_FOUND
			else
				return i;
		}
	}
	