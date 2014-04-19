
#include "lindaStuff.h"
#include "/nfshome/rbutler/public/courses/pp6430/mpich3i/include/mpi.h"


using namespace std;

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

	int lindaStuff::allocate(int &size, int &type)
	{
		char *resp = (char *)malloc(size);
		
		if (resp != NULL)//if no space
		{
			node tempNode;
			tempNode.type = type;
			tempNode.size = size;
			tempNode.reserved = false;
			tempNode.memory = resp;

			myNodes.push_back(tempNode);
			
			return myNodes.size() -1;//index
		}
		return -1;
	}
	void lindaStuff::store(void *work_unit_buf, int &index)
	{
		memcpy(myNodes[index].memory, work_unit_buf, myNodes[index].size);
	}
	// handle[0]/*.rank*/			= picked_server; 
	// handle[1]/*.ID*/			= resp.ID;
	// handle[2]/*.size_of_work*/	= resp.size;
	// handle[3]/*.type_of_work*/	= resp.type;
	// int reserve_buf, int handle
	void lindaStuff::reserver(int reserve_buf[], int handle[])
	{
		int i = 0;
		memset(handle, '\0', 4*sizeof(int));
		if(reserve_buf[0] == 0) //then just take first one
		{
			for (; i < myNodes.size(); ++i)
			{
				if ( !myNodes[i].reserved )
				{
					myNodes[i].reserved = true;
					// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!--
					handle[0]/*.rank*/			= my_world_rank; // <<<<<=======<<<<
					handle[1]/*.ID*/			= i;
					handle[2]/*.size_of_work*/	= myNodes[i].size;
					handle[3]/*.type_of_work*/	= myNodes[i].type;
					return;
				}
			}
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
						{
							myNodes[i].reserved = true;
							handle[0]/*.rank*/			= my_world_rank; 
							handle[1]/*.ID*/			= i;
							handle[2]/*.size_of_work*/	= myNodes[i].size;
							handle[3]/*.type_of_work*/	= myNodes[i].type;
							return;
						}
					}
				}
			}
		}
		
		if(i == reserve_buf[0])
			handle[1] = -1;
	}
	
	void lindaStuff::taker(int index, void * work_unit_buf)
	{
		memcpy(work_unit_buf, myNodes[index].memory, myNodes[index].size);
		myNodes.erase(myNodes.begin()+index);
	}