#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include <string>
#include "../Hero/Hero.h"

USING_NS_CC;

class Enemy : public Sprite
{
private:

	Hero* hero;

	int spriteWidth;
	int spriteHeight;

public:

	Enemy(std::string file);
	void setHero(Hero* hero);
	float width();
	float height();

};

#endif // __ENEMY_H__