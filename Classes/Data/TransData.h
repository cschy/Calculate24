#ifndef __TRANS_DATA_H__
#define __TRANS_DATA_H__

#include "cocos2d.h"
#include "LoginScene.h"
#include "GameScene.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>

USING_NS_CC;


enum class DataType_C
{
	LOGIN,
	REGISTER,
	STATE,
	QD,
	RESULT,
};

//struct UserInfo
//{
//	DataType_C dataType;
//	unsigned int dataSize;
//	std::string usrName;
//	std::string passWord;
//};

struct UserInfo
{
	DataType_C _eType;
	unsigned int _iSize;
	char _sName[17];
	char _spassWord[17];
	//char _sHead[15];
};

//struct GameData_C
//{
//	DataType_C dataType;
//	unsigned int dataSize;
//	std::string userName;
//	std::string calcStr;
//};



struct SendState_C
{
	DataType_C _eType;
	unsigned int _iSize;
	//char _sName[17];
	bool _bisReady;
};

struct SendCalc_C
{
	DataType_C _eType;
	unsigned int _iSize;
	char _sName[17];
	char _sCalculate[12];
};

struct SendResult_C
{
	DataType_C _eType;
	unsigned int _iSize;
	bool _bisWin;
};

enum class DataType_S
{
	DISALLOW_LOGIN,
	ALLOW_LOGIN,
	ALLOW_REGISTER,
	NQD,
	CARD,
	RESULT,
	SYN,
};


//struct GameData_S
//{
//	DataType_S dataType;
//	unsigned int dataSize;
//	char card[4];
//	char gameResult[50];
//};

enum class DisAllowLoginType
{
	SAME_NAME,
};

struct SendDisAllowLogin_S
{
	DataType_S _eType;
	unsigned int _iSize;
	DisAllowLoginType _eDisType;
};

struct SPlayer {
	bool _bOccupied;
	bool _bOnline;
	bool _bReady;
	bool _bAnswer;
	bool _bWinner;
	int _iScore;
	char _sName[17];
	char _sHead[15];
	
};

enum class SynType {
	NONE,
	ENTER,
	READY,
	POKER,
	QD,
	RESULT,
	RESET,
	LEAVE,
};

struct SendSynData_S {
	DataType_S _eType;
	unsigned int _iSize;
	SynType _eSynType;
	SPlayer _nPlayer[4];
};

struct SendAllowLogin_S//syn player
{
	DataType_S _eType;
	unsigned int _iSize;
	//std::vector<const char*> _vOthers;
	//char _sOthers[3][17];
	//char _sOthers[4][17];
	
	int _iRoom;
	
	//SPlayer _nPlayer[4];
};

struct SendAllowRegister_S
{
	DataType_S _eType;
	unsigned int _iSize;
};

struct SendCard_S
{
	DataType_S _eType;
	unsigned int _iSize;
	char _sCard[5];//多一个存边界'\0'
	//char pad[50];//没有这个就会内存越界，最后一个字符数组（没有'\0'）对socket传输的影响
};

struct SendNQD_S
{
	DataType_S _eType;
	unsigned int _iSize;
	char _sName[17];
};

struct SendResult_S
{
	DataType_S _eType;
	unsigned int _iSize;
	bool _bisWin;
};

//class Player
//{
//public:
//	Player():_sName(""), _iScore(100) {};
//	~Player() {};
//	std::string _sName;
//	unsigned int _iScore;
//};

class TransData : public Node
{
public:
	TransData();
	~TransData();

	static TransData* getInstance();
	void destoryInstance();

	void sendData(DataType_C dataType);
	void parseData(const char* data);

	LoginScene* _loginScene;
	GameScene* _gameScene;
	//UserInfo* _userInfo;
	std::string _userName;
	DisAllowLoginType _disAllowLoginType;
	//std::vector<const char*> _vOthers;
	//char _sOthers[3][17];
	//Player* _players[3];
	/*std::set<std::string> _playerSet;*/
	std::vector<std::string> _playerSet;

	std::string _tmpHeadImg;
	unsigned int _tmpRoomNum;
	int _tmpScore;
private:
	static TransData* _transData;
	
};

#endif // !__TRANS_DATA_H__
