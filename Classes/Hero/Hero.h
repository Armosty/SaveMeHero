#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"
#include "../Tools/Map.h"
#include "Dialog.h"
#include <string>
#include <vector>
#include "HeroStats.h"

USING_NS_CC;

class Hero : public Sprite
{
private:

	int heroSpeed;
	HeroStats stats;

	int spriteWidth;
	int spriteHeight;
	int frame;

	bool _isMoving;

	enum class Direction : int
	{
		Bottom = 0,
		Left,
		Top,
		Right
	} direction;

	// animation
	void changeFrame(float);
	void normalizeFrame(float);

	SEL_SCHEDULE changeFrameCallback;
	SEL_SCHEDULE normalizeFrameCallback;

	// activate hero and camera move and returns move time
	float moveTo(const Vec2& start, const Vec2& destination, float delay = 0);

	Dialog *dialog;

	// hero activity
	class Behavior : public Node
	{
		enum class ActionType
		{
			None, Move, Sleep
		};

		struct Replica
		{
			std::string phrase;
			ActionType action;
		};

		Hero *hero;

		std::vector <Replica> replicas;
		Replica *currentReplica;

		bool isActivated;
		bool isActiveStage;

		SEL_SCHEDULE actingCallback;
		void actingFunction(float);

		SEL_SCHEDULE moveCallback;
		void moveFunction(float);

		SEL_SCHEDULE sleepCallback;
		void sleepFunction(float);

	public:

		Behavior(Hero *hero);
		void startActing();
		void endActing();

		void onTouchEnded(Touch*, Event*);

	} *behavior;

	// activate behavior
	void startActing(float);

public:

	Hero();

	// hero and camera move along optimal way
	float move(const Vec2& destination);

	int speed()
	{
		return heroSpeed;
	}

	float width()
	{
		return spriteWidth * getScaleX();
	}

	float height()
	{
		return spriteHeight * getScaleY();
	}

	bool isMoving()
	{
		return _isMoving;
	}

	void showDialog(const std::string& text);
	void hideDialog();
	bool isDialogShowing();
	Size dialogSize();

	SEL_SCHEDULE startActingCallback;
	void disableBehavior();

};

#endif // __HERO_H__