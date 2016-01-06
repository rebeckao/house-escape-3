#include "Tile.h"


Tile::~Tile()
{
}

Tile::Tile() :exits(std::vector<bool>(4, 0)) {
}

Tile::Tile(std::vector<bool> init_exits) {
	exits = init_exits;
}