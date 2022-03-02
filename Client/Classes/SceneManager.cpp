#include "SceneManager.h"
#include "LoginScene.h"
#include "GameScene.h"


bool SceneManager::init()
{
	if (!Scene::init())
	{
		return false;
	}
	/*
	//账号框 设置输入16个字符
	Size visibleSize = Director::getInstance()->getVisibleSize();
	EditBox* accountBox = EditBox::create(Size(200, 35), Scale9Sprite::create("UI/editbox.png"));
	accountBox->setPosition(Point(visibleSize.width/2, visibleSize.height*0.7));
	accountBox->setPlaceHolder("Name:");//占位字符
	accountBox->setMaxLength(16);
	//editbox1->setFontColor(Color3B::BLACK);//设置输入字体的颜色
	accountBox->setTag(1);
	this->addChild(accountBox);
	//密码框 password
	EditBox* editbox2 = EditBox::create(Size(200, 35), Scale9Sprite::create("UI/editbox.png"));
	editbox2->setPosition(Point(visibleSize.width/2, visibleSize.height*0.5));
	editbox2->setPlaceHolder("Password:");
	editbox2->setPlaceholderFontColor(Color3B::RED);//设置占位字体的颜色
	//editbox2->setFontColor(Color3B::BLACK);
	editbox2->setInputMode(EditBox::InputMode::ANY);//设置输入类型 数字及特殊符号输入
	editbox2->setInputFlag(EditBox::InputFlag::PASSWORD);//设置输入标志 密码输入标志（显示为*）
	editbox2->setMaxLength(16);
	editbox2->setTag(2);
	this->addChild(editbox2);
	
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
			EditBox* box1 = (EditBox *)this->getChildByTag(1);
			EditBox* box2 = (EditBox *)this->getChildByTag(2);
			log("ac: %s\n", box1->getText());
			log("ps: %s\n", box2->getText());
			//初始化DLL
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);

			sockaddr_in sockAddr;
			memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
			sockAddr.sin_family = PF_INET;
			InetPton(AF_INET, TEXT("192.168.1.11"), &sockAddr.sin_addr.s_addr);
			//sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
			sockAddr.sin_port = htons(1234);

			//创建套接字
			SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			//向服务器发起请求
			connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
			//接收服务器传回的数据
			char szBuffer[MAXBYTE] = { 0 };
			//std::string message = std::string(box1->getText()) + "+" + std::string(box2->getText());
			//send(sock, message.c_str(), sizeof(message), NULL);
			char cmd[] = "1";
			send(sock, cmd, sizeof(char)*strlen(cmd), NULL);
			//Sleep(1000 * 1);
			
			send(sock, box1->getText(), sizeof(char)*strlen(box1->getText()), NULL);
			Sleep(1000*1);
			send(sock, box2->getText(), sizeof(char)*strlen(box2->getText()), NULL);
			recv(sock, szBuffer, MAXBYTE, NULL);;
			//输出接收到的数据
			log("Message form server: %s\n", szBuffer);
			//关闭套接字
			closesocket(sock);
			//终止使用 DLL
			//WSACleanup();

			if (strcmp(szBuffer, "welcome") == 0)
			{

			}

			break;
		}
	});

	Button* regisAccount = Button::create("UI/register.png");
	regisAccount->setScale(0.5f);
	regisAccount->setPosition(Vec2(visibleSize.width*0.3, visibleSize.height*0.3));
	this->addChild(regisAccount);

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
			log("ac: %s\n", box1->getText());
			log("ps: %s\n", box2->getText());
			//初始化DLL
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);

			sockaddr_in sockAddr;
			memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
			sockAddr.sin_family = PF_INET;
			InetPton(AF_INET, TEXT("192.168.1.11"), &sockAddr.sin_addr.s_addr);
			//sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
			sockAddr.sin_port = htons(1234);

			//创建套接字
			SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			//向服务器发起请求
			connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
			//接收服务器传回的数据
			char szBuffer[MAXBYTE] = { 0 };
			
			char cmd[] = "0";
			send(sock, cmd, sizeof(char)*strlen(cmd), NULL);
			//Sleep(1000 * 1);
			send(sock, box1->getText(), sizeof(char)*strlen(box1->getText()), NULL);
			Sleep(1000 * 1);
			send(sock, box2->getText(), sizeof(char)*strlen(box2->getText()), NULL);

			recv(sock, szBuffer, MAXBYTE, NULL);
			//输出接收到的数据
			log("Message form server: %s\n", szBuffer);
			//关闭套接字
			closesocket(sock);
			//终止使用 DLL
			//WSACleanup();

			break;
		}
	});
	*/
	Size vSize = Director::getInstance()->getVisibleSize();
	Label *label = Label::createWithTTF("loading...", "fonts/Marker Felt.ttf", 36);
	label->setPosition(Vec2(vSize.width / 2, vSize.height / 2));
	this->addChild(label);
	scheduleOnce([&](float dt) {
		goLoginScene();
	}, 1.0f, "yaeryo");
	return true;
}

void SceneManager::update(float delta)
{
	
}

void SceneManager::goLoginScene()
{
	LoginScene* loginScene = LoginScene::create();
	loginScene->tsm = this;

	Director::getInstance()->replaceScene(loginScene);
}

void SceneManager::goHallScene()
{


}

void SceneManager::goGameScene()
{
	GameScene* gameScene = GameScene::create();
	//gameScene->sock = sock;
	gameScene->tsm = this;
	

	/*Node *socketNode = Node::create();
	gameScene->addChild(socketNode);
	char str[_MAX_STR_LEN_];
	socketNode->schedule([&](float dt) {
		_send(sock, "having acc");
		_recv(sock, str);
		log("having acc: %s", str);
	}, 1.0f, kRepeatForever, 0.0f, "socket");*/
	
	Director::getInstance()->replaceScene(gameScene);
}