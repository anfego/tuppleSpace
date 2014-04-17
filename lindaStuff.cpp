
#include "lindaStuff.h"
#include "/nfshome/rbutler/public/courses/pp6430/mpich3i/include/mpi.h"




	// Default ructor
	lindaStuff::lindaStuff()
	{
		server = 0;
	}
	// Constructor
	lindaStuff::lindaStuff(const MPI_Comm comm_world, const MPI_Comm comm_inter, int am_server)
	{
		set_linda_comm(comm_world, comm_inter);
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
	int lindaStuff::set_linda_comm( MPI_Comm comm_world,  MPI_Comm comm_inter)
	{
		LINDA_COMM = comm_inter;
		INTER_COMM = comm_inter;
		return MPI_SUCCESS;
	}
	int lindaStuff::get_comm_inter(MPI_Comm * comm_in)
	{
		*comm_in = INTER_COMM;
		return 0;
	}

	int lindaStuff::get_comm_linda(MPI_Comm * comm_in)
	{
		*comm_in = LINDA_COMM;
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



	