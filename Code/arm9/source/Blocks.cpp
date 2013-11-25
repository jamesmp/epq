#include "Blocks.h"
#include <stdio.h>
extern Game* gp;
BlockFactory bf;

Wall::Wall(){
	HasEntity = false;
	Solid = Opaque = true;
}

Floor::Floor(){
	HasEntity = false;
	Solid = Opaque = false;
}

Door::Door(){
	HasEntity = false;
	Solid = Opaque = true;
	LinkedBlock = LinkedLevel = false;
}

void Door::linkToBlock(int _bx, int _by){
	if (_bx < gp->lvl->SizeX && _by < gp->lvl->SizeY){
		bx = _bx;
		by = _by;
		LinkedBlock = true;
		LinkedLevel = false;
	}
}

void Door::linkToLevel(Level* _lp){
	lp = _lp;
	LinkedLevel = true;
	LinkedBlock = false;
}

void Door::useOn(Item* _ip, Entity* _ep){
	if (gp->lvl->isPlayer(_ep)){
		if (LinkedBlock){
			gp->lvl->IPlayer->GridX = bx;
			gp->lvl->IPlayer->GridY = by;
		} else if (LinkedLevel){
			gp->setLevel(lp);
		} else{
			iprintf("Unlinked Door Accessed");
		}
	}
}

Wall* BlockFactory::makeWall(u16 _ti){
	Wall* r = new Wall();
	r->setTileIndex(_ti);
	return r;
}
Floor* BlockFactory::makeFloor(u16 _ti){
	Floor* r = new Floor();
	r->setTileIndex(_ti);
	return r;
}
Door* BlockFactory::makeDoor(u16 _ti){
	Door* r = new Door();
	r->setTileIndex(_ti);
	return r;
}