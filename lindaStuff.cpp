
#include "lindaStuff.h"
#include "/nfshome/rbutler/public/courses/pp6430/mpich3i/include/mpi.h"


using namespace std;

	// Default ructor
	lindaStuff::lindaStuff()
	{
		server = 0;
		index = 0;
	}
	// Constructor
	lindaStuff::lindaStuff( int am_server )
	{
		server = am_server;
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

	int lindaStuff::allocate(int size, int type)
	{
		node tempNode;
		// Allocates size +1 to ensure NULL ended
		tempNode.memory = (char *)malloc((size+1)*sizeof(char));
		if (tempNode.memory != NULL)//if no space
		{
			memset(tempNode.memory,'\0',(size+1)*sizeof(char));
			tempNode.type = type;
			tempNode.size = size;
			tempNode.reserved = false;
			tempNode.index = index++;
			

			myNodes.push_back(tempNode);
			printf("\tMemory allocated: %s\n\tNode size: %d\n",(char *)tempNode.memory, myNodes.size());
			return (myNodes.size()-1);//index
		}
		return -1;
	}
	void lindaStuff::store(void *work_unit_buf, int &index)
	{
		// store data in allocated memory
		int local_index = getLocalIndex(index);
		memcpy(myNodes[local_index].memory, work_unit_buf, myNodes[local_index].size*sizeof(char));
		// Ensure Null Ended Data
		// memset(myNodes[index].memory+myNodes[index].size,'\0',1);
		printData(index);
	}

	void lindaStuff::reserver(int rq_buf[], int handle[])
	{
		int i = 0;
		memset(handle, '\0', 4*sizeof(int));
		if(rq_buf[0] == 0) //then just take first one
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
					for (int j = 1; j < rq_buf[0]; ++i)
					{
						if(myNodes[i].type == rq_buf[j])
						{
							myNodes[i].reserved = true;
							handle[0]/*.rank*/			= my_world_rank; 
							handle[1]/*.ID*/			= i;
							handle[2]/*.size_of_work*/	= myNodes[i].size;
							handle[3]/*.type_of_work*/	= myNodes[i].type;
							handle[4]					= myNodes[i].index;
							return;
						}
					}
				}
			}
		}
		
		if(i == rq_buf[0])
			handle[1] = -1;
	}
	bool lindaStuff::reserver(LindaContact & rsvHandler)
	{
		for (int i = 0; i < myNodes.size(); ++i)
		{
			for (int j = 0; j < rsvHandler.types.size(); ++j)
			{
				//TODO: Add target validation
				if (!myNodes[i].reserved && ( rsvHandler.types[j] == -1 || myNodes[i].type == rsvHandler.types[j]))
				{
					printf("reserved\n");
					myNodes[i].reserved = true;
					rsvHandler.location_rank = my_side_rank;
					rsvHandler.size = myNodes[i].size;
					rsvHandler.clcWorkTypes();
					rsvHandler.addWorkType(myNodes[i].type);
					return true;

				}	

			}
		}
		// not found
		return false;
	}
	
	void lindaStuff::taker(int index, void * work_unit_buf)
	{
		memcpy(work_unit_buf, myNodes[index].memory, myNodes[index].size);
		myNodes.erase(myNodes.begin()+index);
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
	int lindaStuff::getLocalIndex(int index)
{
	for (int i = 0; i < myNodes.count; ++i)
	{
		if(myNodes[i].index == index)
			return i;
	}
	return -1;
}