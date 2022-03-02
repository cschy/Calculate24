#include "SocketServer.h"
#include "../Game/24.h"
#include <iostream>


//SocketServer::ptr SocketServer::s_server = nullptr;

//SocketServer::ptr SocketServer::getInstance()
//{
//	if (s_server == nullptr)
//	{
//		s_server = std::make_shared<SocketServer>(new SocketServer());
//	}
//
//	return s_server;
//}

//void SocketServer::destroyInstance()
//{
//	delete s_server;
//}

SocketServer::SocketServer() :
	_socketServer(0),
	_isRunning(false),
	onRecv(nullptr),
	onStart(nullptr),
	onNewConnection(nullptr)
{

}

SocketServer::~SocketServer()
{
	//this->clear();
};

void SocketServer::clear()
{
	_mutex.lock();
	this->closeConnect(_socketServer);
	_socketServer = 0;
	_mutex.unlock();
}

bool SocketServer::startServer(unsigned short port)
{
	/*if (!initServer(port))
	{
		return false;
	}
	return true;*/
	return initServer(port);
}

bool SocketServer::initServer(unsigned short port)
{
	if (_socketServer != 0)
	{
		closeConnect(_socketServer);
	}
	_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	
	if (error(_socketServer))
	{
		std::cout << "socket error!" << std::endl;
		_socketServer = 0;
		return false;
	}

	do 
	{
		struct sockaddr_in sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		_serverPort = port;		// save the port

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(_serverPort);
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("");
		//bind
		int ret = ::bind(_socketServer, (const sockaddr*)&sockAddr, sizeof(sockAddr));
		if (ret < 0)
		{
			std::cout << "bind error!" << std::endl;
			break;
		}
		//listen
		ret = listen(_socketServer, 5);
		if (ret < 0)
		{
			std::cout << "listen error!" << std::endl;
			break;
		}
		// start 
		/*char hostName[256];
		gethostname(hostName, sizeof(hostName));
		std::cout << "hostname: " << hostName << std::endl;//test print hostname 
		struct hostent* hostInfo = gethostbyname(hostName);
		char* ip = inet_ntoa(*(struct in_addr *)*hostInfo->h_addr_list);
		std::cout << "ip: " << ip << std::endl;//test print ip*/
		std::cout << "port: " << port << " start!" << std::endl;
		this->acceptClient();

		return true;

	} while (false);
	
	closeConnect(_socketServer);
	_socketServer = 0;
	return false;
}

void SocketServer::acceptClient()
{
	std::thread th(&SocketServer::acceptFunc, this);
	th.detach();
}

void SocketServer::acceptFunc()
{
	struct sockaddr_in sockAddr;
	while (true)//如果注释了客服端无法再连接
	{	
#ifdef __linux__
		socklen_t len = sizeof(sockaddr_in);
		HSocket clientSock = accept(_socketServer, (sockaddr*)&sockAddr, &len);
#elif _WIN32
		int len = sizeof(sockaddr);
		HSocket clientSock = accept(_socketServer, (sockaddr*)&sockAddr, &len);
#endif
		_clientSockets.push_back(clientSock);
		if (error(clientSock))
		{
			std::cout << "accept error!" << std::endl;
			break;
		}
		else
		{
			//getpeername(clientSock, );
			_isRunning = true;
			this->newClientConnected(clientSock);
		}
	}
	closeClientSocket();
	_isRunning = false;
}

void SocketServer::newClientConnected(HSocket socket)
{
	std::cout << "new connect: " << socket << std::endl;

	//_clientSockets.push_back(socket);
	//new op: create recv mq for every connection
	//nothing write
	//std::cout << "Current Connections: " << _clientSockets.size() << std::endl;//print now conn nums

	std::thread th(&SocketServer::recvMessage, this, socket);
	th.detach();
}

void SocketServer::recvMessage(HSocket socket)
{
	char buff[1024];
	int ret = 0;

	while (true)
	{
		ret = recv(socket, buff, sizeof(buff), 0);
		if (ret < 0)
		{
			std::cout << "recv(" << socket << ") error, return " << ret << std::endl;
			break;
		}
		else
		{
			//暂不使用消息队列
			/*FILE *fptr;
			fptr = fopen("runoob.txt", "w");
			if (fptr != NULL)
			{
				fprintf(fptr, "%s", buff);
				fclose(fptr);
			}*/
			Calculate24::getInstance()->parseData(buff, socket);
		}
	}
	std::cout << "disconnet: " << socket << std::endl;
	_mutex.lock();
	this->closeConnect(socket);
	//对游戏数据包括_mSockToPlayer等内存数据进行清理
	auto iter = Calculate24::getInstance()->_mSockToPlayer.find(socket);
	if (iter != Calculate24::getInstance()->_mSockToPlayer.end())
	{
		int roomNum = iter->second._roomNum;
		int seatNum = iter->second._seatNum;
		Calculate24::getInstance()->_vRoom[roomNum]._player[seatNum]._synData._bOccupied = false;
		strcpy(Calculate24::getInstance()->_vRoom[roomNum]._player[seatNum]._synData._sName, "");
		strcpy(Calculate24::getInstance()->_vRoom[roomNum]._player[seatNum]._synData._sHead, "");
		Calculate24::getInstance()->_vRoom[roomNum]._player[seatNum]._synData._bReady = false;
		Calculate24::getInstance()->_vRoom[roomNum]._player[seatNum]._synData._bAnswer = false;
		Calculate24::getInstance()->_vRoom[roomNum]._player[seatNum]._synData._bOnline = false;
		Calculate24::getInstance()->_vRoom[roomNum]._player[seatNum]._synData._bWinner = false;
		Calculate24::getInstance()->_vRoom[roomNum]._player[seatNum]._synData._iScore = 100;		
		Calculate24::getInstance()->_vRoom[roomNum]._player[seatNum]._socket = -1;
		--Calculate24::getInstance()->_vRoom[roomNum]._playerCount;
		/*if (Calculate24::getInstance()->_vRoom[roomNum]._playerCount > 0)
		{
			Calculate24::getInstance()->sendData(socket, DataType_S::ALLOW_LOGIN);
		}*/
		//删除map中对应数据
		Calculate24::getInstance()->_mSockToPlayer.erase(iter);

		Calculate24::getInstance()->sendData(socket, DataType_S::SYN, SynType::LEAVE);
	}
	else
	{
		std::cout << "raise: " << socket << " can not login." << std::endl;
	}
	
	//Calculate24::getInstance()->_isFinish = true;
	//_clientSockets.pop_back();//new op
	_mutex.unlock();
}

void SocketServer::sendMessage(HSocket socket, const char* data, int count)
{
	/*for (auto& sock : _clientSockets)
	{
		if (sock == socket)
		{
			int ret = send(socket, data, count, 0);
			if (ret < 0)
			{
				std::cout << "send error!" << std::endl;
			}
			break;
		}
	}*/
	
	int ret = send(socket, data, count, 0);
	if (ret < 0)
	{
		std::cout << "send error!" << std::endl;
	}
		
}

void SocketServer::sendMessage(const char* data, int count)
{
	for (auto& socket : _clientSockets)
	{
		int ret = send(socket, data, count, 0);
		if (ret < 0)
		{
			std::cout << "send error!" << std::endl;
		}
	}
}

void SocketServer::closeConnectPublic(HSocket socket)
{
	this->closeConnect(socket);
}