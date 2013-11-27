#include "Blocks.h"
#include "Entities.h"
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
//Door
void Door::linkToLevel(Level* _lp){
	lp = _lp;
	LinkedLevel = true;
	LinkedBlock = false;
}

void Door::useOn(Item* _ip, Entity* _ep){
	if (gp->lvl->isPlayer(_ep)){
		if (LinkedBlock){
			Entity* p = gp->lvl->IPlayer;
			p->translate(bx-p->GridX, by-p->GridY);
		} else if (LinkedLevel){
			gp->setLevel(lp);
		} else{
			iprintf("Unlinked Door Accessed");
		}
	}
}
//Triggerable
Triggerable::Triggerable(){
	HasEntity = false;
};

void Triggerable::trigger(bool _state){
	iprintf("I be triggered");
}

Actuator::Actuator(){
	HasEntity = false;
	Up = true;
	Solid = Opaque = true;
}

void Actuator::trigger(bool _s){
	if (!Locked){
		if (Up==_s){
			if (_s){
				Up = false;
				TileIndex-=1;
				Solid = Opaque = false;
			}
			else{
				Up = true;
				TileIndex+=1;
				Solid = Opaque = true;
			}
			gp->lvl->AnimDirty = true;
		}
	}
}
//Plate
Plate::Plate(){
	HasEntity = false;
	Solid = Opaque = false;
	LastState = false;
}

bool Plate::tick(){
	if (HasEntity){
		IEntity->tick();
		if (!LastState){
			tp->trigger(true);
			LastState = true;
		}
	}
	else{
		if (LastState){
			tp->trigger(false);
			LastState = false;
		}
	}
}

void Plate::linkTrigger(Triggerable* _tp){
	tp = _tp;
}
//Factory
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

Actuator* BlockFactory::makeActuator(u16 _ti){
	Actuator* r = new Actuator();
	r->setTileIndex(_ti);
	return r;
}

Plate* BlockFactory::makePlate(u16 _ti, Triggerable* t){
	Plate* r = new Plate();
	r->setTileIndex(_ti);
	r->linkTrigger(t);
	return r;
}