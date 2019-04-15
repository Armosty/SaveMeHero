#include "HeroStats.h"

HeroStats::HeroStats()
{
	armor = 0;
	cure = 1;
	damage = 2;
	defence = 3;
	health = 4;
	dodge = 5;

	strength = 6;
	endurance = 7;
	intelligence = 8;
	dexterity = 9;

	currentHealth = 10;
	maxHealth = 11;
	currentPower = 12;
	maxPower = 13;

	level = 1;
}

int HeroStats::getArmor()
{
	return armor;
}

int HeroStats::getCure()
{
	return cure;
}

int HeroStats::getDamage()
{
	return damage;
}

int HeroStats::getDefence()
{
	return defence;
}

int HeroStats::getHealth()
{
	return health;
}

int HeroStats::getDodge()
{
	return dodge;
}

int HeroStats::getStrength()
{
	return strength;
}

int HeroStats::getEndurance()
{
	return endurance;
}

int HeroStats::getIntelligence()
{
	return intelligence;
}

int HeroStats::getDexterity()
{
	return dexterity;
}

int HeroStats::getCurrentHealth()
{
	return currentHealth;
}

int HeroStats::getMaxHealth()
{
	return maxHealth;
}

int HeroStats::getCurrentPower()
{
	return currentPower;
}

int HeroStats::getMaxPower()
{
	return maxPower;
}

int HeroStats::getLevel()
{
	return level;
}
