#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "SceneManager.h"
#include "Socket/SocketClient.h"

#include "ui/CocosGUI.h"
using namespace ui;

#include <iostream>
#include <vector>
#include <list>
//#include <string>
#include <exception>

enum class __SynType {
	NONE,
	ENTER,
	READY,
	POKER,
	QD,
	RESULT,
	RESET,
	LEAVE,
};

struct InitFromServer{
	int _iRoom;
	int _iScore;
	std::string _sHead;
};

struct sPlayer
{
	bool _isOccupied;
	bool _isOnline;
	bool _isReady;
	bool _isAnswer;
	bool _isWinner;
	int _iScore;
	std::string _sName;
	std::string _sHead;
};

class CountDown : public ActionInterval
{
public:
	static CountDown* create(float duration);
	virtual void update(float time) override;
	virtual void startWithTarget(Node *target) override;
	virtual void stop() override;
	bool initWithDuration(float duration);
protected:
	int _count;
CC_CONSTRUCTOR_ACCESS:
	CountDown() {}
	virtual ~CountDown() {}
};

class HeadImage : public Button
{
public:
	static HeadImage* create();
	/*static HeadImage* create(const std::string& normalImage,
		const std::string& selectedImage = "",
		const std::string& disableImage = "",
		TextureResType texType = TextureResType::LOCAL);*/
	virtual bool init();
};

class GameScene;
class PlayerArea : public Layout
{
public:
	virtual bool init(const Size& size);
	static PlayerArea* create(const Size& size);
	sPlayer _synData = {false, false, false, false, false, -1, "", "",};
	bool _updateMask[8] = {false, false, false, false, false, false, false, false };
	
	void recvData(const sPlayer& data) {

		_updateMask[0] = (data._isOccupied != _synData._isOccupied);
		_updateMask[1] = (data._isOnline != _synData._isOnline);
		_updateMask[2] = (data._isReady != _synData._isReady);
		_updateMask[3] = (data._isAnswer != _synData._isAnswer);
		_updateMask[4] = (data._isWinner != _synData._isWinner);
		_updateMask[5] = (data._iScore != _synData._iScore);
		_updateMask[6] = (data._sName != _synData._sName);
		_updateMask[7] = (data._sHead != _synData._sHead);
		/*if (data._isOccupied != _synData._isOccupied)
		{
			_updateMask[0] = true;
		}
		if (data._isOnline != _synData._isOnline)
		{
			_updateMask[1] = true;
		}
		if (data._isReady != _synData._isReady)
		{
			_updateMask[2] = true;
		}
		if (data._isAnswer != _synData._isAnswer)
		{
			_updateMask[3] = true;
		}
		if (data._isWinner != _synData._isWinner)
		{
			_updateMask[4] = true;
		}
		if (data._iScore != _synData._iScore)
		{
			_updateMask[5] = true;
		}
		if (data._sName != _synData._sName)
		{
			_updateMask[6] = true;
		}
		if (data._sHead != _synData._sHead)
		{
			_updateMask[7] = true;
		}*/
		_synData = data;
		
		_needUpdateFromServer = true;
	}

	bool _needUpdateFromServer;
	void setPositionM(const Vec2& pos, bool isNormalized = false);
	enum class RELATIVE_POS {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
	};
	void setPositionR(Node* node, RELATIVE_POS rpos);
	/*void setScore(int score)
	{
		_score = score;
		_uiScore->setString(std::to_string(_score));
	}*/
	/*void loadHead() {
		_uiHead->loadTextureNormal("UI/Head/" + _synData._sHead);
	}
	void loadName() {
		_uiName->setString(_synData._sName);
	}
	void loadScore() {
		_uiScore->setString(std::to_string(_synData._iScore));
	}*/
	void setReadyVisible(bool isVisible) {
		if (isVisible)
		{
			_uiReady->setString(_synData._isReady ? "ready" : "cancel");
		}
		else {
			_uiReady->setString("");
		}
		
	}
	void setDrop()
	{
		_uiReady->setString("lose");
		_uiReady->setVisible(true);
	}

	HeadImage* _uiHead;
	Label* _uiName;
	//Sprite* _uiSprite;
	Label* _uiAnswerIng;//暂时用这个label代替
	Label* _uiScore;
	Label* _uiTimer;
	Label* _uiReady;
	GameScene* _gameScene;
};

class ControlLayer : public Layer
{
public:
	virtual bool init();
	static ControlLayer* create();
	GameScene* _gameScene;

	void setReadyButton(bool visible) {
		_rcToggle->setVisible(visible);
	}
	void setQDButton(bool visible) {
		_qdButton->setVisible(visible);
	}

	MenuItemToggle* _rcToggle;
	Button* _qdButton;
	Button* _disCardButton;
};

class HeroArea : public PlayerArea
{
public:
	virtual bool init(const Size& size);
	static HeroArea* create(const Size& size);

	MenuItemToggle* _rcToggle;
	Button* _qdButton;
	bool _canQD;
	void setReadyButton(bool visible) {
		_rcToggle->setVisible(visible);
	}
	void setQDButton(bool visible) {
		_qdButton->setVisible(visible);
	}
};

class Poker : public LayerColor
{
public:
	struct StateRecord {
		Texture2D* _texture;
		int _num;
		bool _isVisible;
	};
	enum DIRECTION {
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};
	enum OPERATOR {
		PLUS,
		MINUS,
		MULTIPLY,
		DIVIDE,
		NON,
	};
	virtual bool init();
	CREATE_FUNC(Poker);

	void addPicture(const std::string& poker, const std::string& plus, const std::string& minus, const std::string& multiply, const std::string& divide);

	Sprite* _uiPoker;
	Button* _uiPlus;
	Button* _uiMinus;
	Button* _uiMultiply;
	Button* _uiDivide;


	std::list<StateRecord> _stateList;
	std::list<StateRecord>::iterator _now;
	//StateRecord* _now;
	int _num;
	char _pchar;
	Vec2 _originPos;
	OPERATOR _op;
	//static Vector<Poker* >_vPokerLayout;
	void setOperatorVisible(bool isVisible)
	{
		_uiPlus->setVisible(isVisible);
		_uiMinus->setVisible(isVisible);
		_uiMultiply->setVisible(isVisible);
		_uiDivide->setVisible(isVisible);
	}
	void setPositionM(const Vec2& pos, DIRECTION plus, DIRECTION minus, DIRECTION multiply, DIRECTION divide);
	//Vector<Sprite*> _optSprite;
	Texture2D* _texture = nullptr;
	const std::string colorList[5] = { "spade", "heart", "club", "diamond" };
	
	void clearTexture()
	{
		Director::getInstance()->getTextureCache()->removeTexture(_texture);
	}

	void setData(char c) {
		_pchar = c;
		if (_pchar == 'A' || _pchar == 'J' || _pchar == 'Q' || _pchar == 'K'){
			_num = 1;
		}
		else if (_pchar == 'D') {
			_num = 10;
		}
		else{
			_num = _pchar - '0';
		}
		
		auto filePath = StringUtils::format("poker/%s/%c.jpg", colorList[rand() % 4].c_str(), _pchar);
		_texture = Director::getInstance()->getTextureCache()->addImage(filePath);
	}
	void flip()
	{
		_uiPoker->removeAllChildrenWithCleanup(true);
		_uiPoker->setTexture(_texture);
	}
};

class PokerArea : public Widget
{
public:
	struct OptRecord{
		Poker* _first;
		Poker* _second;
	};
	std::list<OptRecord> _recordList;
	//OptRecord* _now;
	std::list<OptRecord>::iterator _now;
	virtual bool init();
	bool _isWin;
	//CREATE_FUNC(PokerArea);
	static PokerArea* create(const Size& size);
	Poker* _poker[4];
	Poker* _currentPoker;
	Poker* _anotherPoker;

	bool _isTouchPoker;
	bool _isSelectOperator;
	//bool _isInteractPoker;

	EventListenerTouchOneByOne* _listener;
	Button* _undo;
	Button* _redo;
	GameScene* _gameScene;
	Label* _downLabel;
	
	//typedef int(&array)[10] d;
	char _data[5];
	bool setData(char(&poker)[5])
	{
		try{
			strcpy(_data, poker);
			srand(time(NULL));
			for (int i = 0; i < 4; i++)
			{
				_poker[i]->setData(_data[i]);
			}
		}
		catch(std::exception e){
			log("%s", e.what());
			return false;
		}
		return true;
	}
	//倒计时翻牌
	void flipPokersWithCountDown(int cd) {
		_downLabel->setVisible(true);
		_downLabel->runAction(Sequence::create(CountDown::create(cd), CallFunc::create([&]() {
			_downLabel->setVisible(false);
			setAllPokerVisible(true);
			//for (int i = 0; i < 4; i++)
			//{
			//	_poker[i]->setOperatorVisible(false);
			//	_poker[i]->flip();
			//	//_poker[i]->setVisible(true);
			//}
			//work();
		}), NULL));
	}
	
	void clear();
	bool getWin() const {
		return _isWin;
	}
	void initialize();
	void work() {
		clear();
		initialize();
	}

	void setAllPokerVisible(bool isVisible) {
		for (auto i : _poker)
		{
			i->setVisible(isVisible);
		}
	}
	
	void enableListener(bool en) {
		_listener->setEnabled(en);
	}
};

//负责背景和一些(按键)事件
class BackGroundLayer : public Layer
{
public:
	static BackGroundLayer* create(const std::string& filename);
	virtual bool init(const std::string& filename);

	Sprite* _backGroundImg;
	GameScene* _gameScene;
};

class NavArea : public ListView
{
public:
	static NavArea* create(const Size& size);
	virtual bool init();
	void mPushBackItem(Widget* item){
		item->setFlippedX(true);
		pushBackCustomItem(item);
	}

	const Size BUTTON_SIZE = Size(64, 64);
	GameScene* _gameScene;
};

class SystemNav : public NavArea
{
public:
	static SystemNav* create(const Size& size);
	virtual bool init();

	int _room;
	void setRoom(int room)
	{
		_room = room;
	}
	void displayRoom() {
		_uiRoom->setTitleText(StringUtils::format("R:%d", _room));
	}
	Button* _uiRoom;
	Button* _uiBack;
};

class UserNav : public NavArea
{
public:
	static UserNav* create(const Size& size);
	virtual bool init();
};


//class TransData;
class GameScene :public Scene
{
public:
	virtual bool init();
	//CREATE_FUNC(GameScene);
	GameScene() : _isInit(false) {}
	static GameScene* create();
	//void update(float delta);

	Texture2D * _texPlayer;
	Texture2D * _texHolder;
	//Menu* _rdyCelMenu;
	MenuItemToggle* _rcToggle;
	Button* _qdButton;
	Button* _ansButton;

	Sprite* _selfSprite;
	Sprite* _pokerSprite[4];
	Poker* _pokerLayout[4];//new
	PokerArea* _pokerArea;//new

	Label* _downLabel;
	EditBox* _ansBox;

	SceneManager* tsm;
	SocketClient* s_client;


	char _poker[5];
	char _qdName[17];
	unsigned int _qdPos;
	
	Label* _roomLabel;

	bool _needUpdatePlayer;
	bool _isReady;
	bool _isRecvPoker;
	bool _canQD;
	bool _isWin;
	bool _isOver;
	

	//std::string _usr;
	void initVar();
	/*void showOthers();
	void setPoker();
	void checkQD();
	void checkCalculate(const char* calcStr);
	void checkResult();*/
	//new
	bool getQD() const {
		return _canQD;
	}
	void setReady(bool ready)
	{
		_isReady = ready;
	}
	void setWin(bool win)
	{
		_isWin = win;
	}

	void checkRecvSynData();
	//void checkRecvPoker();
	PlayerArea* _playerArea[3];
	//HeroArea* _heroArea;
	PlayerArea* _hero;
	ControlLayer* _controlLayer;
	BackGroundLayer* _backGroundLayer;
	SystemNav* _systemNav;
	UserNav* _userNav;
	bool _isEnter;
	bool _isRecvSynData;
	bool _isInit;
	//std::string _name;
	//from server
	int _room;
	
	
	sPlayer _player[4];
	__SynType _synType;
	void needUpdateFromServer()
	{
		for (auto i : _playerArea)
		{
			if (i->_synData._isOccupied)
				i->_needUpdateFromServer = true;
		}
		_hero->_needUpdateFromServer = true;
	}

	void synEnter();
	void synReady();
	void synQD();
	void synPoker();
	void synResult();
	void synReset();
	void synLeave();
};



#endif