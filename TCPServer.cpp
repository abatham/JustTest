#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_BACKLOG 10

using namespace std;

int main(int argc, char *argv[])
{
	// Check for the input params
	if ( argc < 2)
	{
		cout<<"Incorrect number of params\n";
		cout<<"Usage: EXE <IP> <PORT>\n";
		return -1;
	}
	// Create a Socket
	int servSock = -1;
	if ((servSock = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		cout<<"Failed to create a Socket\n";
		return -1;
	}

	
	//cout<<"IP- "<<argv[1]<<endl;
	//cout<<"PORT- "<<argv[2]<<endl;
	//return 0;
	// bind it to the address and port
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons( stoi(argv[2]));
	//servAddr.sin_addr.s_addr = INADDR_ANY;
	if (inet_pton(AF_INET, argv[1], (void *) &servAddr.sin_addr.s_addr) <= 0)
	{
		perror("inet_pton failed\n");
		return -1;
	}
	
	if ( bind( servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
	{
		//cout<<"Failed to bind to the given IP:PORT\n";
		perror("Failed to bind to this port  ");
		return -1;
	}	


	// listen on the socket
	if ( listen(servSock, SERVER_BACKLOG) == -1)
	{
		cout<<"Failed on Listen\n";
		return -1;
	}
	
	fd_set masterFd, copyFd;

	FD_ZERO(&masterFd);
	FD_ZERO(&copyFd);
	// use select to poll on the changing sockets
	FD_SET(servSock, &masterFd);
	char buff[2048];
	memset(buff, 0, 2048);
	while(true)
	{
		copyFd = masterFd;

		if ( select(FD_SETSIZE, &copyFd, NULL, NULL, NULL) < 0)
		{
			cout<<"Select failed \n";
			return -1;
		}

		for(int i= 0; i <= FD_SETSIZE; i++)
		{
			if (FD_ISSET(i , &copyFd))
			{
				if ( i == servSock )
				{
					// accept connection
					int cliSock = accept(i, NULL, NULL);
					if ( cliSock == -1 )
					{
						cout<<"Failed to accept on the server socket\n";
						return -1;
					}

					FD_SET(cliSock, &masterFd);
				}
				else
				{
					// receive the messagei
					memset(buff, 0, 2048);
					int len = recv(i , &buff, 2048, 0);
					if ( len == 0 )
					{
						cout<<"Client dropped\n";
						close(i);
						FD_CLR(i , &masterFd);
						continue;
					}

					// broadcast the message to all the other clients
					for( int j =0 ; j <= FD_SETSIZE; j++)
					{
						if (FD_ISSET(i, &masterFd))
						{
							if ( j == servSock || j == i )
								continue;
							send(j, &buff, len + 1, 0);
							//memset(buff, 0, 2048);
						}
					}

				}	
			}
		}

	}
	close(servSock);
	return 0;
}
