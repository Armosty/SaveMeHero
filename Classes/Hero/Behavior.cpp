#include "Hero.h"
#include "../Tools/PathFinding.h"
#include "cocos-ext.h"
#include <algorithm>

using namespace rapidjson;

Hero::Behavior::Behavior(Hero *hero)
{
	init();
	autorelease();
	this->hero = hero;

	std::string filename = FileUtils::getInstance()->getStringFromFile("text/replicas.json");
	std::string locale = "en";

	Document document;
	document.Parse <0>(filename.c_str());

	if (document.HasMember("replicas"))
	{
		for (int i = 0; i < document["replicas"].Size(); i++)
		{
			std::string phrase = document["replicas"][i][locale.c_str()].GetString();
			std::string action = document["replicas"][i]["action"].GetString();

			Replica replica;
			replica.phrase = phrase;

			if (action == "move")
			{
				replica.action = ActionType::Move;
			}
			else if (action == "sleep")
			{
				replica.action = ActionType::Sleep;
			}
			else
			{
				replica.action = ActionType::None;
			}

			replicas.push_back(replica);
		}
	}

	isActivated = false;
	isActiveStage = false;
	currentReplica = nullptr;
	actingCallback = schedule_selector(Hero::Behavior::actingFunction);
	moveCallback = schedule_selector(Hero::Behavior::moveFunction);
	sleepCallback = schedule_selector(Hero::Behavior::sleepFunction);

	auto touch = EventListenerTouchOneByOne::create();

	touch->onTouchBegan = [](Touch*, Event*) {
		return true;
	};

	touch->onTouchEnded = CC_CALLBACK_2(Hero::Behavior::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch, this);
}

void Hero::Behavior::actingFunction(float)
{
	static int currentRandomReplica = 0;

	if (isActiveStage)
	{
		if (currentRandomReplica == replicas.size())
		{
			std::string lastPhrase = replicas.back().phrase;

			do
			{
				std::random_shuffle(replicas.begin(), replicas.end());
			} while (replicas.front().phrase == lastPhrase);

			currentRandomReplica = 0;
		}

		currentReplica = &replicas[currentRandomReplica++];
		hero->showDialog(currentReplica->phrase);
	}
	else
	{
		hero->hideDialog();

		if (!isActivated)
		{
			unschedule(actingCallback);
		}
	}

	isActiveStage = !isActiveStage;
}

void Hero::Behavior::startActing()
{
	if (!isActivated)
	{
		isActivated = true;
		isActiveStage = true;
		schedule(actingCallback, 3.0f);

		std::random_shuffle(replicas.begin(), replicas.end());
	}
}

void Hero::Behavior::endActing()
{
	isActivated = false;
	hero->scheduleOnce(hero->startActingCallback, 5.0f);
}

void Hero::Behavior::onTouchEnded(Touch* touch, Event* event)
{
	if (!hero->isDialogShowing())
	{
		return;
	}

	Vec2 touchCoords = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation());

	if (touchCoords.x < 0 || touchCoords.x >= hero->dialogSize().width ||
		touchCoords.y < 0 || touchCoords.y >= hero->dialogSize().height)
	{
		if (isScheduled(sleepCallback))
		{
			hero->hideDialog();
		}
		return;
	}

	event->stopPropagation();
	hero->hideDialog();

	if (!currentReplica)
	{
		endActing();
		return;
	}

	switch (currentReplica->action)
	{
	case ActionType::None:
		return;
	case ActionType::Move:
		schedule(moveCallback, 2.0f);
		break;
	case ActionType::Sleep:
		schedule(sleepCallback, 0.5f);
		break;
	}
	currentReplica = nullptr;
	unschedule(actingCallback);
}

void Hero::Behavior::moveFunction(float)
{
	if (!isActivated)
	{
		unschedule(moveCallback);
		endActing();
		return;
	}
	if (!hero->isMoving())
	{
		static int walkRange = 800;

		TileMap *map = (TileMap*)hero->getParent();
		Vec2 position = hero->getPosition();

		int minX = std::max(0.0f, position.x - walkRange);
		int maxX = std::min(map->getContentSize().width - hero->width() / 2, position.x + walkRange);
		int minY = std::max(0.0f, position.y - walkRange);
		int maxY = std::min(map->getContentSize().height - hero->height() / 2, position.y + walkRange);

		RandomHelper rnd;
		Vec2 walkPosition;

		PathFinding pf(map, hero);

		std::vector <Vec2> path;

		do
		{
			int walkX = rnd.random_int(minX, maxX);
			int walkY = rnd.random_int(minY, maxY);

			walkPosition = Vec2(walkX, walkY);

		} while (!pf.find(hero->getPosition(), walkPosition, path) || path.size() < 1);
		hero->move(walkPosition);
	}
}

void Hero::Behavior::sleepFunction(float)
{
	static char sleepText[][4]{
		"",
		"z",
		"zz",
		"zzZ"
	};
	static int currentSleepText = 0;
	static const int sleepTextSize = sizeof(sleepText) / sizeof(sleepText[0]);

	if (!isActivated)
	{
		unschedule(sleepCallback);
		currentSleepText = 0;
		endActing();
		return;
	}

	hero->showDialog(sleepText[currentSleepText]);
	currentSleepText = (currentSleepText + 1) % sleepTextSize;
}
