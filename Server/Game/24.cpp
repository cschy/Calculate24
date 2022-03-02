
#include "24.h"
//#include "../Socket/SocketServer.h"
#include <stack>
#include <map>
#include <time.h>
#include <string.h>
#include <thread>

void Clock::set(int hour, int min, int sec)
{
	h = hour;
	m = min;
	s = sec;
}

void Clock::tick()
{
	time_t t = time(NULL);//取得当前时间
	while (time(NULL) == t);
	if (--s < 0) {
		s = 59;
		if (--m < 0) {
			m = 59;
			--h < 0;
		}
	}
}

void Clock::show()
{
	std::cout << '\r';
	if (h < 10)std::cout << 0;
	std::cout << h << ':';
	if (m < 10)std::cout << 0;
	std::cout << m << ':';
	if (s < 10)std::cout << 0;
	std::cout << s << std::flush;
}

void Clock::run(bool& accordVar, bool& setVar)
{
	while (h != 0 || m != 0 || s != 0) {
		if (!accordVar)
		{
			return;
		}
		tick();
		show();
	}
	std::cout << std::endl;
	setVar = false;
}

//int PlayerRoom::_getFreeSeat() 
//{
//	for (int i = 0; i < 4; i++)
//	{
//		if (!_player[i]._isOccupied)
//		{
//			return i;
//		}
//	}
//	return -1;
//}


//Calculate24::ptr Calculate24::_c24(new Calculate24(), Calculate24::destroyInstance);
Calculate24* Calculate24::_c24 = new Calculate24();
Calculate24::AutoRelease Calculate24::_autoRelease;
Calculate24::Calculate24() :
	//_server(std::make_shared<SocketServer>(new SocketServer())),
	_readyCount(0),
	_isStart(false),
	_isFinish(false),
	_qdLock(false),
	_isWin(false),
	_isRunning(true),
	_vRoom(),
	_mSockToPlayer(),
	_roomNull(true)
{
	/*_serverData = new GameData_S {
		DataType_S::ALLOW_LOGIN, sizeof(GameData_S), "", "",
	};*/

}

Calculate24::~Calculate24()
{

}

Calculate24* Calculate24::getInstance()
{
	if (!_c24)
	{
		_c24 = new Calculate24();
	}
	return _c24;
}

void Calculate24::parseData(const char* data, HSocket socket)
{
	/*TypeTraits* type = (TypeTraits*)data;//类型萃取
	if (type->typeC == DataType_C::LOGIN)*/
	//直接用枚举来接受，要保证结构体第一个成员是类型
	//根据枚举类型来强制转换成需要的数据
	DataType_C* dataType = (DataType_C*)data;

	if (*dataType == DataType_C::LOGIN || *dataType == DataType_C::REGISTER)
	{
		UserInfo* userInfo = (UserInfo*)data;
		if (userInfo->_iSize == sizeof(UserInfo))
		{
			//验证redis缓存数据库中账号密码...
			bool needGenNewRoom = true;
			bool isFindSameName = false;
			switch (userInfo->_eType)
			{
			case DataType_C::LOGIN:
				//1.没有找到账号，不允许登录
				//2.找到账号，密码错误，不允许登录
				//3.账号密码正确，允许登录
				//进入房间和座位

				for (int i = 0; i < _vRoom.size(); i++)
				{
					//if (!_vRoom[i]._isFull())
					if (_vRoom[i]._playerCount < 4 && !_vRoom[i]._isStart)
					{

						for (int idx = 0; idx < 4; idx++)
						{
							if (_vRoom[i]._player[idx]._synData._sName == userInfo->_sName)
							{
								isFindSameName = true;
								break;
							}
						}
						if (!isFindSameName)
						{
							++_vRoom[i]._playerCount;
							unsigned int j = _vRoom[i]._getFreeSeat();
							_vRoom[i]._player[j]._synData._bOccupied = true;
							_vRoom[i]._player[j]._synData._bOnline = true;
							strcpy(_vRoom[i]._player[j]._synData._sName, userInfo->_sName);
							//_vRoom[i]._player[j]._name = userInfo->_sName;//强转深copy?
							//_vRoom[i]._player[j]._isOccupied = true;
							_vRoom[i]._player[j]._socket = socket;
							_vRoom[i]._player[j]._roomNum = i;
							_vRoom[i]._player[j]._seatNum = j;

							_mSockToPlayer[socket] = _vRoom[i]._player[j];		//暂时还没用new，_mSockToPlayer[socket]
																				//99%是单独的数据，需要同步

						}
						needGenNewRoom = false;
						break;
					}
				}
				if (needGenNewRoom)
				{
					PlayerRoom playerRoom = PlayerRoom();
					playerRoom._player[0]._synData._bOccupied = true;
					playerRoom._player[0]._synData._bOnline = true;
					strcpy(playerRoom._player[0]._synData._sName, userInfo->_sName);

					playerRoom._player[0]._socket = socket;
					playerRoom._player[0]._roomNum = _vRoom.size();
					playerRoom._player[0]._seatNum = 0;
					_mSockToPlayer[socket] = playerRoom._player[0];
					_vRoom.push_back(playerRoom);
					++_vRoom.back()._playerCount;
				}
				if (!isFindSameName)
				{
					//_mPlayers[userInfo->_sName] = socket;//登录以后的消息收发不用带name
					sendData(socket, DataType_S::ALLOW_LOGIN);
					sendData(socket, DataType_S::SYN, SynType::ENTER);
					//sendData(socket, DataType_S::SYN);
					/*SendRoomInfo_S sendRoomInfo = { DataType_S::ROOM_INFO, sizeof(SendRoomInfo_S), _mSockToPlayer[socket]._roomNum };
					_server->sendMessage(socket, (const char*)&sendRoomInfo, sizeof(SendRoomInfo_S));*/
					std::cout << userInfo->_sName << " login!" << std::endl;
				}
				else
				{
					sendData(socket, DataType_S::DISALLOW_LOGIN);
				}
				break;
			case DataType_C::REGISTER:
				//1.找到账号，不允许注册
				//2.未找到账号，允许注册
				sendData(DataType_S::ALLOW_REGISTER);
				std::cout << userInfo->_sName << " register!" << std::endl;
				break;
			default:
				break;
			}
		}
	}

	else if (*dataType == DataType_C::STATE)
	{
		SendState_C* recvState = (SendState_C*)data;
		if (recvState->_iSize == sizeof(SendState_C))
		{
			int roomNum = _mSockToPlayer[socket]._roomNum;
			int seatNum = _mSockToPlayer[socket]._seatNum;
			_vRoom[roomNum]._player[seatNum]._synData._bReady = recvState->_bisReady;
			_mSockToPlayer[socket]._synData._bReady = recvState->_bisReady;

			if (_vRoom[roomNum]._player[seatNum]._synData._bReady)
			{
				std::cout << roomNum << ":" << seatNum << ":" << _vRoom[roomNum]._player[seatNum]._synData._sName << " is ready" << std::endl;
			}
			else
			{
				std::cout << roomNum << ":" << seatNum << ":" << _vRoom[roomNum]._player[seatNum]._synData._sName << " is cancel" << std::endl;
			}
			//同步状态给所有玩家
			sendData(socket, DataType_S::SYN, SynType::READY);
			/*for (int i = 0; i < 4; i++)
			{
				if (_vRoom[roomNum]._player[i]._synData._bOccupied)
				{
					sendData(_vRoom[roomNum]._player[i]._socket, DataType_S::SYN, SynType::READY);
				}
			}*/
		}
	}

	else if (*dataType == DataType_C::QD)
	{
		int roomNum = _mSockToPlayer[socket]._roomNum;
		int seatNum = _mSockToPlayer[socket]._seatNum;
		if (!_vRoom[roomNum]._qdLock)
		{
			_vRoom[roomNum]._qdLock = true;

			_vRoom[roomNum]._player[seatNum]._synData._bAnswer = true;
			_mSockToPlayer[socket]._synData._bAnswer = true;

			sendData(socket, DataType_S::SYN, SynType::QD);
			/*SendNQD_S sendNQD = { DataType_S::NQD, sizeof(SendNQD_S), "" };
			strcpy(sendNQD._sName, _mSockToPlayer[socket]._name.c_str());
			for (int i = 0; i < 4; i++)
			{
				if (_vRoom[roomNum]._player[i]._isOccupied && i != seatNum)
				{
					_server->sendMessage(_vRoom[roomNum]._player[i]._socket, (const char*)&sendNQD, sizeof(SendNQD_S));
				}
			}*/
		}
	}

	else if (*dataType == DataType_C::RESULT)
	{
		SendResult_C* recvResult = (SendResult_C*)data;
		if (recvResult->_iSize == sizeof(SendResult_C))
		{
			/*SendResult_S sendResult = { DataType_S::RESULT, sizeof(SendResult_S), false };
			sendResult._bisWin = !recvResult->_bisWin;*/
			int roomNum = _mSockToPlayer[socket]._roomNum;
			int seatNum = _mSockToPlayer[socket]._seatNum;
			_mSockToPlayer[socket]._synData._bWinner = recvResult->_bisWin;
			_vRoom[roomNum]._player[seatNum]._synData._bWinner = recvResult->_bisWin;
			if (_vRoom[roomNum]._player[seatNum]._synData._bWinner)
			{
				_vRoom[roomNum]._player[seatNum]._synData._iScore += 10;
			}
			else
			{
				_vRoom[roomNum]._player[seatNum]._synData._iScore -= 10;
			}
			for (int i = 0; i < 4; i++)
			{

				if (_vRoom[roomNum]._player[i]._synData._bOccupied && i != seatNum)
				{
					_vRoom[roomNum]._player[i]._synData._bWinner = !recvResult->_bisWin;
					if (_vRoom[roomNum]._player[i]._synData._bWinner)
					{
						_vRoom[roomNum]._player[i]._synData._iScore += 10;
					}
					else
					{
						_vRoom[roomNum]._player[i]._synData._iScore -= 10;
					}
					//_server->sendMessage(_vRoom[roomNum]._player[i]._socket, (const char*)&sendResult, sizeof(SendResult_S));
				}
			}
			sendData(socket, DataType_S::SYN, SynType::RESULT);
			_vRoom[roomNum]._isFinish = true;
			sendData(socket, DataType_S::SYN, SynType::RESET);
		}

	}
}

void Calculate24::sendData(HSocket socket, DataType_S dataType, SynType synType)
{
	int roomNum = _mSockToPlayer[socket]._roomNum;
	int seatNum = _mSockToPlayer[socket]._seatNum;

	/*int seatIndex[3], i = 0;
	for (int j = 0; j < 4; j++)
	{
		if (j != seatNum && i < 3)
		{
			seatIndex[i] = j;
			i++;
		}
	}*/
	auto p1 = _vRoom[roomNum]._player[0];
	auto p2 = _vRoom[roomNum]._player[1];
	auto p3 = _vRoom[roomNum]._player[2];
	auto p4 = _vRoom[roomNum]._player[3];

	if (dataType == DataType_S::ALLOW_LOGIN)
	{
		//后面可以改进打包成发送当前房间的info，包括房间号和房间创建时间等系统info
		SendAllowLogin_S sendAllowLogin = { dataType, sizeof(SendAllowLogin_S), roomNum, };
		//只给当前socket发
		_server->sendMessage(socket, (const char*)&sendAllowLogin, sizeof(SendAllowLogin_S));

		std::cout << _mSockToPlayer[socket]._synData._sName << " enter: " << roomNum << ":" << seatNum << std::endl;

		if (!_vRoom[roomNum]._isGenThread)
		{
			_vRoom[roomNum]._isGenThread = true;
			std::thread th(&Calculate24::startGame, this, roomNum);
			th.detach();
		}
	}

	else if (dataType == DataType_S::DISALLOW_LOGIN)
	{
		SendDisAllowLogin_S sendDisAllowLogin = { dataType, sizeof(SendDisAllowLogin_S), DisAllowLoginType::SAME_NAME };
		_server->sendMessage(socket, (const char*)&sendDisAllowLogin, sizeof(SendDisAllowLogin_S));
		//_server->closeConnectPublic(socket);//万一这一行执行快呢
	}

	else if (dataType == DataType_S::SYN)
	{
		if (synType == SynType::ENTER) {
			srand(time(NULL));
			strcpy(_vRoom[roomNum]._player[seatNum]._synData._sHead, ("UI/Head/" + std::to_string(rand() % 24) + ".png").c_str());
			strcpy(_mSockToPlayer[socket]._synData._sHead, _vRoom[roomNum]._player[seatNum]._synData._sHead);
		}
		else if (synType == SynType::READY) {

		}
		else if (synType == SynType::POKER) {

		}
		else if (synType == SynType::QD) {

		}
		else if (synType == SynType::RESULT) {

		}
		else if (synType == SynType::RESET) {
			for (int i = 0; i < 4; i++)
			{
				if (_vRoom[roomNum]._player[i]._synData._bOccupied)
				{
					_vRoom[roomNum]._player[i]._synData._bReady = false;
					_vRoom[roomNum]._player[i]._synData._bAnswer = false;
					_vRoom[roomNum]._player[i]._synData._bWinner = false;
				}
			}
		}
		else if (synType == SynType::LEAVE) {

		}

		SendSynData_S sendSynData = { dataType, sizeof(SendSynData_S), synType, {_vRoom[roomNum]._player[0]._synData, _vRoom[roomNum]._player[1]._synData, _vRoom[roomNum]._player[2]._synData, _vRoom[roomNum]._player[3]._synData}, };
		for (int i = 0; i < 4; i++)
		{
			if (_vRoom[roomNum]._player[i]._synData._bOccupied)
			{
				_server->sendMessage(_vRoom[roomNum]._player[i]._socket, (const char*)&sendSynData, sizeof(SendSynData_S));
			}
		}
	}
}

//void Calculate24::sendData(HSocket socket, DataType_S dataType)//系统信息
//{
//	int roomNum = _mSockToPlayer[socket]._roomNum;
//	int seatNum = _mSockToPlayer[socket]._seatNum;
//
//	int seatIndex[3], i = 0;
//	for (int j = 0; j < 4; j++)
//	{
//		if (j != seatNum && i < 3)
//		{
//			seatIndex[i] = j;
//			i++;
//		}
//	}
//	auto p1 = _vRoom[roomNum]._player[seatIndex[0]];
//	auto p2 = _vRoom[roomNum]._player[seatIndex[1]];
//	auto p3 = _vRoom[roomNum]._player[seatIndex[2]];
//
//	if (dataType == DataType_S::ALLOW_LOGIN)
//	{
//		//syn players data
//		
//		//auto p4 = _mSockToPlayer[socket];
//		//int seatNum = _mSockToPlayer[socket]._seatNum;
//		
//		//后面可以改进打包成发送当前房间的info，包括房间号和房间创建时间等系统info
//		SendAllowLogin_S sendAllowLogin = { dataType, sizeof(SendAllowLogin_S), roomNum, };
//
//		srand(time(NULL));
//		strcpy(_mSockToPlayer[socket]._synData._sHead, (std::to_string(rand() % 24) + ".png").c_str());
//		strcpy(_vRoom[roomNum]._player[seatNum]._synData._sHead, _mSockToPlayer[socket]._synData._sHead);
//		std::cout << _mSockToPlayer[socket]._synData._sName << " enter: " << roomNum << ":" << seatNum << std::endl;
//		
//		_server->sendMessage(socket, (const char*)&sendAllowLogin, sizeof(SendAllowLogin_S));
//
//		/*if (_vRoom[roomNum]._player[seatNum]._isOccupied){
//			std::cout << _mSockToPlayer[socket]._name << " enter: " << roomNum << ":" << seatNum << std::endl;
//		}else{
//			std::cout << _mSockToPlayer[socket]._name << " leave: " << roomNum << ":" << seatNum << std::endl;
//		}
//
//		for (int i = 0; i < 4; i++)
//		{
//			strcpy(sendAllowLogin._sOthers[i], _vRoom[roomNum]._player[i]._name.c_str());
//		}
//		for (int i = 0; i < 4; i++)
//		{
//			if (_vRoom[roomNum]._player[i]._isOccupied)
//			{
//				_server->sendMessage(_vRoom[roomNum]._player[i]._socket, (const char*)&sendAllowLogin, sizeof(SendAllowLogin_S));
//			}
//		}
//		*/
//
//		if (!_vRoom[roomNum]._isGenThread)
//		{
//			_vRoom[roomNum]._isGenThread = true;
//			std::thread th(&Calculate24::startGame, this, roomNum);
//			th.detach();
//		}
//
//	}
//
//	else if (dataType == DataType_S::DISALLOW_LOGIN)
//	{
//		SendDisAllowLogin_S sendDisAllowLogin = { dataType, sizeof(SendDisAllowLogin_S), DisAllowLoginType::SAME_NAME };
//		_server->sendMessage(socket, (const char*)&sendDisAllowLogin, sizeof(SendDisAllowLogin_S));
//		//_server->closeConnectPublic(socket);//万一这一行执行快呢
//	}
//	
//	else if (dataType == DataType_S::SYN)
//	{
//		SendSynData_S sendSynData = { dataType, sizeof(SendSynData_S), {p1._synData, p2._synData, p3._synData, _mSockToPlayer[socket]._synData}, };
//	
//		_server->sendMessage(socket, (const char*)&sendSynData, sizeof(SendSynData_S));
//	}
//}

void Calculate24::sendData(DataType_S dataType)
{
	if (dataType == DataType_S::ALLOW_REGISTER)
	{
		SendAllowRegister_S sendAllowRegister = { dataType, sizeof(SendAllowRegister_S) };
		_server->sendMessage((const char*)&sendAllowRegister, sizeof(SendAllowRegister_S));
	}
}

void Calculate24::checkRoomNull()
{
	do {
		_roomNull = true;
		for (auto &i : _vRoom)
		{
			if (i._playerCount > 0)
			{
				_roomNull = false;
				break;
			}
		}
	} while (true);
}

void Calculate24::init()
{
	_server = SocketServer::ptr(new SocketServer());
	if (_server && _server->startServer(8888))
	{
		/*std::thread th(&Calculate24::checkRoomNull, this);
		th.detach();*/

		startServer();
	}
	else
	{
		std::cout << "start server error" << std::endl;
	}
}

void Calculate24::startServer()
{
	//Clock clock(0, 1, 0);
	_clock = Clock(0, 30, 0);
	do
	{
		_roomNull = true;
		for (auto& i : _vRoom)
		{
			if (i._playerCount > 0)
			{
				_roomNull = false;
				break;
			}
		}
		if (_roomNull) {
			_clock.tick();
			_clock.show();
		}
		else {
			_clock.set(0, 30, 0);
			//_clock.show();
		}
	} while (_clock.h != 0 || _clock.m != 0 || _clock.s != 0);
	/*do
	{
		_clock.run();
		_roomNull = true;
		for (auto& i : _vRoom)
		{
			if (i._playerCount > 0)
			{
				_roomNull = false;
				break;
			}
		}
		if (_roomNull) {
			break;
		}
		else {
			_clock.set(0, 1, 0);
		}
	} while (true);*/
	//do
	//{
	//	/*clock.set(0, 1, 0);
	//	clock.run(_roomNull, _isRunning);
	//	if (!_isRunning)
	//	{
	//		break;
	//	}*/
	//	_clock.run();
	//} while (0);
	
	_server->clear();
	//while (_server->_isRunning);
}

void Calculate24::sendPoker(int roomNum, std::string poker)
{
	SendCard_S sendCard = { DataType_S::CARD, sizeof(SendCard_S), "" };
	strcpy(sendCard._sCard, poker.c_str());
	std::cout << "room ID: " << roomNum << " dispatch cards are: " << sendCard._sCard << std::endl;
	for (int i = 0; i < 4; i++)
	{
		if (_vRoom[roomNum]._player[i]._synData._bOccupied)
		{
			_server->sendMessage(_vRoom[roomNum]._player[i]._socket, (const char*)&sendCard, sizeof(SendCard_S));
		}
	}

}

std::string Calculate24::_genRandomPoker()
{
	srand(time(NULL));
	char str[5] = "AAAA";
	for (int i = 0; i < 4; i++)
	{
		int rand1 = rand() % 5;
		if (rand1 < 3)
		{
			int rand2 = rand() % 3;
			str[i] = std::to_string(3 * rand1 + rand2 + 1)[0];
			if (str[i] == '1')
			{
				str[i] = 'A';
			}
		}
		else if (rand1 == 3)
		{
			str[i] = rand() % 2 ? 'D' : 'J';
		}
		else
		{
			str[i] = rand() % 2 ? 'Q' : 'K';
		}
	}
	return str;
}

void Calculate24::startGame(int roomNum)
{
	std::cout << "spi room: " << roomNum << std::endl;
	do
	{
		//如果没有人则退出线程房间
		if (_vRoom[roomNum]._playerCount <= 0)
		{
			_vRoom[roomNum]._isOver = true;
			break;
		}
		//所有准备好的人直接开
		if (!_vRoom[roomNum]._isStart)
		{
			_vRoom[roomNum]._fcanStart();//不用每次循环都进来了，减小开销
		}
		if (_vRoom[roomNum]._bcanStart && !_vRoom[roomNum]._isStart)
		{
			_vRoom[roomNum]._isStart = true;
			std::cout << "room: " << roomNum << " start game" << std::endl;
			sendPoker(roomNum, _genRandomPoker());
		}

		if (_vRoom[roomNum]._isStart && _vRoom[roomNum]._isFinish)
		{
			for (int i = 0; i < 4; i++)
			{
				_vRoom[roomNum]._player[i]._synData._bReady = false;
				_vRoom[roomNum]._player[i]._synData._bAnswer = false;
				_vRoom[roomNum]._player[i]._synData._bWinner = false;
			}
			_vRoom[roomNum]._isStart = false;
			_vRoom[roomNum]._qdLock = false;
			_vRoom[roomNum]._isFinish = false;
			_vRoom[roomNum]._bcanStart = false;
			//break;
		}
	} while (1);
	std::cout << "clear room: " << roomNum << std::endl;
	_vRoom[roomNum]._isStart = false;
	_vRoom[roomNum]._qdLock = false;
	_vRoom[roomNum]._isFinish = false;
	_vRoom[roomNum]._bcanStart = false;
	_vRoom[roomNum]._isGenThread = false;
	//char c;
	//std::cout << "按e退出" << std::endl;
	//while ((c = getchar()) != EOF)
	//{
	//	if (c == 'e')
	//	{
	//		_isOver = true;
	//		//_server->destroyInstance();
	//		break;
	//	}
	//}
}