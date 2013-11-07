#include "Blocks.h"

BlockFactory bf;

Wall::Wall(){
	HasEntity = false;
	Solid = Opaque = true;
}

Floor::Floor(){
	HasEntity = false;
	Solid = Opaque = false;
}

Wall BlockFactory::makeWall(u16 _ti){
	Wall r;
	r.setTileIndex(_ti);
	return r;
}
Floor BlockFactory::makeFloor(u16 _ti){
	Floor r;
	r.setTileIndex(_ti);
	return r;
}