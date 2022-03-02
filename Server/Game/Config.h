#ifndef __Config_H__
#define __Config_H__

#include <iostream>
#include <string>
#include <vector>

enum class DataType_C
{
	LOGIN,
	REGISTER,
	STATE,
	QD,
	RESULT,
};

struct TypeTraits
{
	DataType_C typeC;
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
	char _sCard[5];
	//char pad[50];
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

#endif