#ifndef __HERO_STATS_H__
#define __HERO_STATS_H__

class HeroStats
{
private:

	int armor;
	int cure;
	int damage;
	int defence;
	int health;
	int dodge;

	int strength;
	int endurance;
	int intelligence;
	int dexterity;

	int currentHealth;
	int maxHealth;
	int currentPower;
	int maxPower;

	int level;

public:

	HeroStats();

	int getArmor();
	int getCure();
	int getDamage();
	int getDefence();
	int getHealth();
	int getDodge();

	int getStrength();
	int setStrength(int strength);
	int getEndurance();
	int setEndurance(int endurance);
	int getIntelligence();
	int setIntelligence(int intelligence);
	int getDexterity();
	int setDexterity(int dexterity);

	int getCurrentHealth();
	int getMaxHealth();
	int getCurrentPower();
	int getMaxPower();

	int getLevel();
};

#endif // __HERO_STATS_H__
