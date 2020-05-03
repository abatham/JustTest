#include "TCPListnr.h"


TCPListnr::TCPListnr(string ip,int port)
:m_ip(ip), m_port(port)
{
	FD_ZERO(&m_master);
	cout<<"Listener Object created\n";
}

TCPListnr::~TCPListnr()
{
	close (m_servSock);
	FD_CLR(m_servSock, &m_master);
	for (int i=0; i<= FD_SETSIZE; i++)
	{
		if (FD_ISSET(i, &m_master))
		{
			FD_CLR(i, &m_master);
			close(i);
		}
	}
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
	
	FD_SET(m_servSock, &m_master);
	return 0;	
}

int TCPListnr::run()
{
	//fd_set copyFd;
	//FD_ZERO(&copyFd);
	//copyFd = m_master;

	while(true)
	{
		fd_set copyFd = m_master; // copy the master list to the working list 

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
					int client = accept( i, NULL, NULL);
					if ( client == -1 )
					{
						perror("Failed to accept a connection");
						return -2;
					}
					cout<<"Client added:"<<client<<"\n";
					cout<<"Before \n";
					disp();
					FD_SET(client, &m_master);
					cout<<"after\n";
					disp();
					onConnection(client);
				}
				else //if ( i != )
				{
					cout<<"Message from:"<<i<<"\n";
					int len = receiveData(i);
					if ( len == -1 )
					{
						cout<<"Failed to receive data\n";
						//return -3;
						continue;
					}	

				}
			}
		}

	}

}
void TCPListnr::disp()
{
	cout<<"//////////////////////\n";
        for( int j =0 ; j<= FD_SETSIZE; j++)
        {
                if ( FD_ISSET(j , &m_master) != 0)
                                cout<<j<<endl;
        }
}

int TCPListnr::receiveData(int sock)
{
	cout<<"Before receive:\n";
	disp();
	memset(m_msg, 0, 2048);
	int len = recv(sock, &m_msg, 2048, 0);
	if ( len <=0 )
	{
		close(sock);
		FD_CLR(sock, &m_master);
		memset(m_msg, 0, 2048);
		return len;
	}

	disp();
	cout<<"Client :"<<sock<<"\n";
	
	int ret = broadcastMsg(sock, m_msg, len);
	if( len == -1)
	{
		memset(m_msg, 0, 2048);
		return len;
	}
	
	return 0;
}

TCPListnr::broadcastMsg(int sock, const char * buff, int len)
{
	//fd_set copy = m_master;

	for ( int i=0; i<= FD_SETSIZE; i++)
	{
		if (FD_ISSET(i, &m_master) != 0)
		{
			if (( i != sock) && (i != m_servSock))
			{
				int stat = sendData(i, buff, len);
				if ( stat == -1 )
				{
					cout<<"Broadcast failed \n";
					return -1;
				}
			}
		}
	}
	return 0;
}

int TCPListnr::sendData(int sock, const char * buff, int len)
{
	int stat = send(sock, buff, len, 0);
	if ( stat == -1 )
	{
		cout<<"FD failed:"<<sock<<"\t";
		perror("Send Failed ");
		return stat;
	}
	return stat;
}

void TCPListnr::onConnection(int sock)
{
	string welcomeMsg = "Hey there..\n";
	int stat = sendData( sock, welcomeMsg.c_str(), welcomeMsg.length()+1);
	if (stat == -1 )
	{
		cout<<"Failed to send the welcome message to client:"<<sock<<"\n";
	}

}
