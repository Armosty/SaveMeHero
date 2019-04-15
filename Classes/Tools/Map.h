#ifndef __MAP_H__
#define __MAP_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class TileMap : public TMXTiledMap
{

private:

	Vec2 _viewCenter;

public:

	TileMap()
	{
		initWithTMXFile("bigmap.tmx");
		autorelease();
	}

	Vec2& viewCenter()
	{
		return _viewCenter;
	}

};

/*
class TileMap
{
	Vec2 _viewCenter;
	TMXTiledMap *map;

public:

	TileMap(const std::string file)
	{
		map = TMXTiledMap::create(file);
	}

	Vec2& viewCenter()
	{
		return _viewCenter;
	}

	Size getTileSize()
	{
		return map->getTileSize();
	}

	Size getContentSize()
	{
		return map->getContentSize();
	}

	void setPosition(const Vec2& position)
	{
		map->setPosition(position);
	}

	TMXTiledMap* getMap()
	{
		return map;
	}

	void addChild(Node *child, int zOrder)
	{
		map->addChild(child, zOrder);
	}

	void runAction(Action* action)
	{
		map->runAction(action);
	}

	void stopAllActions()
	{
		map->stopAllActions();
	}

};
*/
#endif