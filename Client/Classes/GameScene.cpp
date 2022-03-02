#include "GameScene.h"
#include "Data/TransData.h"

#include "ui/CocosGUI.h"
using namespace ui;

#include <iostream>
#include <time.h>
#include <exception>

CountDown* CountDown::create(float duration)
{
	CountDown *countDown = new (std::nothrow) CountDown();
	if (countDown && countDown->initWithDuration(duration))
	{
		countDown->autorelease();
		return countDown;
	}
	delete countDown;
	return nullptr;
}

bool CountDown::initWithDuration(float duration)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_count = duration;
		return true;
	}
	return false;
}

void CountDown::stop()
{
	if (nullptr != _target)
		((Label*)_target)->setString("0");
	ActionInterval::stop();
}

void CountDown::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
}

void CountDown::update(float time)
{
	if (_target && !isDone())
	{
		float slice = 1.0f / _count;
		float m = fmodf(time, slice);
		//_target->setVisible(m > slice / 2 ? true : false);
		if (m < slice / 2)
		{
			((Label*)_target)->setString(StringUtils::format("%d", int((1 - time)*_count + 1)));
			//_label->setString(StringUtils::format("%d", int((1 - time)*_count + 1)));
		}
	}
}

BackGroundLayer* BackGroundLayer::create(const std::string& filename)
{
	BackGroundLayer *ret = new (std::nothrow) BackGroundLayer();
	if (ret && ret->init(filename))
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool BackGroundLayer::init(const std::string& filename)
{
	if (!Layer::init())
	{
		return false;
	}
	_backGroundImg = Sprite::create("UI/BackGround/1.jpg");
	_backGroundImg->setPositionNormalized(Vec2(0.5, 0.5));
	addChild(_backGroundImg);

	return true;
}

HeadImage* HeadImage::create()
{
	HeadImage* headImage = new (std::nothrow) HeadImage();
	if (headImage && headImage->init())
	{
		headImage->autorelease();
		return headImage;
	}
	CC_SAFE_DELETE(headImage);
	return nullptr;
}

//HeadImage* HeadImage::create(const std::string &normalImage, const std::string& selectedImage, const std::string& disableImage, TextureResType texType)
//{
//	HeadImage *headImage = new (std::nothrow) HeadImage;
//	if (headImage && headImage->init(normalImage, selectedImage, disableImage, texType) && headImage->init())
//	{
//		headImage->autorelease();
//		return headImage;
//	}
//	CC_SAFE_DELETE(headImage);
//	return nullptr;
//}

bool HeadImage::init()
{
	if (!Button::init())
	{
		return false;
	}
	//setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	return true;
}

PlayerArea* PlayerArea::create(const Size& size)
{
	PlayerArea* widget = new (std::nothrow) PlayerArea();
	if (widget && widget->init(size))
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

ControlLayer* ControlLayer::create()
{
	ControlLayer *ret = new (std::nothrow) ControlLayer();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

HeroArea* HeroArea::create(const Size& size)
{
	HeroArea* widget = new (std::nothrow) HeroArea();

	widget->setContentSize(size);
	if (widget && widget->init(size))
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

PokerArea* PokerArea::create(const Size& size)
{
	PokerArea* widget = new (std::nothrow) PokerArea();

	widget->setContentSize(size);
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

NavArea* NavArea::create(const Size& size)
{
	NavArea* widget = new (std::nothrow) NavArea();
	
	if (widget && widget->init())
	{
		widget->setContentSize(size);
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

SystemNav* SystemNav::create(const Size& size)
{
	SystemNav* widget = new (std::nothrow) SystemNav();
	
	if (widget && widget->init())
	{
		widget->setContentSize(size);
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

UserNav* UserNav::create(const Size& size)
{
	UserNav* widget = new (std::nothrow) UserNav();
	
	if (widget && widget->init())
	{
		widget->setContentSize(size);
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

bool NavArea::init()
{
	if (!ListView::init())
	{
		return false;
	}
	setFlippedX(true);
	setDirection(ScrollView::Direction::HORIZONTAL);
	setGravity(ListView::Gravity::CENTER_VERTICAL);
	setBounceEnabled(true);
	setBackGroundImage("cocosui/green_edit.png");
	setBackGroundImageScale9Enabled(true);

	setScrollBarPositionFromCorner(Vec2(7, 7));
	setItemsMargin(2.0f);
	setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	setMagneticType(ListView::MagneticType::LEFT);

	

	auto pButton = Button::create("cocosui/button.png", "cocosui/buttonHighlighted.png");
	pButton->setContentSize(BUTTON_SIZE);
	pButton->setScale9Enabled(true);
	pButton->setTitleText("panel");
	mPushBackItem(pButton);


	return true;
}

bool SystemNav::init()
{
	if (!NavArea::init())
	{
		return false;
	}

	//房间信息
	_uiRoom = Button::create("UI/panle_1071@2x.png");
	_uiRoom->setContentSize(BUTTON_SIZE);
	_uiRoom->setScale9Enabled(true);
	_uiRoom->setTitleText("R");
	_uiRoom->setTitleFontSize(24);
	_uiRoom->setTitleColor(Color3B::YELLOW);
	mPushBackItem(_uiRoom);

	//返回键
	_uiBack = Button::create("UI/back_normal.png", "UI/back_select.png");
	_uiBack->setContentSize(BUTTON_SIZE);
	_uiBack->setScale9Enabled(true);
	//_uiBack->setTitleText("back");
	mPushBackItem(_uiBack);
	_uiBack->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
		case Widget::TouchEventType::MOVED:
		case Widget::TouchEventType::CANCELED:
			break;
		case Widget::TouchEventType::ENDED:
			log("close connection!");
			_gameScene->s_client->closeConnectPublic();
			TransData::getInstance()->destoryInstance();
			_gameScene->tsm->goLoginScene();
			break;
		}
		});

	return true;
}

bool UserNav::init()
{
	if (!NavArea::init())
	{
		return false;
	}

	

	return true;
}

//void PlayerArea::checkUpdateFromServer()
//{
//	schedule([&](float dt) {
//		if (_updateFromServer && _synData._isOccupied)
//		{
//			_updateFromServer = false;
//
//			if (_synData._isOnline){
//				
//				_uiHead->loadTextureNormal(_synData._sHead);
//				_uiScore->setString(std::to_string(_synData._iScore));
//				_uiName->setString(_synData._sName);
//				if (_synData._isReady) {
//					_uiReady->setString("ready");
//				}
//				else {
//					_uiReady->setString("cancel");
//				}
//				
//			}
//			else {
//				//_uiHead->loadTextureDisabled();
//				_uiReady->setString("lose");
//			}
//			
//
//		}
//	}, 0.0f, kRepeatForever, 0.0f, "check_data_from_server");
//}

void PlayerArea::setPositionM(const Vec2& pos, bool isNormalized)
{
	isNormalized ? setPositionNormalized(pos) : setPosition(pos);
	//头像：正中间
	_uiHead->setPosition(Vec2(getContentSize().width * 0.5, getContentSize().height * 0.5));
	//分数：头像左上角  
	_uiScore->setPosition(_uiHead->getLeftBoundary(), _uiHead->getTopBoundary());
	//准备状态：头像右上角
	_uiReady->setPosition(_uiHead->getRightBoundary(), _uiHead->getTopBoundary());
	//作答标志：头像底部
	_uiAnswerIng->setPosition(_uiHead->getPositionX(), _uiHead->getBottomBoundary() + _uiAnswerIng->getLineHeight() * 0.5);
	//名字：头像下方
	_uiName->setPosition(_uiHead->getPositionX(), _uiHead->getBottomBoundary() - _uiName->getLineHeight() * 0.5);
	//计时器：头像上方
	_uiTimer->setPosition(_uiHead->getPositionX(), _uiHead->getTopBoundary() + _uiTimer->getLineHeight() * 0.5);

	//_uiName->setColor(Color3B::BLUE);
	
	//log("font size: %f, %f, %f, %f", _uiName->getMaxLineWidth(), _uiName->getWidth(), _uiName->getContentSize().width, _uiName->getLineSpacing());
}

void PlayerArea::setPositionR(Node* node, RELATIVE_POS rpos)
{
	switch (rpos)
	{
	case PlayerArea::RELATIVE_POS::TOP:
		//setPosition(Vec2(node->getContentSize().width * 0.5, node->getBoundingBox().getMaxY() + ));
		break;
	case PlayerArea::RELATIVE_POS::BOTTOM:
		break;
	case PlayerArea::RELATIVE_POS::LEFT:
		break;
	case PlayerArea::RELATIVE_POS::RIGHT:
		break;
	default:
		break;
	}
}

bool PlayerArea::init(const Size& size)
{
	if (!Layout::init())
	{
		return false;
	}
	setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	setContentSize(size);
	//setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//setBackGroundColor(Color3B::RED);
	
	_needUpdateFromServer = false;
	
	
	_uiHead = HeadImage::create();
	_uiHead->loadTextureNormal("UI/Head/17.png");
	addChild(_uiHead);

	_uiScore = Label::createWithTTF("", "fonts/arial.ttf", 16);
	_uiScore->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_uiScore->setTextColor(Color4B::BLACK);
	addChild(_uiScore);

	_uiReady = Label::createWithTTF("", "fonts/arial.ttf", 16);
	_uiReady->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_uiReady->setTextColor(Color4B::BLACK);
	addChild(_uiReady);

	_uiAnswerIng = Label::createWithTTF("Answering...", "fonts/arial.ttf", 16);
	_uiAnswerIng->setVisible(false);
	addChild(_uiAnswerIng);

	_uiName = Label::createWithTTF("", "fonts/arial.ttf", 24);
	_uiName->setTextColor(Color4B(178, 34, 34, 255));
	addChild(_uiName);
	
	_uiTimer = Label::createWithTTF("10", "fonts/arial.ttf", 16);
	_uiTimer->setVisible(false);
	addChild(_uiTimer);

	schedule([&](float dt) {
		if (_needUpdateFromServer)
		{
			_needUpdateFromServer = false;

			if (_synData._isOccupied) {
				if (_synData._isOnline) {
					//更新部分
					if (_updateMask[0])//初始化进入游戏
					{
						log("init enter room, head: %s", _synData._sHead.c_str());
						_uiHead->loadTextureNormal(_synData._sHead);
						_uiName->setString(_synData._sName);
						_uiReady->setString(_synData._isReady ? "ready" : "cancel");
						_uiScore->setString(std::to_string(_synData._iScore));
					}
					if (_updateMask[1])//重新连接
					{
						_uiHead->loadTextureNormal(_synData._sHead);
						_uiReady->setString("cancel");
					}
					if (_updateMask[2])//更新ready
					{
						_uiReady->setString(_synData._isReady ? "ready" : "cancel");
						//_uiReady->setVisible(true);
					}
					if (_updateMask[3])//更新qd answer
					{
						//_uiReady->setString("");
						_uiAnswerIng->setVisible(true);

						//倒计时10s
						_uiTimer->setVisible(true);
						_uiTimer->runAction(Sequence::create(CountDown::create(10), CallFunc::create([&]() {
							if (this == _gameScene->_hero)
							{
								_gameScene->setWin(false);
								TransData::getInstance()->sendData(DataType_C::RESULT);
							}
						}), NULL));
					}
					if (_updateMask[4])//更新winner
					{
						log("winner: %s", _synData._sName.c_str());
						
						/*if (_uiTimer->isVisible())
						{
							_uiTimer->stopAllActions();
							_uiTimer->setVisible(false);
						}*/
					}
					if (_updateMask[5])//更新score
					{
						_uiScore->setString(std::to_string(_synData._iScore));
					}
					if (_updateMask[6])//更新name
					{
						_uiName->setString(_synData._sName);
					}
					if (_updateMask[7])//更新head
					{
						_uiHead->loadTextureNormal(_synData._sHead);
					}
					
				}
				else {
					if (_updateMask[1])//掉线
					{
						//_uiHead->loadTextureDisabled();//设置头像为灰色
						_uiReady->setString("lose");//状态显示掉线
					}
				}

			}
			else {
				//设置头像为占位头像
				if (_updateMask[0])//退出游戏
				{
					log("%s leave the room", _synData._sName.c_str());
					_uiHead->loadTextureNormal(_synData._sHead);
					_uiName->setString("");
					_uiReady->setString("");
					_uiScore->setString("");
					_uiTimer->setVisible(false);
					_uiAnswerIng->setVisible(false);

					_synData._iScore = -1;
					//_synData = { false, false, false, false, false, -1, "", "", };
				}

			}

		}
	}, 0.0f, kRepeatForever, 0.0f, "update_data_from_server");

	return true;
}


bool ControlLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	//准备取消按钮、抢答按钮、放弃按钮
	auto readyButton = MenuItemImage::create("UI/ready.png", "UI/ready.png");
	auto cancelButton = MenuItemImage::create("UI/cancel.png", "UI/cancel.png");
	_rcToggle = MenuItemToggle::createWithCallback([&](Ref* pSender) {
		switch (((MenuItemToggle *)pSender)->getSelectedIndex())
		{
		case 1:
			_gameScene->setReady(true);
			TransData::getInstance()->sendData(DataType_C::STATE);
			log("Waiting for other players!");
			break;
		case 0:
			_gameScene->setReady(false);
			TransData::getInstance()->sendData(DataType_C::STATE);
			break;
		default:
			break;
		}
	}, readyButton, cancelButton, NULL);
	//_rcToggle->setPositionNormalized(Vec2(0.7, 0.3));
	_rcToggle->setSelectedIndex(0);
	auto rdyCelMenu = Menu::create(_rcToggle, NULL);
	rdyCelMenu->setPositionNormalized(Vec2(0.7, 0.3));
	rdyCelMenu->setContentSize(_rcToggle->getContentSize());
	addChild(rdyCelMenu);

	_qdButton = Button::create("UI/qd_normal.png", "UI/qd_select.png");
	_qdButton->setScale9Enabled(true);
	_qdButton->setPositionNormalized(Vec2(0.7, 0.7));	//
	_qdButton->setVisible(false);
	addChild(_qdButton);
	_qdButton->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
		case Widget::TouchEventType::MOVED:
		case Widget::TouchEventType::CANCELED:
			break;
		case Widget::TouchEventType::ENDED:
			if (_gameScene->getQD()){
				TransData::getInstance()->sendData(DataType_C::QD);
			}
			else {
				_qdButton->setVisible(false);
			}
			break;
		}
	});

	return true;
}

bool HeroArea::init(const Size& size)
{
	if (!PlayerArea::init(size))
	{
		return false;
	}
	_canQD = true;
	//setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//setColor(Color3B::RED);
	//checkUpdateFromServer();
	

	//准备取消按钮、抢答按钮、放弃按钮
	auto readyButton = MenuItemImage::create("UI/ready.png", "UI/ready.png");
	auto cancelButton = MenuItemImage::create("UI/cancel.png", "UI/cancel.png");
	_rcToggle = MenuItemToggle::createWithCallback([&](Ref* pSender) {
		switch (((MenuItemToggle *)pSender)->getSelectedIndex())
		{
		case 1:
			_gameScene->setReady(true);
			TransData::getInstance()->sendData(DataType_C::STATE);
			log("Waiting for other players!");
			break;
		case 0:
			_gameScene->setReady(false);
			TransData::getInstance()->sendData(DataType_C::STATE);
			break;
		default:
			break;
		}
	},readyButton, cancelButton, NULL);
	_rcToggle->setPositionNormalized(Vec2(0.7, 0.3));
	_rcToggle->setSelectedIndex(0);
	auto rdyCelMenu = Menu::create(_rcToggle, NULL);
	rdyCelMenu->setPosition(Point::ZERO);
	rdyCelMenu->setContentSize(getContentSize());
	addChild(rdyCelMenu);

	_qdButton = Button::create("UI/qd_normal.png", "UI/qd_select.png");
	//this->addChild(qdButton);->setScale(0.5f);
	_qdButton->setPositionNormalized(Vec2(0.7, 0.7));
	_qdButton->setVisible(false);
	addChild(_qdButton);
	_qdButton->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
		case Widget::TouchEventType::MOVED:
		case Widget::TouchEventType::CANCELED:
			break;
		case Widget::TouchEventType::ENDED:

			if (_canQD)
			{
				TransData::getInstance()->sendData(DataType_C::QD);
			}
			//else
			//{
			//	//log("%s is qding!", _gameScene->_qdName);
			//}
			////代码过渡竞争
			//if (_canQD)
			//{
			//	_gameScene->_pokerArea->enableListener(true);	//启动用户操作
			//	this->schedule([&](float dt) {
			//		if (_gameScene->_pokerArea->_isWin)
			//		{
			//			_gameScene->_isWin = true;
			//			TransData::getInstance()->sendData(DataType_C::RESULT);
			//			_gameScene->_isOver = true;
			//			unschedule("check24");
			//		}
			//	}, 0.0f, kRepeatForever, 0.0f, "check24");


			//	_qdButton->setVisible(false);
			//	//_ansButton->setVisible(true);
			//	//倒计时10s
			//	auto cd = CountDown::create(10);
			//	_uiTimer->setVisible(true);
			//	_uiTimer->runAction(Sequence::create(cd, CallFunc::create([this]() {
			//		if (_uiTimer->isVisible())//说明超时
			//		{
			//			unschedule("check24");
			//			//_downLabel->setVisible(false);
			//			TransData::getInstance()->sendData(DataType_C::RESULT);
			//			_gameScene->_isOver = true;//注释了的效果同上，因为服务器代码只同步了其他玩家的输赢(_isOver=true)
			//		}
			//		}), NULL));
			//}
			break;
		}
	});


	return true;
}

bool PokerArea::init()
{
	if (!Widget::init())
	{
		return false;
	}
	//一次性配置
	//setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	setColor(Color3B::RED);

	//_isTouchPoker = false;
	//_isSelectOperator = false;

	_downLabel = Label::createWithTTF("", "fonts/arial.ttf", 36);
	_downLabel->setPositionNormalized(Vec2::ANCHOR_MIDDLE);
	_downLabel->setVisible(false);
	addChild(_downLabel, 1);

	//撤销键
	_undo = Button::create("UI/back_normal.png", "UI/back_select.png");
	_undo->setPositionNormalized(Vec2(0.4, 0.8));
	_undo->setVisible(false);
	_undo->addClickEventListener([&](Ref* pSender) {
		//if (!_now->_first && !_now->_second)
		if (_now == prev(_recordList.end()))
		{
			_undo->setVisible(false);
			return;
		}
		_now->_first->_now++;
		_now->_second->_now++;

		_now->_first->_uiPoker->setTexture(_now->_first->_now->_texture);
		_now->_first->_num = _now->_first->_now->_num;
		_now->_first->setVisible(_now->_first->_now->_isVisible);

		_now->_second->_uiPoker->removeAllChildrenWithCleanup(true);
		_now->_second->_uiPoker->setTexture(_now->_second->_now->_texture);
		_now->_second->_num = _now->_second->_now->_num;
		_now->_second->setVisible(_now->_second->_now->_isVisible);

		++_now;

		if (_now == prev(_recordList.end()))
		{
			_undo->setVisible(false);
		}
		//保证redo在最后
		_redo->setVisible(true);
		});
	addChild(_undo);
	/*_undo->addTouchEventListener([](Ref* pSender, Widget::TouchEventType t) {		//如果上面click事件在安卓上不行则试试这个
		switch (t)
		{
		case ui::Widget::TouchEventType::BEGAN:

			break;
		case ui::Widget::TouchEventType::MOVED:
			break;
		case ui::Widget::TouchEventType::ENDED:
			break;
		case ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
	});*/

	//重做键
	_redo = Button::create("UI/back_normal.png", "UI/back_select.png");
	_redo->setPositionNormalized(Vec2(0.6, 0.8));
	_redo->setVisible(false);
	_redo->addClickEventListener([&](Ref* pSender) {
		if (_now == _recordList.begin())
		{
			_redo->setVisible(false);
			return;
		}
		--_now;
		_now->_first->_now--;
		_now->_second->_now--;

		_now->_first->_uiPoker->setTexture(_now->_first->_now->_texture);
		_now->_first->_num = _now->_first->_now->_num;
		_now->_first->setVisible(_now->_first->_now->_isVisible);

		_now->_second->_uiPoker->setTexture(_now->_second->_now->_texture);
		_now->_second->_num = _now->_second->_now->_num;
		_now->_second->setVisible(_now->_second->_now->_isVisible);

		if (_now == _recordList.begin())
		{
			_redo->setVisible(false);
		}
		//保证undo在最后
		_undo->setVisible(true);
		});
	addChild(_redo);

	float pokerWidth = getContentSize().width / 8;
	for (int i = 0; i < 4; i++)
	{
		_poker[i] = Poker::create();
		_poker[i]->addPicture("poker/pokerback.png", "UI/plus.png", "UI/minus.png", "UI/multiply.png", "UI/divide.png");
		Vec2 pos = Vec2(pokerWidth * (i + i + 1), getContentSize().height * 0.5);
		_poker[i]->setPositionM(pos, Poker::DIRECTION::RIGHT, Poker::DIRECTION::LEFT, Poker::DIRECTION::UP, Poker::DIRECTION::DOWN);
		addChild(_poker[i]);
		_poker[i]->setName(std::to_string(i));
	}

	_listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = [&](Touch* t, Event* e) {
		_isTouchPoker = false;
		_isSelectOperator = false;
		for (auto i : _poker)
		{
			//log("%f, %f, %f, %f,", i->getBoundingBox().getMinX(), i->getBoundingBox().getMaxX(), i->getBoundingBox().getMinY(), i->getBoundingBox().getMaxY());
			Vec2 localPos = i->convertToNodeSpace(t->getLocation());
			if (i->_uiPoker->getBoundingBox().containsPoint(localPos))
			{
				_currentPoker = i;
				_currentPoker->setOperatorVisible(true);
				log("get current poker: %s", _currentPoker->getName().c_str());
				_isTouchPoker = true;
				break;
			}
		}
		return true;
	};

	_listener->onTouchMoved = [&](Touch* t, Event* e) {
		//log("%f, %f", t->getLocation().x, t->getLocation().y);
		if (_isSelectOperator) {
			_currentPoker->setPosition(convertToNodeSpace(t->getLocation()));
		}
		else {
			if (_isTouchPoker) {
				//convertTouchToNodeSpace(t)
				Vec2 localPos = _currentPoker->convertToNodeSpace(t->getLocation());
				if (_currentPoker->_uiPlus->getBoundingBox().containsPoint(localPos)) {
					log("op: plus");
					ActionInterval* jumpTo = JumpTo::create(1.5, _currentPoker->_uiPlus->getPosition(), _currentPoker->_uiPlus->getContentSize().height, 3);
					_currentPoker->_uiPlus->runAction(jumpTo);

					_currentPoker->_op = Poker::OPERATOR::PLUS;
					_isSelectOperator = true;
				}
				else if (_currentPoker->_uiMinus->getBoundingBox().containsPoint(localPos)) {
					log("op: minus");
					ActionInterval* jumpTo = JumpTo::create(1.5, _currentPoker->_uiMinus->getPosition(), _currentPoker->_uiMinus->getContentSize().height, 3);
					_currentPoker->_uiMinus->runAction(jumpTo);

					_currentPoker->_op = Poker::OPERATOR::MINUS;
					_isSelectOperator = true;
				}
				else if (_currentPoker->_uiMultiply->getBoundingBox().containsPoint(localPos)) {
					log("op: multiply");
					ActionInterval* jumpTo = JumpTo::create(1.5, _currentPoker->_uiMultiply->getPosition(), _currentPoker->_uiMultiply->getContentSize().height, 3);
					_currentPoker->_uiMultiply->runAction(jumpTo);

					_currentPoker->_op = Poker::OPERATOR::MULTIPLY;
					_isSelectOperator = true;
				}
				else if (_currentPoker->_uiDivide->getBoundingBox().containsPoint(localPos)) {
					log("op: divide");
					ActionInterval* jumpTo = JumpTo::create(1.5, _currentPoker->_uiDivide->getPosition(), _currentPoker->_uiDivide->getContentSize().height, 3);
					_currentPoker->_uiDivide->runAction(jumpTo);

					_currentPoker->_op = Poker::OPERATOR::DIVIDE;
					_isSelectOperator = true;
				}
				else {
					//log("op: none");
					_currentPoker->_op = Poker::OPERATOR::NON;
					_isSelectOperator = false;
				}
			}
			else {
				//log("not select poker");
			}
		}

	};
	//this->getChildByName("poker");
	_listener->onTouchEnded = [&](Touch* t, Event* e) {
		if (_isSelectOperator) {
			//_anotherPoker = nullptr;
			bool _isInteractPoker = false;
			Rect currentRect(_currentPoker->convertToWorldSpace(_currentPoker->_uiPoker->getPosition()), _currentPoker->_uiPoker->getContentSize());
			//log("current: %f, %f, %f, %f,", currentRect.getMinX(), currentRect.getMaxX(), currentRect.getMinY(), currentRect.getMaxY());
			for (auto i : _poker)
			{
				Rect iRect(i->convertToWorldSpace(i->_uiPoker->getPosition()), i->_uiPoker->getContentSize());
				if (i->isVisible() && i != _currentPoker && currentRect.intersectsRect(iRect))
				{
					//log("i: %f, %f, %f, %f,", iRect.getMinX(), iRect.getMaxX(), iRect.getMinY(), iRect.getMaxY());
					_anotherPoker = i;
					_isInteractPoker = true;
					break;
				}
			}

			if (_isInteractPoker) {
				_undo->setVisible(true);
				_redo->setVisible(false);

				_currentPoker->setOperatorVisible(false);
				_currentPoker->setVisible(false);

				for (auto it = _recordList.begin(); it != _now;)
				{
					Director::getInstance()->getTextureCache()->removeTextureForKey(it->_first->getName() + '#' + it->_second->getName());
					_recordList.erase(it++);
				}
				//_recordList.erase(_recordList.begin(), _now);
				_recordList.push_front({ _currentPoker, _anotherPoker });
				_now = _recordList.begin();

				switch (_currentPoker->_op)
				{
				case Poker::OPERATOR::PLUS:
					log("idx: %s, %d + %d = %d", _anotherPoker->getName().c_str(), _currentPoker->_num, _anotherPoker->_num, _currentPoker->_num + _anotherPoker->_num);
					_anotherPoker->_num = _currentPoker->_num + _anotherPoker->_num;
					break;
				case Poker::OPERATOR::MINUS:
					log("idx: %s, %d - %d = %d", _anotherPoker->getName().c_str(), _currentPoker->_num, _anotherPoker->_num, _currentPoker->_num - _anotherPoker->_num);
					_anotherPoker->_num = _currentPoker->_num - _anotherPoker->_num;
					break;
				case Poker::OPERATOR::MULTIPLY:
					log("idx: %s, %d * %d = %d", _anotherPoker->getName().c_str(), _currentPoker->_num, _anotherPoker->_num, _currentPoker->_num * _anotherPoker->_num);
					_anotherPoker->_num = _currentPoker->_num * _anotherPoker->_num;
					break;
				case Poker::OPERATOR::DIVIDE:
					log("idx: %s, %d / %d = %d", _anotherPoker->getName().c_str(), _currentPoker->_num, _anotherPoker->_num, _currentPoker->_num / _anotherPoker->_num);
					_anotherPoker->_num = _currentPoker->_num / _anotherPoker->_num;
					break;
				case Poker::OPERATOR::NON:
					log("non operator: should not be here");
					break;
				default:
					log("other operator: should not be here");
					break;
				}

				//check result
				if (_anotherPoker->_num == 24)
				{
					bool chosenOne = true;
					for (auto i : _poker)
					{
						if (i != _anotherPoker)
						{
							if (i->isVisible())
							{
								chosenOne = false;
								break;
							}
						}
					}
					if (chosenOne)
					{
						_isWin = true;
						//_gameScene->_isWin = true;
						//可以加一些特效给这张24
					}
				}
				//改变poker
				_anotherPoker->_uiPoker->removeAllChildrenWithCleanup(true);
				_anotherPoker->_uiPoker->setTexture(Sprite::create("poker/pokerback.png")->getTexture());
				//结果标签
				auto resultLabel = Label::createWithTTF(std::to_string(_anotherPoker->_num), "fonts/arial.ttf", 48);
				resultLabel->setPositionNormalized(Vec2(0.5, 0.5));
				resultLabel->setColor(Color3B::RED);
				_anotherPoker->_uiPoker->addChild(resultLabel);
				//捕获
				utils::captureNode(_anotherPoker->_uiPoker, [&](Image* img) {
					Director::getInstance()->getTextureCache()->addImage(img, _currentPoker->getName() + '#' + _anotherPoker->getName());
					});
				scheduleOnce([&](float dt) {
					//不scheduleOnce得不到纹理
					_anotherPoker->_stateList.erase(_anotherPoker->_stateList.begin(), _anotherPoker->_now);
					_anotherPoker->_stateList.push_front({ Director::getInstance()->getTextureCache()->getTextureForKey(_currentPoker->getName() + '#' + _anotherPoker->getName()), _anotherPoker->_num, _anotherPoker->isVisible(), });
					_anotherPoker->_now = _anotherPoker->_stateList.begin();
					//主动数其实只改变了不可见
					_currentPoker->_stateList.erase(_currentPoker->_stateList.begin(), _currentPoker->_now);
					_currentPoker->_stateList.push_front({ _currentPoker->_uiPoker->getTexture(), _currentPoker->_num, _currentPoker->isVisible() });
					_currentPoker->_now = _currentPoker->_stateList.begin();
					}, 0.0f, "Capture-Texture");
				//currentPoker->_uiPoker->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey("sdf"));
				//currentPoker->_uiPoker->setTexture(anotherPoker->_textureList.back());

			}
			else {
				log("not contains another poker");
			}

			_currentPoker->setPosition(_currentPoker->_originPos);

			//_isTouchPoker = false;
			//_isSelectOperator = false;
		}
		else {
			if (_isTouchPoker) {
				_currentPoker->setOperatorVisible(false);
			}
			else {
				//log("non current poker");
			}
		}


	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
	_listener->setEnabled(false);

	return true;
}

void PokerArea::clear()
{
	for (auto i : _poker)
	{
		i->_stateList.clear();
		//clear texture
		i->clearTexture();
		i->_uiPoker->removeAllChildrenWithCleanup(true);
	}
	//还要对TextureCache进行清理
	for (auto it = _recordList.begin(); it != _recordList.end(); ++it)
	{
		if (it->_first && it->_second)
		{
			Director::getInstance()->getTextureCache()->removeTextureForKey(it->_first->getName() + '#' + it->_second->getName());
		}
	}
	_recordList.clear();

}

void PokerArea::initialize()
{
	//control
	_isWin = false;
	//_listener->setEnabled(false);

	//view
	_undo->setVisible(false);
	_redo->setVisible(false);
	setAllPokerVisible(true);

	_recordList.push_front({ nullptr, nullptr });
	_now = _recordList.begin();
	for (auto i : _poker)
	{
		i->setOperatorVisible(false);
		i->_uiPoker->setTexture(i->_texture);
		i->_stateList.push_front({ i->_uiPoker->getTexture(), i->_num, i->isVisible() });
		i->_now = i->_stateList.begin();
	}
}


void Poker::setPositionM(const Vec2& pos, Poker::DIRECTION plus, Poker::DIRECTION minus,
	Poker::DIRECTION multiply, Poker::DIRECTION divide)
{
	_originPos = pos;
	setPosition(pos);

	_uiPoker->setPositionNormalized(Vec2(0.5, 0.5));
	

	switch (plus)
	{
	case Poker::UP:
		_uiPlus->setAnchorPoint(Vec2(0.5, 1));
		_uiPlus->setPositionNormalized(Vec2(0.5, 1));
		break;
	case Poker::DOWN:
		_uiPlus->setAnchorPoint(Vec2(0.5, 0));
		_uiPlus->setPositionNormalized(Vec2(0.5, 0));
		break;
	case Poker::LEFT:
		_uiPlus->setAnchorPoint(Vec2(0, 0.5));
		_uiPlus->setPositionNormalized(Vec2(0, 0.5));
		break;
	case Poker::RIGHT:
		_uiPlus->setAnchorPoint(Vec2(1, 0.5));
		_uiPlus->setPositionNormalized(Vec2(1, 0.5));
		break;
	default:
		break;
	}

	switch (minus)
	{
	case Poker::UP:
		_uiMinus->setAnchorPoint(Vec2(0.5, 1));
		_uiMinus->setPositionNormalized(Vec2(0.5, 1));
		break;
	case Poker::DOWN:
		_uiMinus->setAnchorPoint(Vec2(0.5, 0));
		_uiMinus->setPositionNormalized(Vec2(0.5, 0));
		break;
	case Poker::LEFT:
		_uiMinus->setAnchorPoint(Vec2(0, 0.5));
		_uiMinus->setPositionNormalized(Vec2(0, 0.5));
		break;
	case Poker::RIGHT:
		_uiMinus->setAnchorPoint(Vec2(1, 0.5));
		_uiMinus->setPositionNormalized(Vec2(1, 0.5));
		break;
	default:
		break;
	}

	switch (multiply)
	{
	case Poker::UP:
		_uiMultiply->setAnchorPoint(Vec2(0.5, 1));
		_uiMultiply->setPositionNormalized(Vec2(0.5, 1));
		break;
	case Poker::DOWN:
		_uiMultiply->setAnchorPoint(Vec2(0.5, 0));
		_uiMultiply->setPositionNormalized(Vec2(0.5, 0));
		break;
	case Poker::LEFT:
		_uiMultiply->setAnchorPoint(Vec2(0, 0.5));
		_uiMultiply->setPositionNormalized(Vec2(0, 0.5));
		break;
	case Poker::RIGHT:
		_uiMultiply->setAnchorPoint(Vec2(1, 0.5));
		_uiMultiply->setPositionNormalized(Vec2(1, 0.5));
		break;
	default:
		break;
	}

	switch (divide)
	{
	case Poker::UP:
		_uiDivide->setAnchorPoint(Vec2(0.5, 1));
		_uiDivide->setPositionNormalized(Vec2(0.5, 1));
		break;
	case Poker::DOWN:
		_uiDivide->setAnchorPoint(Vec2(0.5, 0));
		_uiDivide->setPositionNormalized(Vec2(0.5, 0));
		break;
	case Poker::LEFT:
		_uiDivide->setAnchorPoint(Vec2(0, 0.5));
		_uiDivide->setPositionNormalized(Vec2(0, 0.5));
		break;
	case Poker::RIGHT:
		_uiDivide->setAnchorPoint(Vec2(1, 0.5));
		_uiDivide->setPositionNormalized(Vec2(1, 0.5));
		break;
	default:
		break;
	}
}

void Poker::addPicture(const std::string& poker, const std::string& plus, const std::string& minus, const std::string& multiply, const std::string& divide)
{
	_uiPoker->setTexture(Director::getInstance()->getTextureCache()->addImage(poker));
	_uiPlus->loadTextures(plus, plus);
	_uiMinus->loadTextures(minus, minus);
	_uiMultiply->loadTextures(multiply, multiply);
	_uiDivide->loadTextures(divide, divide);
	setContentSize(_uiPoker->getContentSize() + _uiPlus->getContentSize() * 2);
}

//Vector<Poker* > Poker::_vPokerLayout = {};
bool Poker::init()
{
	if (!LayerColor::init())
	{
		return false;
	}
	setColor(Color3B::BLUE);
	setIgnoreAnchorPointForPosition(false);
	setAnchorPoint(Vec2(0.5, 0.5));
	
	
	_uiPoker = Sprite::create("poker/1.jpg");
	addChild(_uiPoker);
	_uiPlus = Button::create("UI/minus.png");
	addChild(_uiPlus);
	_uiMinus = Button::create("UI/plus.png");
	addChild(_uiMinus);
	_uiMultiply = Button::create("UI/divide.png");
	addChild(_uiMultiply);
	_uiDivide = Button::create("UI/multiply.png");
	addChild(_uiDivide);

	/*_uiPoker->setName("poker");
	_uiPlus->setName("plus");
	_uiMinus->setName("minus");
	_uiMultiply->setName("multiply");
	_uiDivide->setName("divide");*/

	setOperatorVisible(false);

	//static const std::string posName[] = {"top", "buttom", "left", "right"};
	//for (const std::string& i : posName)
	//{
	//	Sprite* sprite = Sprite::create();
	//	addChild(sprite);
	//	_optSprite.pushBack(sprite);

	//	sprite->setName(i);//方位
	//}
	//

	/*auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch, Event* event) {
		return true;
	};
	listener->onTouchMoved = [this](Touch* touch, Event* event) {
		if (touchedControls)
			touchedControls->setPosition(touch->getLocation());
	};
	listener->onTouchEnded = [this](Touch* touch, Event* event) {

	};*/

	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//_uiPlus = Sprite::create();
	////_uiPlus->setPosition
	//_uiPlus->setPositionNormalized(Vec2(0.5, 0.5));
	//_uiPlus->setLocalZOrder(-1);

	//this->addChild(_uiPlus);
	return true;
}

void GameScene::initVar()
{

}

void GameScene::synEnter()
{
	
}
void GameScene::synReady()
{
	
}
void GameScene::synPoker()
{
	log("have recved poker!");
	//1.对pokerArea进行清理
	_pokerArea->enableListener(false);
	_pokerArea->clear();

	//2.复制数据给所需数据的模块并初始化
	if (!_pokerArea->setData(_poker))
	{
		log("failed: copy data to pokerArea");
	}
	_pokerArea->initialize();
	_pokerArea->setAllPokerVisible(false);

	//3.控制流程
	for (auto i : _playerArea)
	{
		i->setReadyVisible(false);
	}
	_hero->setReadyVisible(false);
	_controlLayer->setReadyButton(false);		//设置准备取消按钮失效
	
	_pokerArea->flipPokersWithCountDown(3);		//倒计时3秒翻牌

	_controlLayer->setQDButton(true);
	//checkResult();
}
void GameScene::synQD()
{
	_controlLayer->setQDButton(false);

	if (_hero->_synData._isAnswer)//自己在作答
	{
		//_pokerArea->work();
		_pokerArea->enableListener(true);	//启动用户操作

		this->schedule([&](float dt) {
			if (_pokerArea->getWin())
			{
				_isWin = true;
				
				TransData::getInstance()->sendData(DataType_C::RESULT);
				//_isOver = true;
				unschedule("check24");
			}
		}, 0.0f, kRepeatForever, 0.0f, "check24");

		
		//auto cd = CountDown::create(10);
		//_heroArea->_uiTimer->setVisible(true);
		//_heroArea->_uiTimer->runAction(Sequence::create(cd, CallFunc::create([this]() {
		//	if (_heroArea->_uiTimer->isVisible())//说明超时
		//	{
		//		unschedule("check24");
		//		//_downLabel->setVisible(false);
		//		TransData::getInstance()->sendData(DataType_C::RESULT);
		//		//_isOver = true;//注释了的效果同上，因为服务器代码只同步了其他玩家的输赢(_isOver=true)
		//	}
		//}), NULL));
	}
	/*else
	{
		for (auto i : _playerArea)
		{
			if (i->_synData._isAnswer)
			{
				auto cd = CountDown::create(10);
				i->_uiTimer->setVisible(true);
				i->_uiTimer->runAction(Sequence::create(cd, CallFunc::create([&]() {
					i->_uiTimer->setVisible(false);
				}), NULL));
				break;
			}
		}
	}*/
}
void GameScene::synResult()
{
	//把回答者的计时器和回答标志清楚
	if (_hero->_synData._isAnswer)
	{
		unschedule("check24");
		_hero->_uiTimer->stopAllActions();
		_hero->_uiTimer->setVisible(false);
		_hero->_uiAnswerIng->setVisible(false);
	}
	else
	{
		for (auto i : _playerArea)
		{
			if (i->_synData._isAnswer)
			{
				i->_uiTimer->stopAllActions();
				i->_uiTimer->setVisible(false);
				i->_uiAnswerIng->setVisible(false);
				break;
			}
		}
	}

	//客户端同步服务器的数据还原
	for (auto i : _playerArea)
	{
		i->_synData._isReady = false;
		i->_synData._isAnswer = false;
		i->_synData._isWinner = false;
	}
	_hero->_synData._isReady = false;
	_hero->_synData._isAnswer = false;
	_hero->_synData._isWinner = false;
}
void GameScene::synReset()
{
	for (auto i : _playerArea)
	{
		if (i->_synData._isOccupied)
		{
			if (i->_synData._isOnline)
			{
				i->setReadyVisible(true);
			}
			else {
				i->setDrop();
			}
		}
	}
	
	_hero->setReadyVisible(true);

	_controlLayer->_rcToggle->setSelectedIndex(0);
	_controlLayer->setReadyButton(true);
}
void GameScene::synLeave()
{

}

void GameScene::checkRecvSynData()
{
	schedule([&](float dt) {
		if (_isRecvSynData)
		{
			_isRecvSynData = false;

			int i = 0;
			for (const auto& p : _player)
			{
				if (p._sName != TransData::getInstance()->_userName && i < 3){
					//_playerArea[i]->_synData = p;
					_playerArea[i]->recvData(p);
					i++;
				}
				else {
					//log("hero head: %s", p._sHead.c_str());
					_hero->recvData(p);
					//_heroArea->_updateFromServer = true;
				}
			}

			switch (_synType)
			{
			case __SynType::NONE:
				break;
			case __SynType::ENTER:
				synEnter();
				break;
			case __SynType::READY:
				synReady();
				break;
			case __SynType::POKER:
				synPoker();
				break;
			case __SynType::QD:
				synQD();
				break;
			case __SynType::RESULT:
				synResult();
				break;
			case __SynType::RESET:
				synReset();
				break;
			case __SynType::LEAVE:
				synLeave();
				break;
			default:
				break;
			}
		}

	}, 0.0f, kRepeatForever, 0.0f, "checkRecvSynData");
}

//void GameScene::checkRecvPoker()
//{
//	schedule([&](float dt) {
//		if (_isRecvPoker)
//		{
//			_isRecvPoker = false;
//			log("have recved poker!");
//			//1.复制数据给所需数据的模块
//			if (!_pokerArea->setData(_poker)) 
//			{
//				log("failed: copy data to pokerArea");
//			}
//			//2.控制流程
//			_heroArea->setWork4Ready(false);		//设置准备取消按钮失效
//			_pokerArea->flipPokersWithCountDown(3);		//倒计时3秒翻牌
//		
//			checkQD();
//			checkResult();
//			//unschedule("recv_poker");
//		}
//		}, 0.0f, kRepeatForever, 0.0f, "recv_poker");
//}

GameScene* GameScene::create()
{
	GameScene *gameScene = new (std::nothrow) GameScene();
	if (gameScene && gameScene->init())
	{
		gameScene->_isInit = true;
		gameScene->autorelease();
		return gameScene;
	}
	else
	{
		CC_SAFE_DELETE(gameScene);
		return nullptr;
	}
}

bool GameScene::init()//主要存放游戏运行所需的数据成员，具体的逻辑功能则又其他区代理实现
{
	if (!Scene::init())
	{
		return false;
	}
	TransData::getInstance()->_gameScene = this;	//用以从服务器设置游戏状态
	s_client = SocketClient::getInstance();

	//_roomNum = 100;//可扩展成连接的基本信息，用一个struct来装，比如IP信息、作者信息等
	_qdPos = 3;
	_needUpdatePlayer = false;
	_isReady = false;
	
	
	_isOver = false;
	_isWin = false;
	
	_texHolder = Director::getInstance()->getTextureCache()->addImage("player-placeholder.png");
	_texPlayer = Director::getInstance()->getTextureCache()->addImage("player.png");
	
	//new
	_isEnter = false;
	_isRecvSynData = false;
	//_isRecvPoker = false;
	//_canQD = true;

	Size vSize = Director::getInstance()->getVisibleSize();

	//背景层
	_backGroundLayer = BackGroundLayer::create("UI/BackGround/1.jpg");
	_backGroundLayer->setPosition(Vec2::ZERO);
	addChild(_backGroundLayer, -1, "_backGroundLayer");
	_backGroundLayer->_gameScene = this;

	//系统导航功能区
	_systemNav = SystemNav::create(Size(vSize.width, 64));
	_systemNav->setPosition(Vec2(vSize.width * 0.5, vSize.height - _systemNav->getContentSize().height * 0.5));
	addChild(_systemNav, _systemNav->getLocalZOrder(), "_systemNav");
	_systemNav->_gameScene = this;

	//用户导航功能区
	_userNav = UserNav::create(Size(vSize.width, 64));
	_userNav->setPosition(Vec2(vSize.width * 0.5, _userNav->getContentSize().height * 0.5));
	addChild(_userNav, _userNav->getLocalZOrder(), "_userNav");
	_userNav->_gameScene = this;

	//牌区
	const Size __POKERS_SIZE__ = Size(720, 320);
	_pokerArea = PokerArea::create(__POKERS_SIZE__);//Size(vSize.width * 0.75, vSize.height * 0.4)
	_pokerArea->setPositionNormalized(Vec2(0.5, 0.5));
	addChild(_pokerArea, _pokerArea->getLocalZOrder(), "_pokerArea");
	_pokerArea->_gameScene = this;


	const Size __PLAYER_SIZE__ = Size(160, 160);
	//对手区
	_playerArea[0] = PlayerArea::create(__PLAYER_SIZE__);
	_playerArea[0]->setPositionM(Vec2(vSize.width * 0.5, _systemNav->getBottomBoundary() - _playerArea[0]->getContentSize().height * 0.5), false);
	addChild(_playerArea[0], _playerArea[0]->getLocalZOrder(), "_playerArea[0]");
	_playerArea[0]->_gameScene = this;
	//log("csize:%f", _playerArea[0]->getContentSize().width);

	_playerArea[1] = PlayerArea::create(__PLAYER_SIZE__);
	//_playerArea[1]->setPositionM(Vec2(1 / 16, 0.5), true);
	_playerArea[1]->setPositionM(Vec2(_playerArea[1]->getContentSize().width * 0.5, vSize.height * 0.5), false);
	_playerArea[1]->_gameScene = this;
	addChild(_playerArea[1], _playerArea[1]->getLocalZOrder(), "_playerArea[1]");
	
	_playerArea[2] = PlayerArea::create(__PLAYER_SIZE__);
	_playerArea[2]->setPositionM(Vec2(vSize.width - _playerArea[2]->getContentSize().width * 0.5, vSize.height * 0.5), false);
	_playerArea[2]->_gameScene = this;
	addChild(_playerArea[2], _playerArea[2]->getLocalZOrder(), "_playerArea[2]");

	//主角区
	_hero = PlayerArea::create(__PLAYER_SIZE__);
	_hero->setPositionM(Vec2(vSize.width * 0.5, _userNav->getTopBoundary() + _hero->getContentSize().height * 0.5));
	addChild(_hero, _hero->getLocalZOrder(), "_hero");
	_hero->_gameScene = this;

	_controlLayer = ControlLayer::create();
	_controlLayer->setPosition(Vec2::ZERO);
	addChild(_controlLayer, _controlLayer->getLocalZOrder(), "_controlLayer");
	_controlLayer->_gameScene = this;
	//_heroArea->_name = TransData::getInstance()->_userName;

	schedule([&](float dt) {
		if (_isEnter)
		{
			_systemNav->setRoom(_room);
			_systemNav->displayRoom();
			unschedule("checkEnter");
		}

	}, 0.0f, kRepeatForever, 0.0f, "checkEnter");

	checkRecvSynData();
	//checkRecvPoker();
	//_color = { "spade", "heart", "club", "diamond" };
	////down label
	//_downLabel = Label::createWithTTF("", "fonts/arial.ttf", 48);
	//_downLabel->setColor(Color3B(124, 252, 0));
	//_downLabel->setPosition(visibleSize.width / 2, visibleSize.height * 0.8);
	//this->addChild(_downLabel, 1);
	
	
	////bg
	//auto bg = Sprite::create("UI/BackGround/1.jpg");
	//bg->setPositionNormalized(Vec2(0.5, 0.5));
	//addChild(bg, -1);

	//room number
	/*auto room = Sprite::create("UI/panle_1071@2x.png");
	room->setPosition(room->getContentSize().width, vSize.height - room->getContentSize().height);
	addChild(room);
	_roomLabel = Label::createWithTTF("", "fonts/arial.ttf", 48);
	_roomLabel->setColor(Color3B(255, 0, 0));
	_roomLabel->setPosition(room->getPositionX() + room->getContentSize().width, room->getPositionY());
	addChild(_roomLabel);*/
	
	//_pokerArea = PokerArea::create();
	////pa->setContentSize(Size(540, 320));
	//_pokerArea->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));
	////log("pos:%f, %f, size:%f,%f", pa->getPositionX(), pa->getPositionY(), pa->getContentSize().width, pa->getContentSize().height);
	//
	//addChild(_pokerArea);

	//checkQD();
	//checkResult();
	
	//ready && cancel button
	/*auto readyButton = MenuItemImage::create("UI/ready.png", "UI/ready.png");
	auto cancelButton = MenuItemImage::create("UI/cancel.png", "UI/cancel.png");
	_rcToggle = MenuItemToggle::createWithCallback([&](Ref* pSender) {
		switch (((MenuItemToggle *)pSender)->getSelectedIndex())
		{
		case 1:
			_isReady = true;
			TransData::getInstance()->sendData(DataType_C::STATE);
			log("Waiting for other players!");
			break;
		case 0:
			_isReady = false;
			TransData::getInstance()->sendData(DataType_C::STATE);
			break;
		default:
			break;
		}
	},readyButton, cancelButton, NULL);
	_rcToggle->setPosition(vSize.width*0.7, vSize.height*0.3);
	_rcToggle->setSelectedIndex(0);
	auto rdyCelMenu = Menu::create(_rcToggle, NULL);
	rdyCelMenu->setPosition(Point::ZERO);
	addChild(rdyCelMenu);*/

	
	//聊天框(以前的答案框)
	//_ansBox = EditBox::create(Size(200, 35), Scale9Sprite::create("UI/editbox.png"));
	//_ansBox->setPosition(Vec2(vSize.width*0.5, _ansBox->getLayoutSize().height));
	//_ansBox->setPlaceHolder("Chat:");//占位字符
	//_ansBox->setMaxLength(256);
	////editbox1->setFontColor(Color3B::BLACK);//设置输入字体的颜色
	////_ansBox->setTag(1);
	//this->addChild(_ansBox);
	////send button
	//_ansButton = Button::create("UI/send.png", "UI/send.png");
	////this->addChild(ansButton);->setScale(0.5f);
	//_ansButton->setName("ansButton");
	//_ansButton->setPosition(Vec2(_ansBox->getBoundingBox().getMaxX() + _ansButton->getLayoutSize().width, _ansBox->getPositionY()));
	//_ansButton->setVisible(false);
	//this->addChild(_ansButton);
	//_ansButton->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
	//	switch (type)
	//	{
	//	case Widget::TouchEventType::BEGAN:
	//	case Widget::TouchEventType::MOVED:
	//	case Widget::TouchEventType::CANCELED:
	//		break;
	//	case Widget::TouchEventType::ENDED:
	//		//check calculate
	//		/*_ansButton->setVisible(false);*/
	//		//checkCalculate(_ansBox->getText());
	//		//TransData::getInstance()->sendData(DataType_C::RESULT);
	//		//_isOver = true;//注释了收不到You win或You lose的消息
	//		break;
	//	}
	//});

	//qiang da button
	//_qdButton = Button::create("UI/qd_normal.png", "UI/qd_select.png");
	////this->addChild(qdButton);->setScale(0.5f);
	//_qdButton->setPosition(Vec2(vSize.width*0.7, vSize.height*0.7));
	//_qdButton->setVisible(false);
	//this->addChild(_qdButton);
	//_qdButton->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
	//	switch (type)
	//	{
	//	case Widget::TouchEventType::BEGAN:
	//	case Widget::TouchEventType::MOVED:
	//	case Widget::TouchEventType::CANCELED:
	//		break;
	//	case Widget::TouchEventType::ENDED:
	//		
	//		if (_canQD)
	//		{
	//			TransData::getInstance()->sendData(DataType_C::QD);
	//		}
	//		else
	//		{
	//			log("%s is qding!", _qdName);
	//		}
	//		//代码过渡竞争
	//		if (_canQD)
	//		{
	//			_pokerArea->enableListener(true);	//启动用户操作
	//			this->schedule([&](float dt) {
	//				if (_pokerArea->_isWin)
	//				{
	//					_isWin = true;
	//					TransData::getInstance()->sendData(DataType_C::RESULT);
	//					_isOver = true;
	//					unschedule("check24");
	//				}
	//			}, 0.0f, kRepeatForever, 0.0f, "check24");


	//			_qdButton->setVisible(false);
	//			//_ansButton->setVisible(true);
	//			//倒计时10s
	//			auto cd = CountDown::create(10);
	//			_downLabel->setVisible(true);
	//			_downLabel->runAction(Sequence::create(cd, CallFunc::create([this]() {
	//				if (_downLabel->isVisible())//说明超时
	//				{
	//					unschedule("check24");
	//					//_downLabel->setVisible(false);
	//					TransData::getInstance()->sendData(DataType_C::RESULT);
	//					_isOver = true;//注释了的效果同上，因为服务器代码只同步了其他玩家的输赢(_isOver=true)
	//				}
	//			}), NULL));
	//		}
	//		break;
	//	}
	//});

	////close connection
	//Button* closecon = Button::create("UI/back_normal.png", "UI/back_select.png");
	//closecon->setPosition(Vec2(vSize.width*0.8, vSize.height*0.8));
	//this->addChild(closecon);
	//
	//closecon->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
	//	switch (type)
	//	{
	//	case Widget::TouchEventType::BEGAN:
	//	case Widget::TouchEventType::MOVED:
	//	case Widget::TouchEventType::CANCELED:
	//		break;
	//	case Widget::TouchEventType::ENDED:
	//		log("close connection!");
	//		s_client->closeConnectPublic();
	//		TransData::getInstance()->destoryInstance();
	//		tsm->goLoginScene();
	//		break;
	//	}
	//});
	//self
	//srand(time(NULL));
	//_selfSprite = Sprite::create(StringUtils::format("UI/Head/%d.png", rand() % 24));
	//_selfSprite->setPosition(vSize.width*0.5, vSize.height*0.2);
	////_selfSprite->setLocalZOrder(-1);
	//this->addChild(_selfSprite, -1);
	//auto selfName = Label::createWithTTF(TransData::getInstance()->_userName, "fonts/arial.ttf", 24);
	//selfName->setTag(1);
	//selfName->setPositionNormalized(Vec2(0.5, 0.5));
	//selfName->setColor(Color3B(138, 43, 226));
	//_selfSprite->addChild(selfName);
	//auto selfScore = Label::createWithTTF("100", "fonts/arial.ttf", 24);
	//selfScore->setTag(2);
	//selfScore->setPositionNormalized(Vec2(0.5, 0.2));
	//selfScore->setColor(Color3B(138, 43, 226));
	//_selfSprite->addChild(selfScore);

	////对手
	//for (int i = 0; i < 3; i++)
	//{
	//	_players[i] = Player::create();
	//	_players[i]->setContentSize(_players[i]->_uiSprite->getContentSize());
	//	_players[i]->setAnchorPoint(Vec2(0.5,0.5));
	//	//this->addChild(_players[i]);
	//}
	//_players[0]->setPosition(visibleSize.width*0.5, visibleSize.height*0.8);
	//_players[1]->setPosition(visibleSize.width*0.1, visibleSize.height*0.5);
	//_players[2]->setPosition(visibleSize.width*0.9, visibleSize.height*0.5);
	//for (int i = 0; i < 3; i++)
	//{
	//	this->addChild(_players[i]);
	//}
	//showOthers();
	//setPoker();
	return true;
}

//void GameScene::update(float delta)
//{
//	
//}
//
//void GameScene::showOthers()
//{
//	this->schedule([&](float dt) {
//		if (_needUpdatePlayer)
//		{
//			_needUpdatePlayer = false;
//			_roomLabel->setString(StringUtils::format("%d", _roomNum));
//			for (int i = 0; i < 3; i++)
//			{
//				if (i < TransData::getInstance()->_playerSet.size())
//				{
//					_players[i]->_isOn = true;
//					_players[i]->_uiSprite->setTexture(_texPlayer);
//					_players[i]->_sName = TransData::getInstance()->_playerSet[i];
//					_players[i]->_uiName->setString(TransData::getInstance()->_playerSet[i]);
//					_players[i]->_uiScore->setVisible(true);
//				}
//				else
//				{
//					_players[i]->_isOn = false;
//					_players[i]->_uiSprite->setTexture(_texHolder);
//					_players[i]->_sName = "";
//					_players[i]->_uiName->setString("");
//					_players[i]->_uiScore->setVisible(false);
//				}
//			}
//			
//		}
//		//静态插入法
//		/*for (auto &player : TransData::getInstance()->_playerSet)
//		{
//			bool isFind = false;
//			bool haveSeat = false;
//			int seat;
//			for (int i = 0; i < 3; i++)
//			{
//				if (_players[i]->_sName == player)
//				{
//					isFind = true;
//					break;
//				}
//				if (!_players[i]->_isOn)
//				{
//					haveSeat = true;
//					seat = i;
//				}
//			}
//			if (!isFind && haveSeat)
//			{
//				_players[seat]->_isOn = true;
//				_players[seat]->_sName = player;
//				_players[seat]->_uiSprite->setTexture(pic);
//				_players[seat]->_uiName->setString(player);
//				_players[seat]->_uiScore->setVisible(true);
//			}
//		}*/
//		
//	}, 0.0f, kRepeatForever, 0.0f, "showOthers");
//}
//
//void GameScene::setPoker()
//{
//	srand(time(NULL));
//	static std::string colorList[] = { "spade", "heart", "club", "diamond" };
//	this->schedule([&](float dt) {
//		if (_isRecvPoker)
//		{
//			_isRecvPoker = false;
//			log("have recved poker!");
//			_rcToggle->setVisible(false);
//			//倒计时3秒翻牌
//			auto cd = CountDown::create(3);
//			_downLabel->setVisible(true);
//			_downLabel->runAction(Sequence::create(cd, CallFunc::create([this]() {
//				_downLabel->setVisible(false);
//				for (int i = 0; i < 4; i++)
//				{
//					std::string colorPath;
//					if (_poker[i] == '1')
//					{
//						colorPath = StringUtils::format("poker/%s/A.jpg", colorList[rand() % 4].c_str());
//						_pokerArea->_poker[i]->_num = 1;
//					}
//					else if (_poker[i] == 'D')
//					{
//						colorPath = StringUtils::format("poker/%s/10.jpg", colorList[rand() % 4].c_str());
//						_pokerArea->_poker[i]->_num = 10;
//					}
//					else
//					{
//						colorPath = StringUtils::format("poker/%s/%c.jpg", colorList[rand() % 4].c_str(), _poker[i]);
//						if (_poker[i] == 'J' || _poker[i] == 'Q' || _poker[i] == 'K'){
//							_pokerArea->_poker[i]->_num = 1;
//						}
//						else {
//							_pokerArea->_poker[i]->_num = _poker[i] - '0';
//						}
//					}
//					Texture2D * pic = Director::getInstance()->getTextureCache()->addImage(colorPath);
//					//_pokerSprite[i]->setTexture(pic);
//					//((Sprite*)(_pokerArea->_poker[i]->getChildByName("poker")))->setTexture(pic);
//					/*_pokerArea->_poker[i]->setVisible(true);*/
//					_pokerArea->_poker[i]->_uiPoker->setTexture(pic);
//					//_pokerLayout[i]->_uiPoker->setTexture(pic);//new
//				}
//				_pokerArea->work();
//				_qdButton->setVisible(true);
//				
//			}), NULL));
//			checkQD();
//			checkResult();
//			//unschedule("recv_poker");
//		}
//	}, 0.0f, kRepeatForever, 0.0f, "recv_poker");
//}

//void GameScene::checkQD()
//{
//	_heroArea->setWork4QD(true);
//	schedule([&](float dt) {
//		if (!_canQD)
//		{
//			log("!canQD");
//			_heroArea->setWork4QD(false);
//
//			for (int i = 0; i < 3; i++)
//			{
//				if (_players[i]->_sName == _qdName)
//				{
//					_qdPos = i;
//					break;
//				}
//			}
//
//			auto cd = CountDown::create(10);
//			_players[_qdPos]->_uiAnswerIng->setVisible(true);
//			_players[_qdPos]->_uiTimer->setVisible(true);
//			_players[_qdPos]->_uiTimer->runAction(Sequence::create(cd, NULL));
//
//			unschedule("checkQD");
//		}
//		}, 0.0f, kRepeatForever, 0.0f, "checkQD");
//
//
//	/*this->schedule([&](float dt) {
//		if (!_canQD)
//		{
//			log("!canQD");
//			_qdButton->setVisible(false);
//			for (int i = 0; i < 3; i++)
//			{
//				if (_players[i]->_sName == _qdName)
//				{
//					_qdPos = i;
//					break;
//				}
//			}
//
//			auto cd = CountDown::create(10);
//			_players[_qdPos]->_uiAnswerIng->setVisible(true);
//			_players[_qdPos]->_uiTimer->setVisible(true);
//			_players[_qdPos]->_uiTimer->runAction(Sequence::create(cd, NULL));
//		
//			unschedule("checkQD");
//		}
//	}, 0.0f, kRepeatForever, 0.0f, "checkQD");*/
//}

//void GameScene::checkCalculate(const char* calcStr)
//{
//	//设计(栈)解析calc字符串结果是否为24，先手动挡
//			//std::cout << "len: " << strlen(recvCalc->_sCalculate) << std::endl;
//			//中缀->后缀（括号法），第一步，操作符移动到对应括号后面
//	std::stack<char> c_stack;
//	std::map<int, bool> ibMap;//运算符是否已归位
//
//	char tmp[13] = "";
//	memset(tmp, ')', sizeof(tmp) - 1);
//	log("tmp is ): %s", tmp);
//	strncpy(tmp, calcStr, strlen(calcStr));
//	log("tmp last is ): %s", tmp);
//	for (int i = 0; i <= strlen(calcStr); i++)
//	{
//		ibMap.insert(std::pair<int, bool>(i, false));
//	}
//	for (int i = 0; i <= strlen(calcStr); i++)
//	{
//		if ((tmp[i] == '+' || tmp[i] == '-' || tmp[i] == '*' || tmp[i] == '/') && !ibMap[i])
//		{
//
//			for (int j = i + 1; j < strlen(calcStr); j++)
//			{
//				if (calcStr[j] == '(')
//				{
//					c_stack.push(tmp[j]);
//				}
//				else if (calcStr[j] == ')')
//				{
//					if (c_stack.empty())
//					{
//						char tt = tmp[i];
//						tmp[i] = tmp[j];
//						tmp[j] = tt;
//						ibMap[j] = true;
//						std::cout << "tmp: " << tmp << std::endl;
//						break;
//					}
//					else
//					{
//						c_stack.pop();
//					}
//				}
//				if (j == strlen(calcStr) - 1)
//				{
//					char tt = tmp[i];
//					tmp[i] = tmp[j + 1];
//					tmp[j + 1] = tt;
//					ibMap[j + 1] = true;
//					std::cout << "tmp: " << tmp << std::endl;
//				}
//
//			}
//		}
//	}
//	//std::cout << "tmp: " << tmp << std::endl;
//	//中缀->后缀（括号法），第二步，去括号
//	char tmp2[12];
//	int index = 0;
//	for (char* p = &tmp[0]; *p != '\0'; p++)
//	{
//		if (*p != '(' && *p != ')')
//		{
//			tmp2[index] = *p;
//			index++;
//		}
//	}
//	tmp2[index] = '\0';
//	std::cout << "tmp2: " << tmp2 << std::endl;
//
//	//后缀表达式计算
//	//std::cout << "stack is empty? :" << c_stack.empty() << std::endl;
//	std::stack<int> i_stack;
//	for (int i = 0; i < strlen(tmp2); i++)
//	{
//		if (tmp2[i] == '+' || tmp2[i] == '-' || tmp2[i] == '*' || tmp2[i] == '/')
//		{
//			int op2 = i_stack.top();
//			i_stack.pop();
//			int op1 = i_stack.top();
//			i_stack.pop();
//			int r;
//			switch (tmp2[i])
//			{
//			case '+':
//				r = op1 + op2;
//				break;
//			case '-':
//				r = op1 - op2;
//				break;
//			case '*':
//				r = op1 * op2;
//				break;
//			case '/':
//				r = op1 / op2;
//				break;
//			default:
//				break;
//			}
//			i_stack.push(r);
//		}
//		else
//		{
//			i_stack.push(tmp2[i] - '0');
//		}
//	}
//	//std::cout << "result: " << i_stack.top() << std::endl;
//
//	i_stack.top() == 24 ? _isWin = true : _isWin = false;
//}

//void GameScene::checkResult()
//{
//	this->schedule([&](float dt) {
//		if (_isOver)
//		{
//			log("Game over!");
//			if (_isWin)
//			{
//				log("You win!");
//
//			}
//			else
//			{
//				log("You lose");
//			}
//
//			_downLabel->stopAllActions();
//			_downLabel->setVisible(false);
//			//_ansButton->setVisible(false);
//
//			_rcToggle->setSelectedIndex(0);
//			_rcToggle->setVisible(true);
//			
//			if (!_canQD)//被动模式则清除他人UI
//			{
//				_players[_qdPos]->_uiTimer->stopAllActions();
//				_players[_qdPos]->_uiTimer->setVisible(false);
//				_players[_qdPos]->_uiAnswerIng->setVisible(false);
//			}
//
//			_canQD = true;
//			_isOver = false;
//
//			unschedule("check_result");
//		}
//	}, 0.0f, kRepeatForever, 0.0f, "check_result");
//}