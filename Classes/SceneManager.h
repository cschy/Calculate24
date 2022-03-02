#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;



class SceneManager : public Scene
{
public:
	CREATE_FUNC(SceneManager);
	virtual bool init();
	void update(float delta);

	void goLoginScene();
	//void goLoadScene();
	void goHallScene();
	void goGameScene();

};



#endif
