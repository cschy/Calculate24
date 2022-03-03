#include "SocketClient.h"
#include "Data/TransData.h"

#include <iostream>

SocketClient* SocketClient::s_client = nullptr;

SocketClient* SocketClient::getInstance()
{
	if (s_client == nullptr)
	{
		s_client = new SocketClient();
	}
	return s_client;
}

void SocketClient::destroyInstance()
{
	delete s_client;
}

SocketClient::SocketClient(void) :
	onRecv(nullptr),
	_socektClient(0)
{
}

SocketClient::~SocketClient(void)
{
	
}

void SocketClient::initNetwork()
{
	//s_client = SocketClient::construct();
	/*s_client->onRecv = CC_CALLBACK_2(ClientLayer::onRecv, this);
	_client->onDisconnect = CC_CALLBACK_0(ClientLayer::onDisconnect, this);
	_lblInfo->setString("This is Client");
	isConnect = false;*/
	if (_socektClient)
	{
		return;
	}
	
	if (!s_client->connectServer("xxx", 1234))//填自己的服务器IP和端口
	{
		std::cout << "Client connect error" << std::endl;
	}
}

bool SocketClient::initClient()
{
	_socektClient = socket(AF_INET, SOCK_STREAM, 0);
	if (error(_socketServer))
	{
		std::cout << "init client error!" << std::endl;
		_socektClient = 0;
		return false;
	}
	return true;
}

bool SocketClient::connectServer(const char* serverIP, unsigned short port)
{
	if (!this->initClient())
	{
		return false;
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(struct sockaddr_in));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(serverIP);

	int ret = 0;
	ret = connect(_socektClient, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));
	if (ret < 0)
	{
		_socektClient = 0;
		return false;
	}

	std::thread recvThread(&SocketClient::recvMessage, this);
	recvThread.detach();

	return true;
}

void SocketClient::recvMessage()
{
	char recvBuf[1024];
	int ret = 0;
	while (true)
	{
		ret = recv(_socektClient, recvBuf, sizeof(recvBuf), 0);
		if (ret < 0)
		{
			std::cout << "recv error!" << std::endl;
			break;
		}
		else
		{
			TransData::getInstance()->parseData(recvBuf);
		}
		
	}
	_mutex.lock();
	this->closeConnect(_socektClient);
	std::cout << "close client" << std::endl;
	
	_socektClient = 0;
	_mutex.unlock();
}

void SocketClient::sendMessage(const char* data, int count)
{
	if (_socektClient != 0)
	{
		int ret = send(_socektClient, data, count, 0);
		if (ret < 0)
		{
			std::cout << "send error!" << std::endl;
		}
	}
}


void SocketClient::closeConnectPublic()
{
	closeConnect(_socektClient);
}