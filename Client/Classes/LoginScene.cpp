#include "LoginScene.h"

#include "Socket/SocketClient.h"
#include "./Data/TransData.h"


#include <iostream>
#include <string>

//#include "extensions\cocos-ext.h"
//USING_NS_CC_EXT;
//using namespace network;


bool LoginScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	_loginSpinLock = true;
	_isAllowLogin = false;
	_isAllowRegister = false;
	s_client = SocketClient::getInstance();
	TransData::getInstance()->_loginScene = this;


	Size visibleSize = Director::getInstance()->getVisibleSize();
	log("width: %f", visibleSize.width);
	//�˺ſ� ��������8���ַ�
	_accountBox = EditBox::create(Size(200, 35), Scale9Sprite::create("UI/editbox.png"));
	_accountBox->setPosition(Point(visibleSize.width / 2, visibleSize.height*0.7));
	_accountBox->setPlaceHolder("Name:");//ռλ�ַ�
	_accountBox->setMaxLength(8);
	//editbox1->setFontColor(Color3B::BLACK);//���������������ɫ
	_accountBox->setTag(1);
	this->addChild(_accountBox);
	//����� password
	_passwordBox = EditBox::create(Size(200, 35), Scale9Sprite::create("UI/editbox.png"));
	_passwordBox->setPosition(Point(visibleSize.width / 2, visibleSize.height*0.5));
	_passwordBox->setPlaceHolder("Tips: Don't need password");
	_passwordBox->setPlaceholderFontColor(Color3B::RED);//����ռλ�������ɫ
	//editbox2->setFontColor(Color3B::BLACK);
	//_passwordBox->setInputMode(EditBox::InputMode::ANY);//������������ ���ּ������������
	//_passwordBox->setInputFlag(EditBox::InputFlag::PASSWORD);//���������־ ���������־����ʾΪ*��
	_passwordBox->setMaxLength(50);
	_passwordBox->setTag(2);
	this->addChild(_passwordBox);

	Button* enterGame = Button::create("UI/enter_normal.png", "UI/enter_select.png");
	enterGame->setPosition(Vec2(visibleSize.width*0.7, visibleSize.height*0.3));
	this->addChild(enterGame);

	enterGame->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
		case Widget::TouchEventType::MOVED:
		case Widget::TouchEventType::CANCELED:
			break;
		case Widget::TouchEventType::ENDED:
			//EditBox* box1 = (EditBox *)this->getChildByTag(1);
			//EditBox* box2 = (EditBox *)this->getChildByTag(2);
			log("Account: %s", _accountBox->getText());
			log("Password: %s", _passwordBox->getText());

			if (std::string(_accountBox->getText()).empty())
			{
				_passwordBox->setText("Name cannot be empty, Don't need password");
			}
			else
			{
				TransData::getInstance()->_userName = _accountBox->getText();
				log("cpy name: %s", TransData::getInstance()->_userName.c_str());
				//��֤�û���¼
				s_client->initNetwork();
				log("Net is ok!");


				/*TransData::getInstance()->_userInfo->usrName = box1->getText();
				TransData::getInstance()->_userInfo->passWord = box2->getText();*/
				TransData::getInstance()->sendData(DataType_C::LOGIN);
				while (_loginSpinLock);
				_loginSpinLock = true;
				if (!_isAllowLogin)
				{
					s_client->closeConnectPublic();
					if (TransData::getInstance()->_disAllowLoginType == DisAllowLoginType::SAME_NAME)
					{
						_passwordBox->setText("Already has the same name!");
					}
					
				}
				//while (!_isAllowLogin);
				else
				{
					log("Login is ok!");
					tsm->goGameScene();
				}
			}
			break;
		}
	});

	Button* regisAccount = Button::create("UI/register.png");
	regisAccount->setScale(0.5f);
	regisAccount->setPosition(Vec2(visibleSize.width*0.3, visibleSize.height*0.3));
	this->addChild(regisAccount);
	regisAccount->setVisible(false);
	regisAccount->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
		case Widget::TouchEventType::MOVED:
		case Widget::TouchEventType::CANCELED:
			break;
		case Widget::TouchEventType::ENDED:
			EditBox* box1 = (EditBox *)this->getChildByTag(1);
			EditBox* box2 = (EditBox *)this->getChildByTag(2);
			log("register ac: %s\n", box1->getText());
			log("register ps: %s\n", box2->getText());

			s_client->initNetwork();
			/*TransData::getInstance()->_userInfo->usrName = box1->getText();
			TransData::getInstance()->_userInfo->passWord = box2->getText();*/
			TransData::getInstance()->sendData(DataType_C::REGISTER);

			//TransData::getInstance()->_loginScene = this;
			while (!_isAllowRegister);
			log("Register is ok!");
			break;
		}
	});

	//scheduleUpdate();
	return true;
}