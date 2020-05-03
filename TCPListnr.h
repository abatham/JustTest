
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define SERVER_LIMIT 10

using namespace std;

class TCPListnr{

	public:
		TCPListnr(string ip="0.0.0.0", int port=54000);
		~TCPListnr();
		int init();
		int run();
		virtual int receiveData(int sock);
		virtual int sendData(int sock, const char * buff, int len);	
		virtual void onConnection(int sock);
		int broadcastMsg(int sock, const char * buff, int len);	
		void disp();
	private:

		string m_ip;
		int m_port;
		int m_servSock;
		struct sockaddr_in m_servAddr;
		fd_set m_master;
		char m_msg[2048];

};
