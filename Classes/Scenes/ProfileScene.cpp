#include "ProfileScene.h"

Scene* ProfileScene::createScene()
{
	return ProfileScene::create();
}

bool ProfileScene::init()
{
	Sprite *background = Sprite::create("images/backgroundfull.png");

	Size screenSize = Director::getInstance()->getVisibleSize();
	background->setAnchorPoint(Vec2(0, 0));
	background->setScale(std::min(screenSize.width / background->getContentSize().width,
								  screenSize.height / background->getContentSize().height));
	addChild(background, -1);

	auto profile = TMXTiledMap::create("interface.tmx");
	profile->setAnchorPoint(Vec2(0, 0));
	scaleX = screenSize.width / profile->getContentSize().width;
	scaleY = screenSize.height / profile->getContentSize().height;
	profile->setScaleX(scaleX);
	profile->setScaleY(scaleY);
	addChild(profile);

	auto clickableLayer = profile->getObjectGroup("clickable");
	auto playButton = clickableLayer->getObject("game");

	this->playButton = Rect(playButton["x"].asFloat() * profile->getScaleX(), playButton["y"].asFloat() * profile->getScaleY(), 
							playButton["width"].asFloat(), playButton["height"].asFloat());

	auto touch = EventListenerTouchOneByOne::create();

	touch->onTouchBegan = [](Touch*, Event*) {
		return true;
	};

	touch->onTouchEnded = CC_CALLBACK_2(ProfileScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch, this);

	return true;
}

void ProfileScene::onTouchEnded(Touch* touch, Event* event)
{
	Vec2 touchCoords = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation());

	if (playButton.containsPoint(touchCoords))
	{
		Director::getInstance()->popScene();
	}
}

void ProfileScene::setStats(HeroStats *stats)
{
	auto profile = TMXTiledMap::create("interface.tmx");
	auto statGroup = profile->getObjectGroup("icons");

	static int offStatSize = 35;
	createStatText(statGroup, "armor", offStatSize, stats->getArmor());
	createStatText(statGroup, "cure", offStatSize, stats->getCure());
	createStatText(statGroup, "dmg", offStatSize, stats->getDamage());
	createStatText(statGroup, "def", offStatSize, stats->getDefence());
	createStatText(statGroup, "health", offStatSize, stats->getHealth());
	createStatText(statGroup, "dodge", offStatSize, stats->getDodge());

	static int mainStatSize = 40;
	createStatText(statGroup, "str", mainStatSize, stats->getStrength());
	createStatText(statGroup, "end", mainStatSize, stats->getEndurance());
	createStatText(statGroup, "int", mainStatSize, stats->getIntelligence());
	createStatText(statGroup, "dex", mainStatSize, stats->getDexterity());

	static int resourcesSize = 100;
	createStatText(statGroup, "hp", resourcesSize, stats->getCurrentHealth(), stats->getMaxHealth());
	createStatText(statGroup, "pwr", resourcesSize, stats->getCurrentPower(), stats->getMaxPower());

	static int levelSize = 40;
	createStatText(statGroup, "lvl", levelSize, stats->getLevel());
	createStatText(statGroup, "lvlto", levelSize, stats->getLevel() + 1);
}

void ProfileScene::createStatText(const TMXObjectGroup *group, const std::string& objectName, int fontSize, int statValue, int secondStatValue)
{
	auto object = group->getObject(objectName);
	Vec2 coords((object["x"].asFloat() + object["width"].asFloat() / 2) * scaleX, 
				(object["y"].asFloat() + object["height"].asFloat()) * scaleY);

	std::string text = std::to_string(statValue);

	if (secondStatValue != -1)
	{
		text += "/" + std::to_string(secondStatValue);
	}

	auto layer = Label::createWithTTF(text, "himalaya.ttf", fontSize);
	layer->setAnchorPoint(Vec2(0.5f, 1.0f));
	layer->setPosition(coords);
	
	addChild(layer);
}
