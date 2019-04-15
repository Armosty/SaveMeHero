#ifndef __PATH_FINDING_H__
#define __PATH_FINDING_H__

#include "cocos2d.h"
#include "../Hero/Hero.h"
#include "Map.h"
#include <vector>

USING_NS_CC;

class PathFinding
{

private:

	Hero *hero;

	int cellWidth;
	int cellHeight;
	int width;
	int height;

	struct Cell
	{
		int x;
		int y;

		int H;
		float G;
		inline float F() 
		{
			return G + H;
		}
		bool walkable;
		Cell* parent;

		inline bool operator == (const Cell& other)
		{
			return x == other.x && y == other.y;
		}

	};

	std::vector <std::vector <Cell>> map;

public:
	
	PathFinding(const TileMap* map, Hero* hero);
	// A* algorithm, path stored in path variable, return false if target not walkable 
	bool find(const Vec2& start, const Vec2& target, std::vector <Vec2>& path);

};

#endif // __PATH_FINDING_H__
