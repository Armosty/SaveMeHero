#ifndef __ENEMY_PLACER_H__
#define __ENEMY_PLACER_H__

#include <vector>
#include "Map.h"
#include "../Enemies/Enemy.h"

class EnemyPlacer
{
private:

	int cellWidth;
	int cellHeight;
	int width;
	int height;

	struct Cell
	{
		float minEmenyDistance;
		bool walkable;
	};

	std::vector <std::vector <Cell>> map;

public:

	EnemyPlacer(const TileMap* map);
	Vec2 place(const std::vector <Enemy*>& enemies);

};

#endif // __ENEMY_PLACER_H__