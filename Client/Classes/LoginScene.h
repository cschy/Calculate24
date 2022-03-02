#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "SceneManager.h"
#include "Socket/SocketClient.h"

#include "ui/CocosGUI.h"
using namespace ui;

class LoginScene : public Scene
{
public:
	virtual bool init();
	CREATE_FUNC(LoginScene);
	SceneManager* tsm;

	SocketClient* s_client;
	bool _isAllowLogin;
	bool _isAllowRegister;
	bool _loginSpinLock;
	
	EditBox* _accountBox;
	EditBox* _passwordBox;
	
};



#endif