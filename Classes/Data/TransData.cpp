#include "TransData.h"
#include "Socket/SocketClient.h"

#include "ui/CocosGUI.h"
using namespace ui;

TransData* TransData::_transData = nullptr;

TransData::TransData() : _loginScene(nullptr), _gameScene(nullptr), _userName(""), _playerSet()
{
	/*for (int i = 0; i < 3; i++)
	{
		_players[i] = new Player();
	}*/
	
	
}
TransData::~TransData() 
{
	/*for (int i = 0; i < 3; i++)
	{
		delete _players[i];
	}*/
}

TransData* TransData::getInstance()
{
	if (_transData == nullptr)
	{
		_transData = new(std::nothrow) TransData;
		//_transData->autorelease();
	}
	return _transData;
}

void TransData::destoryInstance()
{
	//delete _userInfo;
	//_userInfo = nullptr;
	delete _transData;
	_transData = nullptr;
}

void TransData::parseData(const char* data)
{
	DataType_S* dataType = (DataType_S*)data;//type traits
	DataType_C* dataTypeC = (DataType_C*)data;//reuse client's send type

	if (*dataType == DataType_S::ALLOW_LOGIN)
	{
		SendAllowLogin_S* recvAllowLogin = (SendAllowLogin_S*)data;
		if (recvAllowLogin->_iSize == sizeof(SendAllowLogin_S))
		{
			_loginScene->_isAllowLogin = true;
			_loginScene->_loginSpinLock = false;
			//_playerSet.clear();
			//for (int i = 0; i < 4; i++)//3->4
			//{
			//	if (recvAllowLogin->_sOthers[i][0] != '\0' && strcmp(recvAllowLogin->_sOthers[i], _userName))
			//	{
			//		log("player[%d]: %s, userName: %s", i, recvAllowLogin->_sOthers[i], _userName);
			//		//_playerSet.insert(recvAllowLogin->_sOthers[i]);
			//		_playerSet.push_back(recvAllowLogin->_sOthers[i]);
			//	}
			//}
			//log("other player size: %d", _playerSet.size());

			/*_tmpScore = recvAllowLogin->_iScore;
			_tmpHeadImg = recvAllowLogin->_sHead;
			_tmpRoomNum = recvAllowLogin->_iRoom;*/

			while (!_gameScene);
			while (!_gameScene->_isInit);
			//_gameScene->_isEnter = true;
			//_gameScene->_needUpdatePlayer = true;//nullptr
			//_gameScene->_name = _userName;

			//recv system info
			_gameScene->_room = recvAllowLogin->_iRoom;
			_gameScene->_isEnter = true;
			//syn player info
			
			//for (int i = 0; i < 4; i++)
			//{
			//	//可以直接赋值吗？_gameScene->_player[i] = recvAllowLogin->_nPlayer[i]，待实验
			//	_gameScene->_player[i] = { recvAllowLogin->_nPlayer[i]._bOccupied, recvAllowLogin->_nPlayer[i]._bOnline, recvAllowLogin->_nPlayer[i]._bReady, 
			//		recvAllowLogin->_nPlayer[i]._bAnswer, recvAllowLogin->_nPlayer[i]._bWinner, recvAllowLogin->_nPlayer[i]._iScore, recvAllowLogin->_nPlayer[i]._sName, recvAllowLogin->_nPlayer[i]._sHead };
			//}

			//_gameScene->_isEnter = true;
		}
	}

	else if (*dataType == DataType_S::SYN)
	{
		SendSynData_S* recvSynData = (SendSynData_S*)data;
		if (recvSynData->_iSize == sizeof(SendSynData_S))
		{
			std::cout << "recv syn data" << std::endl;
			
			for (int i = 0; i < 4; i++)
			{
				std::cout << recvSynData->_nPlayer[i]._sName << std::endl;
				/*if (TransData::getInstance()->_userName == recvSynData->_nPlayer[i]._sName)
				{
					std::cout << recvSynData->_nPlayer[i]._sHead << std::endl;
				}*/
				//可以直接赋值吗？_gameScene->_player[i] = recvAllowLogin->_nPlayer[i]，待实验
				_gameScene->_player[i] = { recvSynData->_nPlayer[i]._bOccupied, recvSynData->_nPlayer[i]._bOnline, recvSynData->_nPlayer[i]._bReady,
					recvSynData->_nPlayer[i]._bAnswer, recvSynData->_nPlayer[i]._bWinner, recvSynData->_nPlayer[i]._iScore, recvSynData->_nPlayer[i]._sName, recvSynData->_nPlayer[i]._sHead };
			}
			_gameScene->_synType = __SynType(int(recvSynData->_eSynType));
			_gameScene->_isRecvSynData = true;
		}
	}

	else if (*dataType == DataType_S::DISALLOW_LOGIN)
	{
		SendDisAllowLogin_S* recvDisAllowLogin = (SendDisAllowLogin_S*)data;
		if (recvDisAllowLogin->_iSize == sizeof(SendDisAllowLogin_S))
		{
			_disAllowLoginType = recvDisAllowLogin->_eDisType;
			_loginScene->_isAllowLogin = false;
			_loginScene->_loginSpinLock = false;
		}
	}

	else if (*dataType == DataType_S::ALLOW_REGISTER)
	{
		SendAllowRegister_S* recvAllowRegister = (SendAllowRegister_S*)data;
		if (recvAllowRegister->_iSize == sizeof(SendAllowRegister_S))
		{
			_loginScene->_isAllowRegister = true;
		}
	}

	else if (*dataType == DataType_S::CARD)
	{
		/*SendCard_S recvCard;
		memset(&recvCard, 0, sizeof(recvCard));
		memcpy(&recvCard, data, sizeof(recvCard));
		if (recvCard._iSize == sizeof(SendCard_S))
		{
			std::cout << "Game start && Card are: " << recvCard._sCard << std::endl;
			strcpy_s(_gameScene->_poker, strlen(recvCard._sCard) + 1, recvCard._sCard);
			std::cout << "_poker is: " << _gameScene->_poker << std::endl;
			_gameScene->_isRecvPoker = true;
		}*/
		SendCard_S* recvCard = (SendCard_S*)data;
		if (recvCard->_iSize == sizeof(SendCard_S))
		{
			std::cout << "Game start && Card are: " << recvCard->_sCard << std::endl;
			//strcpy_s(_gameScene->_poker, strlen(recvCard->_sCard) + 1, recvCard->_sCard);
			strcpy(_gameScene->_poker, recvCard->_sCard);
			std::cout << "_poker is: " << _gameScene->_poker << std::endl;
			//_gameScene->_isRecvPoker = true;
			_gameScene->_synType = __SynType(int(SynType::POKER));
			_gameScene->_isRecvSynData = true;
		}

	}

	else if (*dataType == DataType_S::NQD)
	{
		SendNQD_S* recvNQD = (SendNQD_S*)data;
		if (recvNQD->_iSize == sizeof(SendNQD_S))
		{
			strcpy_s(_gameScene->_qdName, recvNQD->_sName);
			_gameScene->_canQD = false;
		}
	}

	else if (*dataType == DataType_S::RESULT)
	{
		SendResult_S* recvResult = (SendResult_S*)data;
		if (recvResult->_iSize == sizeof(SendResult_S))
		{
			_gameScene->_isOver = true;
			_gameScene->_isWin = recvResult->_bisWin;
		}

	}
	
	//server perfect forward client' send data, like std::forward
	
}

void TransData::sendData(DataType_C dataType)
{
	//GameData_C gameData;
	//gameData.dataType = dataType;
	//gameData.dataSize = sizeof(GameData_C);
	//gameData.userName = getInstance()->_userInfo->usrName;//同样在这里

	if (dataType == DataType_C::LOGIN || dataType == DataType_C::REGISTER)
	{
		UserInfo userInfo = { dataType, sizeof(UserInfo), "", ""};
		strcpy(userInfo._sName, _loginScene->_accountBox->getText());
		strcpy(userInfo._spassWord, "");
		//strcpy_s(userInfo._spassWord, _loginScene->_passwordBox->getText());
		//以下注释代码待做实验：根据接受数据先看前面保存的类型强制转换
		SocketClient::getInstance()->sendMessage((const char*)&userInfo, sizeof(UserInfo));
		
		//SocketClient::getInstance()->sendMessage((const char*)&gameData, sizeof(GameData_C));
	}
	else if (dataType == DataType_C::STATE)//可以不用发名字，服务器根据socket就可以知道是谁了
	{
		SendState_C sendState = { dataType, sizeof(SendState_C), _gameScene->_isReady };
		
		SocketClient::getInstance()->sendMessage((const char*)&sendState, sizeof(SendState_C));
	}
	
	else if (dataType == DataType_C::QD)//尝试不发结构体，直接发既定类型
	{
		SocketClient::getInstance()->sendMessage((const char*)&dataType, sizeof(DataType_C));
	}

	else if (dataType == DataType_C::RESULT)
	{
		SendResult_C sendResult = { dataType, sizeof(SendResult_C), _gameScene->_isWin };
		SocketClient::getInstance()->sendMessage((const char*)&sendResult, sizeof(SendResult_C));
	}

	/*else if (dataType == DataType_C::CALCULATE)
	{
		SendCalc_C sendCalc = { dataType, sizeof(SendCalc_C), "", "" };
		strcpy_s(sendCalc._sName, _userName);
		strcpy_s(sendCalc._sCalculate, _gameScene->_ansBox->getText());
		SocketClient::getInstance()->sendMessage((const char*)&sendCalc, sizeof(SendCalc_C));
	}*/
	//SocketClient::getInstance()->sendMessage((const char*)&gameData, sizeof(GameData_C));
}