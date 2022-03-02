//#pragma once
#ifndef __SOCKET_SERVER_H__
#define __SOCKET_SERVER_H__

#define _CRT_SECURE_NO_WARNINGS

#include "SocketBase.h"
//#include "../Game/24.h"
#include <memory>
#include <functional>
#include <vector>

struct RecvData
{
	HSocket socketClient;
	int dataLen;
	char data[1024];
};

class SocketServer : public SocketBase, public std::enable_shared_from_this<SocketServer>
{
public:
	typedef std::shared_ptr<SocketServer> ptr;
	void closeClientSocket()
	{
		for (auto&i : _clientSockets)
		{
			closeConnect(i);
		}
	}
	void clear();
	SocketServer();
	~SocketServer();
	//static SocketServer::ptr getInstance();
	//void destroyInstance();

	void closeConnectPublic(HSocket socket);
	bool startServer(unsigned short port);
	void sendMessage(HSocket socket, const char* data, int count);
	void sendMessage(const char* data, int count);

	unsigned int getLines()
	{
		return _clientSockets.size();
	}
	bool _isRunning;
	std::function<void(const char* ip)> onStart;
	std::function<void(HSocket socket)> onNewConnection;
	std::function<void(HSocket socket, const char* data, int count)> onRecv;
	std::function<void(HSocket socket)> onDisconnect;


private:
	
	bool initServer(unsigned short port);
	void acceptClient();
	void acceptFunc();
	void newClientConnected(HSocket socket);
	void recvMessage(HSocket socket);
	
private:
	//static SocketServer::ptr s_server;
	HSocket _socketServer;
	unsigned short _serverPort;

private:
	std::vector<HSocket> _clientSockets;
	//std::list<SocketMessage*> _UIMessageQueue;
	std::mutex   _UIMessageQueueMutex;

};

#endif