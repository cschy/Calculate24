#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__
#include "SocketBase.h"
#include <functional>

class SocketClient : public SocketBase
{
public:
	static SocketClient* getInstance();
	void destroyInstance();

	void initNetwork();
	bool connectServer(const char* serverIP, unsigned short port);
	void sendMessage(const char* data, int count);

	std::function<void(const char* data, int count)> onRecv;
	std::function<void()> onDisconnect;

	//void update(float dt);
	//HSocket _socektClient;//ÐÞ¸Ä
	void closeConnectPublic();


	SocketClient(void);
	~SocketClient(void);

private:
	bool initClient();
	void recvMessage();

	

private:
	HSocket _socketServer;
	HSocket _socektClient;

	static SocketClient* s_client;
	std::mutex   _UIMessageQueueMutex;
};

#endif