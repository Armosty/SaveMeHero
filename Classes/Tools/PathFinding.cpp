#include "PathFinding.h"
#include <algorithm>


PathFinding::PathFinding(const TileMap* map, Hero* hero)
{
	this->hero = hero;
	cellWidth = map->getTileSize().width;
	cellHeight = map->getTileSize().height;
	width = map->getContentSize().width / cellWidth;
	height = map->getContentSize().height / cellHeight;

	this->map = std::vector <std::vector <Cell>> (height, std::vector <Cell>(width));

	auto objectGroups = map->getObjectGroups();
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			this->map[i][j].x = j;
			this->map[i][j].y = i;
			this->map[i][j].walkable = true;
			this->map[i][j].parent = nullptr;
		}
	}
	for (int group = 0; group < objectGroups.size(); group++)
	{
		auto objects = objectGroups.at(group)->getObjects();
		for (auto &object : objects)
		{
			auto &valueMap = object.asValueMap();
			int x = valueMap["x"].asFloat() / cellWidth;
			int y = valueMap["y"].asFloat() / cellHeight;
			int width = (valueMap["width"].asFloat() / cellWidth + 1.0f);
			int height = (valueMap["height"].asFloat() / cellHeight + 1.0f);

			for (int i = y; i < y + height; i++)
			{
				for (int j = x; j < x + width; j++)
				{
					this->map[i][j].walkable = false;
				}
			}
		}
	}
}

bool PathFinding::find(const Vec2& start, const Vec2& target, std::vector <Vec2>& path)
{
	Cell &startTile = map[(int)start.y / cellHeight][(int)start.x / cellWidth];
	Cell &targetTile = map[(int)target.y / cellHeight][(int)target.x / cellWidth];

	if (!map[targetTile.y][targetTile.x].walkable)
	{
		return false;
	}

	std::vector <Cell*> openList;
	std::vector <Cell*> closedList;

	std::vector <Vec2> neighbour{
		{-1, -1}, // corners
		{ 1, -1},
		{-1,  1},
		{ 1,  1},
		{ 0, -1}, // sides
		{-1,  0},
		{ 1,  0},
		{ 0,  1},
	};

	startTile.G = 0.0f;
	openList.push_back(&startTile);

	while (!openList.empty())
	{
		int min = openList.size() - 1;
		for (int i = min - 1; i >= 0; i--)
		{
			if (openList.at(min)->F() > openList.at(i)->F())
			{
				min = i;
			}
		}
		Cell &current = map[openList.at(min)->y][openList.at(min)->x];
		openList.erase(openList.begin() + min);

		if (current == targetTile)
		{
			break;
		}

		closedList.push_back(&current);

		for (int i = 0; i < neighbour.size(); i++)
		{
			int x = current.x + neighbour.at(i).x;
			int y = current.y + neighbour.at(i).y;

			if (x < 0 || x >= width || y < 0 || y >= height)
			{
				continue;
			}

			Cell &next = map[y][x];
			
			if (!map[next.y][next.x].walkable || 
				std::find(closedList.begin(), closedList.end(), &next) != closedList.end())
			{
				continue;
			}
			float newG = current.G + 1.0f;
			if (i <= 3)
			{
				newG += 0.4f;
			}
			if (std::find(openList.begin(), openList.end(), &next) == openList.end())
			{
				next.parent = &current;
				next.H = abs(next.x - targetTile.x) + abs(next.y - targetTile.y);
				next.G = newG;
				openList.push_back(&next);
			}
			else if (newG < next.G)
			{
				next.G = newG;
				next.parent = &current;
			}
		}

	}

	Cell *current = &targetTile;
	while (current->parent)
	{
		// tile center
		path.push_back(Vec2((current->x + 0.5f) * cellWidth, 
							(current->y + 0.5f) * cellHeight));
		current = current->parent;
	}
	if (path.size() > 1)
	{
		path.pop_back();
	}
	std::reverse(path.begin(), path.end());

	return true;
}


