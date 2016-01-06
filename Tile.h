#ifndef TILE_H
#define TILE_H
#include <vector>
class Tile {
public:
	Tile();
	~Tile();
	Tile(std::vector<bool> init_exits);
	std::vector<bool> exits;
	bool explored = false;


};

#endif