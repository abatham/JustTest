#include "TCPListnr.h"


TCPListnr::TCPListnr(string ip,int port)
:m_ip(ip), m_port(port)
{
	FD_ZERO(&master);
	cout<<"Listener Object created\n";
}

TCPListnr::~TCPListnr()
{

}

int TCPListnr::init()
{
	m_servSock = socket( AF_INET, SOCK_STREAM, 0);
	if ( m_servSock == -1 )
        {
                perror("Failed to create a socket");
		return -1;
        }

	m_servAddr.sin_family = AF_INET;
	m_servAddr.sin_port = htons(m_port);
	if (inet_pton(AF_INET, m_ip.c_str(), (void*)&m_servAddr.sin_addr.s_addr) != 1)
	{
		perror("inet_ptons failed");
		return -2;
	}	

	if ( bind(m_servSock, (struct sockaddr *) &m_servAddr, sizeof(m_servAddr)) != 0)
	{
		perror("Bind failed");
		return -3;
	}
	
	if (listen(m_servSock, SERVER_LIMIT) != 0)
	{
		perror("Failed to listen");
		return -4;
	}
	
	FD_SET(m_servSock, &master);
	return 0;	
}

int TCPListnr::run()
{
	fd_set copyFd;
	FD_ZERO(&copyFd);
	copyFd = master;

	while(true)
	{
		copyFd = master; // copy the master list to the working list 

		if ( select(FD_SETSIZE, &copyFd, NULL, NULL, NULL) == 0)
		{
			perror("select timed out before any connection was established");
			return -1;
		}

		for (int i = 0; i <= FD_SETSIZE; ++i)
		{
			if (FD_ISSET(i, &copyFd) != 0)
			{
				if( i == m_servSock ) // accept the incomming connection 
				{
					int client = accept(m_servSock, NULL, NULL);
					if ( client == -1 )
					{
						perror("Failed to accept a connection");
						return -2;
					}

					FD_SET(client, &master);
				}
				else if ( i != )
				{
					int len = receiveData();
					/*
					if ( len == -1 )
					{
						close (i);
						FD_CLR(i, &master);
						continue;
					}
					*/
					if ( len == -1 )
					{
						cout<<"Failed to receive data\n";
						return -3;
					}	

					int stat = sendData();

					if ( stat != 0)
					{
						cout<<"Failed to send the data\n";
						return -4;
					}	

				}
			}
		}

	}

}


int TCPListnr::receiveData()
{
	return 0;
}


int TCPListnr::sendData()
{
	return 0;
}

