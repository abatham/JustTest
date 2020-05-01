#include "TCPListnr.h"


int main(int argc, char * argv[])
{

	if ( argc < 2 )
	{
		cout<<"Incorrect usage\n";
		return -1;
	}
	string ip = argv[1];
	int port = stoi( argv[2]);

	TCPListnr t1(ip, port);
	
	t1.init();
	t1.run();
	return 0;
}
