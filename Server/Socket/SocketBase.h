#ifndef __SOCKET_BASE_H__
#define __SOCKET_BASE_H__

#include <list>
#include <thread>
#include <mutex>


#ifdef _WIN32
	#include <WinSock2.h>
	#pragma comment(lib, "ws2_32.lib")  //¼ÓÔØ ws2_32.dll
	#define HSocket SOCKET
	#define _WINSOCK_DEPRECATED_NO_WARNINGS

#elif __linux__
	#include <error.h>
	#include <arpa/inet.h>		// for inet_**
	#include <netdb.h>			// for gethost**
	#include <netinet/in.h>		// for sockaddr_in
	#include <sys/types.h>		// for socket
	#include <sys/socket.h>		// for socket
	#include <unistd.h>
	#include <fcntl.h> 
	#include <stdio.h>		    // for printf
	#include <stdlib.h>			// for exit
	#include <string.h>			// for bzero
	#define HSocket int

#endif

class SocketBase
{
public:
	SocketBase();
	~SocketBase();

	bool nonBlock(HSocket socket);

	//void closeConnect(HSocket socket);//ÐÞ¸Ä

protected:
	void closeConnect(HSocket socket);
	bool error(HSocket socket);
	
protected:
	std::mutex _mutex;
	
private:
	bool _bInitSuccess;
};

#endif