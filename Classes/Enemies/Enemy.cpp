#include "Enemy.h"

Enemy::Enemy(std::string file)
{
	initWithFile(file);
	autorelease();
	Size originalSize(193, 248);
	setScale(originalSize.width / getContentSize().width,
		originalSize.height / getContentSize().height);
	setAnchorPoint(Vec2(0.5f, 0.5f));

	spriteWidth = getContentSize().width / 6;
	spriteHeight = getContentSize().height / 4;
	setTextureRect(Rect(spriteWidth, spriteHeight, spriteWidth, spriteHeight));
}

void Enemy::setHero(Hero* hero)
{
	this->hero = hero;
}

float Enemy::width()
{
	return spriteWidth * getScaleX();
}

float Enemy::height()
{
	return spriteHeight * getScaleY();
}
