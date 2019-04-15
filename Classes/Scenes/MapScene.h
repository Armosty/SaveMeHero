
#ifndef __MAP_SCENE_H__
#define __MAP_SCENE_H__

#include "cocos2d.h"
#include "../Hero/Hero.h"
#include "../Tools/Map.h"
#include "../Enemies/Enemy.h"
#include <vector>

USING_NS_CC;

class MapScene : public Scene
{

private:

	bool heroWalkLocked;
	
	Vec2 screenCenter;
	TileMap *map;
	Hero *hero;
	
	void enableHeroMove(float);

	int maxEmeniesCount;
	std::vector <Enemy*> enemies;
	Enemy* battleTarget;

	SEL_SCHEDULE startBattleCallback;
	void startBattle(float);

public:
	static Scene* createScene();

	virtual bool init();
	virtual bool onTouchBegan(Touch*, Event*);
	virtual void onTouchEnded(Touch*, Event*);

	void placeEnemies();

	// implement the "static create()" method manually
	CREATE_FUNC(MapScene);
};

#endif // __MAP_SCENE_H__