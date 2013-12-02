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
//Podium
Podium::Podium(){
	HasEntity = false;
	Solid = Opaque = true;
	LinkedLevel = false;
}

void Podium::linkToLevel(Level* _lp){
	lp = _lp;
	LinkedLevel = true;
}

void Podium::useOn(Item* _ip, Entity* _ep){
	if (gp->lvl->isPlayer(_ep)){
		if (LinkedLevel){
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
	Locked = false;
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
			iprintf("Trigger");
			gp->lvl->AnimDirty = true;
		}
	}
}
//Latch
Latch::Latch(){
	HasEntity = false;
	Solid = Opaque = true;
	Triggered= false;
}
void Latch::trigger(bool _s){
	if (!Triggered){
		TileIndex-=1;
		Solid = Opaque = false;
		gp->lvl->AnimDirty = true;
		Triggered = true;
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
			gp->som.playSFX(4, 1024);
		}
	}
	else{
		if (LastState){
			tp->trigger(false);
			LastState = false;
			gp->som.playSFX(4, 1024);
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

Plate* BlockFactory::makePlate(u16 _ti){
	Plate* r = new Plate();
	r->setTileIndex(_ti);
	return r;
}

Latch* BlockFactory::makeLatch(u16 _ti){
	Latch* r = new Latch();
	r->setTileIndex(_ti);
	return r;
}

Podium* BlockFactory::makePodium(u16 _ti, Level* _l){
	Podium* r = new Podium();
	r->setTileIndex(_ti);
	r->linkToLevel(_l);
	return r;
}