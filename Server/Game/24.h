//#pragma once
#ifndef __GAME24_H__
#define __GAME24_H__

#define _CRT_SECURE_NO_WARNINGS

#include "Config.h"
//#include "../DataProcess/ParseRequest.h"
#include "../Socket/SocketServer.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
//#include <memory>

//class SocketServer;

class Clock {
public:
	int h;
	int m;
	int s;
public:
	Clock(int hour = 0, int min = 1, int sec = 0) : h(hour), m(min), s(sec) {}
	void set(int hour, int min, int sec);//set(int ,int ,int )
	void tick();
	void show();
	void run()
	{
		while (h != 0 || m != 0 || s != 0) {
			tick();
			show();
		}
	}
	void run(bool& accordVar, bool& setVar);
	//void run(Calculate24* c24);
};


class Player
{
public:
	Player() : _socket(-1), _roomNum(-1), _seatNum(-1) {
		_synData._bAnswer = false;
		_synData._bOccupied = false;
		_synData._bOnline = false;
		_synData._bReady = false;
		_synData._bWinner = false;
		_synData._iScore = 100;
		strcpy(_synData._sName, "");
		strcpy(_synData._sHead, "");
	}
	~Player() = default;
	
	HSocket _socket;
	//bool _isOccupied;
	unsigned int _roomNum;
	unsigned int _seatNum;
	SPlayer _synData;
	//bool _isOnline;//new
	//bool _isReady;
	//bool _isAnswer;//new
	//bool _isWinner;//new
	//int _score;//new
	//std::string _name;
	//std::string _head;//new
	
};

class PlayerRoom
{
public:
	PlayerRoom() : _playerCount(0), _isGenThread(false), _bcanStart(false),
		_isStart(false), _qdLock(false), _isFinish(false), _isOver(false) {
		for (int i = 0; i < 4; i++)
		{
			_player[i] = Player();
		}
	};
	~PlayerRoom() = default;
	Player _player[4];
	//unsigned int _freeSeat;
	int _getFreeSeat(){
		for (int i = 0; i < 4; i++)
		{
			if (!_player[i]._synData._bOccupied)
			{
				return i;
			}
		}
		return -1;
	};
	/*bool _isFull()
	{
		for (int i = 0; i < 4; i++)
		{
			if (!_player[i]._isOccupied)
			{
				_freeSeat = i;
				return false;
			}
		}
		return true;
	};*/
	//控制相关
	int _playerCount;
	bool _isGenThread;
	bool _isStart;
	bool _isFinish;
	bool _qdLock;
	bool _isOver;
	bool _bcanStart;
	void _fcanStart() {
		//int playerCount = 0;
		bool allReady = true;
		for (int i = 0; i < 4; i++)
		{
			if (_player[i]._synData._bOccupied && !_player[i]._synData._bReady)
			{
				allReady = false;
				break;
			}
		}
		_bcanStart = (_playerCount >= 2 && allReady) ? true : false;
	};
};

class Calculate24
{
public:
	typedef std::shared_ptr<Calculate24> ptr;

	static Calculate24* getInstance();
	//Calculate24::Calculate24(const Calculate24&) = delete;
	void parseData(const char* data, HSocket socket);
	void sendData(DataType_S dataType);
	//void sendData(HSocket socket, DataType_S dataType);//sys info
	void sendData(HSocket socket, DataType_S dataType, SynType synType = SynType::NONE);

	void init();
	void startGame(int roomNum);
	std::string _genRandomPoker();
	void sendPoker(int roomNum, std::string poker);
	void sendResult(int roomNum) {};

	//void startGame();
	void startServer();
	void checkRoomNull();
	bool _roomNull;
	Clock _clock;

	SocketServer::ptr _server;
	unsigned int _readyCount;
	bool _isStart;
	bool _isFinish;

	bool _qdLock;
	bool _isWin;
	bool _isRunning;
	char _card[5];
	std::string _calcStr;
	std::string _userName;

	//GameData_S* _serverData;
	
	//std::map<const char*, HSocket> _mPlayers;
	
	std::vector<PlayerRoom> _vRoom;
	std::map<HSocket, Player> _mSockToPlayer;
	//Player _player[4];
//private:
//	class Clock {
//	private:
//		int h;
//		int m;
//		int s;
//	public:
//		Clock(int hour, int min, int sec) : h(hour), m(min), s(sec) {}
//		void set(int hour, int min, int sec)
//		{
//			h = hour;
//			m = min;
//			s = sec;
//		}
//		void tick()
//		{
//			time_t t = time(NULL);//取得当前时间
//			while (time(NULL) == t);
//			if (--s < 0) {
//				s = 59;
//				if (--m < 0) {
//					m = 59;
//					--h < 0;
//				}
//			}
//		}
//		void show()
//		{
//			std::cout << '\r';
//			if (h < 10)std::cout << 0;
//			std::cout << h << ':';
//			if (m < 10)std::cout << 0;
//			std::cout << m << ':';
//			if (s < 10)std::cout << 0;
//			std::cout << s << std::flush;
//		}
//		void run()
//		{
//			do
//			{
//				while (h != 0 || m != 0 || s != 0)
//				{
//					tick();
//					show();
//				}
//				_roomNull = true;
//				for (auto& i : _vRoom)
//				{
//					if (i._playerCount > 0)
//					{
//						_roomNull = false;
//						break;
//					}
//				}
//				if (_roomNull) {
//					break;
//				}
//				else {
//					set(0, 1, 0);
//				}
//			} while (true);
//		}
//
//	};
//
//	Clock _clock;

private:
	class AutoRelease 
	{
	public:
		AutoRelease() = default;
		~AutoRelease()
		{
			if (_c24) {  //如果_c24还存在就释放掉
				delete _c24;
				_c24 = nullptr;
				std::cout << "~AutoRelease()" << std::endl;
			}
		}
	};
	
private:
	Calculate24();
	~Calculate24();
	
	static AutoRelease _autoRelease;
	static Calculate24* _c24;

};

#endif // !__GAME24_H__