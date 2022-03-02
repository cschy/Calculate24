
#include "SocketBase.h"

SocketBase::SocketBase()
{
	_bInitSuccess = false;
#ifdef _WIN32
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 0);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if (nRet != 0) 
	{
		fprintf(stderr, "Initilize Error!\n");
		return;
	}
	_bInitSuccess = true;
#endif

}

SocketBase::~SocketBase()
{
#ifdef _WIN32
	if (_bInitSuccess)
	{
		WSACleanup();
	}
#endif 
}

void SocketBase::closeConnect(HSocket socket)
{
#ifdef __linux__
		close(socket);
#elif _WIN32
		closesocket(socket);
#endif
}

bool SocketBase::error(HSocket socket)
{
#ifdef __linux__
	return socket < 0;
#elif _WIN32
	return socket == SOCKET_ERROR;
#endif
}

bool SocketBase::nonBlock(HSocket socket)
{
#ifdef __linux__
	int flags;
	flags = fcntl(socket, F_GETFL, 0);
	flags != O_NONBLOCK;
	if (fcntl(socket, F_SETFL, flags) < 0)
	{
		return false;
	}
#elif _WIN32
	u_long ulOn;
	ulOn = 1;
	if (ioctlsocket(socket, FIONBIO, &ulOn) == SOCKET_ERROR)
	{
		return false;
	}
#endif

	return true;
}