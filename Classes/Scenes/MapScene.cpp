#include "MapScene.h"
#include "../Tools/EnemyPlacer.h"
#include "../Enemies/Swordsman.h"

Scene* MapScene::createScene()
{
	return MapScene::create();
}

bool MapScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size size = Director::getInstance()->getVisibleSize();
	screenCenter = Vec2(size.width, size.height) / 2;


	map = new TileMap();
	// hero start position
	map->viewCenter() = Vec2(67 * map->getTileSize().width, 53 * map->getTileSize().height);

	map->setPosition(-(map->viewCenter() - screenCenter));
	this->addChild(map, 0, 99);

	hero = new Hero();
	hero->setPosition(map->viewCenter());
	map->addChild(hero, 2);

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(MapScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(MapScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, map);

	maxEmeniesCount = 6;
	//placeEnemies();

	startBattleCallback = schedule_selector(MapScene::startBattle);

	return true;
}

bool MapScene::onTouchBegan(Touch *touch, Event *event)
{
	return true;
}

void MapScene::onTouchEnded(Touch *touch, Event *event)
{
	if (!heroWalkLocked)
	{
		Vec2 touchCoord = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation());

		hero->disableBehavior();

		float time = hero->move(touchCoord);

		heroWalkLocked = true;

		static float walkPauseTime = 0.2f;
		scheduleOnce(schedule_selector(MapScene::enableHeroMove), std::min(time, walkPauseTime));
		/*
		int i = 0;
		for (; i < enemies.size(); i++)
		{
			if (enemies[i]->getBoundingBox().containsPoint(touchCoord))
			{
				break;
			}
		}
		if (i == enemies.size())
		{
			battleTarget = nullptr;
			unschedule(startBattleCallback);
		}
		else
		{
			battleTarget = enemies[i];
			scheduleOnce(startBattleCallback, time);
		}
		*/
	}
	hero->scheduleOnce(hero->startActingCallback, 5.0f);
}

void MapScene::enableHeroMove(float)
{
	heroWalkLocked = false;
}

void MapScene::placeEnemies()
{
	while (enemies.size() < maxEmeniesCount)
	{
		static EnemyPlacer placer(map);
		Vec2 position = placer.place(enemies);

		Enemy *enemy = new Swordsman();

		position.x = std::min(position.x, map->getContentSize().width - enemy->width() / 2);
		position.x = std::max(position.x, enemy->width() / 2);
		position.y = std::min(position.y, map->getContentSize().height - enemy->height() / 2);
		position.y = std::max(position.y, enemy->height() / 2);

		enemy->setPosition(position);
		map->addChild(enemy);
		enemies.push_back(enemy);
	}
}

void MapScene::startBattle(float)
{
	map->setScale(map->getScale() * 2);
	map->viewCenter() = hero->getPosition();
	map->setPosition(-(map->viewCenter() - screenCenter / 2));
}
