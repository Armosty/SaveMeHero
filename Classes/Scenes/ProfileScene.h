#ifndef __PROFILE_SCENE_H__
#define __PROFILE_SCENE_H__

#include "cocos2d.h"
#include "../Hero/HeroStats.h"

USING_NS_CC;

class ProfileScene : public Scene
{

private:

	Rect playButton;

	float scaleX;
	float scaleY;

	void createStatText(const TMXObjectGroup *group, const std::string& objectName, int fontSize, int startValue, int secondStatValue = -1);

public:

	static Scene* createScene();

	virtual bool init();

	void onTouchEnded(Touch*, Event*);
	void setStats(HeroStats *stats);

	CREATE_FUNC(ProfileScene);

};

#endif // __PROFILE_SCENE_H__