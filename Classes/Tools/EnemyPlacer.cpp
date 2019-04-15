#include "EnemyPlacer.h"

EnemyPlacer::EnemyPlacer(const TileMap* map)
{
	cellWidth = map->getTileSize().width;
	cellHeight = map->getTileSize().height;
	width = map->getContentSize().width / cellWidth;
	height = map->getContentSize().height / cellHeight;

	this->map = std::vector <std::vector <Cell>>(height, std::vector <Cell>(width));

	auto objectGroups = map->getObjectGroups();

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			this->map[i][j].minEmenyDistance = Vec2(width, height).distance(Vec2(0, 0));
			this->map[i][j].walkable = true;
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

Vec2 EnemyPlacer::place(const std::vector <Enemy*>& enemies)
{
	float maxDistance = map[0][0].minEmenyDistance;
	if (enemies.size() > 0)
	{
		maxDistance = 0;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				Vec2 tiledEnemyPosition = enemies.back()->getPosition();
				tiledEnemyPosition.x /= cellWidth;
				tiledEnemyPosition.y /= cellHeight;

				map[y][x].minEmenyDistance = 
					std::min(map[y][x].minEmenyDistance, Vec2(x, y).distance(tiledEnemyPosition));
				maxDistance = std::max(maxDistance, map[y][x].minEmenyDistance);
			}
		}
	}

	std::vector <Vec2> possiblePositions;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (map[y][x].walkable &&
				std::abs(map[y][x].minEmenyDistance - maxDistance) < 4)
			{
				float posX = (x + 0.5f) * cellWidth;
				float posY = (y + 0.5f) * cellHeight;
				possiblePositions.push_back(Vec2(posX, posY));
			}
		}
	}

	RandomHelper rnd;

	return possiblePositions[rnd.random_int(size_t(0), possiblePositions.size())];
}
