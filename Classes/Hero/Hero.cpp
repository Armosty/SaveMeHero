#include "Hero.h"
#include "../Tools/PathFinding.h"
#include "../Scenes/ProfileScene.h"

Hero::Hero() : heroSpeed(400)
{
	initWithFile("images/hero.png");
	autorelease();
	Size originalSize(193, 248);
	setScale(originalSize.width / getContentSize().width, 
			 originalSize.height / getContentSize().height);
	setAnchorPoint(Vec2(0.5f, 0.5f));

	_isMoving = false;

	frame = 0;
	direction = Direction::Bottom;
	spriteWidth = getContentSize().width / 6;
	spriteHeight = getContentSize().height / 4;
	setTextureRect(Rect(frame * spriteWidth, (int)direction * spriteHeight, spriteWidth, spriteHeight));

	changeFrameCallback = schedule_selector(Hero::changeFrame);
	normalizeFrameCallback = schedule_selector(Hero::normalizeFrame);

	dialog = new Dialog();
	dialog->setPosition(Vec2(width() + 10.0f, height() + 10.0f));
	dialog->setAnchorPoint(Vec2(0, 0));

	float dialogWidth = width() * 10;
	float dialogHeight = height() * 3;

	dialog->setScale(dialogWidth / dialog->getContentSize().width,
					 dialogHeight / dialog->getContentSize().height);
	hideDialog();
	
	addChild(dialog);

	behavior = new Behavior(this);
	behavior->setPosition(dialog->getPosition());
	addChild(behavior);
	startActingCallback = schedule_selector(Hero::startActing);

	scheduleOnce(startActingCallback, 5.0f);

	auto touch = EventListenerTouchOneByOne::create();

	touch->onTouchBegan = [](Touch*, Event*) {
		return true;
	};

	touch->onTouchEnded = [&](Touch* touch, Event* event) {

		Vec2 touchCoords = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation());

		if (touchCoords.x < 0 || touchCoords.x >= getContentSize().width ||
			touchCoords.y < 0 || touchCoords.y >= getContentSize().height)
		{
			return;
		}

		event->stopPropagation();
		auto profile = ProfileScene::createScene();
		((ProfileScene*)profile)->setStats(&stats);
		Director::getInstance()->pushScene(profile);
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch, this);
}

float Hero::move(const Vec2& destination)
{

	std::vector <Vec2> path;

	TileMap *map = (TileMap*)getParent();
	PathFinding pf(map, this);

	if (!pf.find(getPosition(), destination, path) || path.size() < 1)
	{
		return 0;
	}
	_isMoving = true;

	stopAllActions();
	unschedule(changeFrameCallback);
	unschedule(normalizeFrameCallback);

	map->stopAllActions();

	float totalTime = 0;
	Vec2 prevPoint = getPosition();

	for (Vec2& point : path)
	{
		point.x = std::max(point.x, width() / 2);
		point.x = std::min(point.x,
			map->getContentSize().width - width() / 2);
		point.y = std::max(point.y, height() / 2);
		point.y = std::min(point.y,
			map->getContentSize().height - height() / 2);

		totalTime = moveTo(prevPoint, point, totalTime);
		prevPoint = point;
	}
	static float frameChangeTime = 0.075f;
	schedule(changeFrameCallback, frameChangeTime, (totalTime / frameChangeTime) - 1, 0);
	scheduleOnce(normalizeFrameCallback, totalTime);
	scheduleOnce(startActingCallback, totalTime + 5.0f);

	return totalTime;
}

float Hero::moveTo(const Vec2& start, const Vec2& destination, float delay)
{   
	float time = destination.distance(start) / heroSpeed;

	if (time == 0.0)
	{
		return time;
	}

	auto heroMoveTo = MoveTo::create(time, destination);
	auto heroMoveSequence = Sequence::createWithTwoActions(DelayTime::create(delay), heroMoveTo);
	runAction(heroMoveSequence);

	Size screenSize = Director::getInstance()->getVisibleSize();
	Vec2 screenCenter = Vec2(screenSize.width, screenSize.height) / 2;

	TileMap *map = (TileMap*)getParent();

	map->viewCenter() = destination;
	map->viewCenter().x = std::max(map->viewCenter().x, screenCenter.x);
	map->viewCenter().x = std::min(map->viewCenter().x, map->getContentSize().width - screenCenter.x);
	map->viewCenter().y = std::max(map->viewCenter().y, screenCenter.y);
	map->viewCenter().y = std::min(map->viewCenter().y, map->getContentSize().height - screenCenter.y);

	// TODO: replace with Camera class
	auto mapMoveTo = MoveTo::create(time, -(map->viewCenter() - screenCenter));
	auto mapMoveSequence = Sequence::createWithTwoActions(DelayTime::create(delay), mapMoveTo);
	map->runAction(mapMoveSequence);

	// vector along x axis
	Vec2 xVector(1, 0);
	Vec2 destinationVector = destination - start;

	int angle = Vec2::angle(xVector, destinationVector) * 180 / M_PI;
	if (destinationVector.y < 0)
	{
		angle = 360 - angle;
	}

	CallFunc *rotateAction;
	if (angle < 45 || angle > 315)
	{
		rotateAction = CallFunc::create([&]() 
		{
			direction = Direction::Right;
		});
	}
	else if (angle < 135)
	{
		rotateAction = CallFunc::create([&]()
		{
			direction = Direction::Top;
		});
	}
	else if (angle < 225)
	{
		rotateAction = CallFunc::create([&]()
		{
			direction = Direction::Left;
		});
	}
	else
	{
		rotateAction = CallFunc::create([&]()
		{
			direction = Direction::Bottom;
		});
	}
	runAction(Sequence::createWithTwoActions(DelayTime::create(delay), rotateAction));

	return delay + time;
}

void Hero::changeFrame(float)
{
	frame = (frame + 1) % 6;
	setTextureRect(Rect(frame * spriteWidth, (int)direction * spriteHeight, spriteWidth, spriteHeight));
}

void Hero::normalizeFrame(float)
{
	frame = 0;
	setTextureRect(Rect(frame * spriteWidth, (int)direction * spriteHeight, spriteWidth, spriteHeight));

	_isMoving = false;
}

void Hero::showDialog(const std::string& text)
{
	dialog->setText(text);
	dialog->setVisible(true);
}

void Hero::hideDialog()
{
	dialog->setVisible(false);
}

bool Hero::isDialogShowing()
{
	return dialog->isVisible();
}

Size Hero::dialogSize()
{
	return dialog->getContentSize();
}

void Hero::startActing(float)
{
	if (!_isMoving)
	{
		behavior->startActing();
	}
}

void Hero::disableBehavior()
{
	behavior->endActing();
}

