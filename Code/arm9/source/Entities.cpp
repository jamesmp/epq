#include <stdio.h>
#include <stdlib.h>
#include "Entities.h"
extern Game* gp;

bool Player::tick(){
	if (!Moving){
		if((keysHeld() & KEY_LEFT) && GridX>0){
			Block* b = gp->lvl->getBlock(GridX-1, GridY);
			if (!b->Solid && !(b->IEntity->Solid && b->HasEntity)){
				Moving = true;
				translate(-1, 0);
				aX = gp->lvl->TileSize*8 - 1;
				IMoveDir = left;
				IFaceDir = left;
				gp->lvl->AnimDirty = true;
			}
		}
		else if((keysHeld() & KEY_RIGHT) && GridX<gp->lvl->SizeX - 1){
			Block* b = gp->lvl->getBlock(GridX+1, GridY);
			if (!b->Solid && !(b->IEntity->Solid && b->HasEntity)){
				Moving = true;
				translate(1, 0);
				aX = -gp->lvl->TileSize*8 + 1;
				IMoveDir = right;
				IFaceDir = right;
				gp->lvl->AnimDirty = true;
			}
		}
		else if((keysHeld() & KEY_UP) && GridY>0){
			Block* b = gp->lvl->getBlock(GridX, GridY-1);
			if (!b->Solid && !(b->IEntity->Solid && b->HasEntity)){
				Moving = true;
				translate(0, -1);
				aY = gp->lvl->TileSize*8 - 1;
				IMoveDir = up;
				IFaceDir = up;
				gp->lvl->AnimDirty = true;
			};
		}
		else if((keysHeld() & KEY_DOWN) && GridY<gp->lvl->SizeY - 1){
			Block* b = gp->lvl->getBlock(GridX, GridY+1);
			if (!b->Solid && !(b->IEntity->Solid && b->HasEntity)){
				Moving = true;
				translate(0, 1);
				aY = -gp->lvl->TileSize*8 + 1;
				IMoveDir = down;
				IFaceDir = down;
				gp->lvl->AnimDirty = true;
			}
		}
		else if (keysDown() & KEY_A){
			for (int i=0; i<4; i++){
				s8 GX = (i-2)*(i%2);
				s8 GY = (i-1)*((i+1)%2);
				if ((GridY+GY)<gp->lvl->SizeY && (GridX+GX)<gp->lvl->SizeX){
					Block* b = gp->lvl->getBlock(GridX+GX, GridY+GY);
					b->useOn((Item*)&MainHand, (Entity*)this);
					if (b->HasEntity){
						gp->som.playSFX(5, 1024);
					}
				}
			}
		}
	}
	else{
		switch (IMoveDir){
			case left:
				aX-=1;
				ISprite.HFlip = true;
				break;
			case right:
				aX+=1;
				ISprite.HFlip = false;
				break;
			case up:
				aY-=1;
				ISprite.HFlip = false;
				break;
			case down:
				aY+=1;
				ISprite.HFlip = false;
				break;
		}
	}
	
	int frame = (aX)/8 + (aY)/8;
	if (frame<0){frame -= 2*frame;};
	if (IMoveDir!=down){
		frame+=2;
		if(IMoveDir==up){
			frame+=2;
		}
	}
	ISprite.AnimFrame = frame;
	
	calcGrid();
	calcSprite();
	if (HitPoints==0){iprintf("Dead");};
	return true;
}
void Player::onLoad(){
	loadCommon();
	Solid = true;
	IFaceDir = down;
	MainHand = Item(25, 1);
	HitPoints = 100;
}

bool Mob::tick(){
	cooldown>0 ? cooldown-- : cooldown=0;
	if (checkPlayer() && gp->lvl->IPlayer->HitPoints>0){
		if (cooldown==0){
			iprintf("Attacking!");
			gp->som.playSFX(1, 1024);
			int att = Attack - ((Player*)gp->lvl->IPlayer)->MainHand.getDef();
			att < 0 ? att=0 : att=att;
			gp->lvl->IPlayer->HitPoints -= att;
			cooldown = Cooltime;
		}
	}
	else{
		moveIdle();
	}
	if(Moving){
		switch (IMoveDir){
			case left:
				aX-=1;
				ISprite.HFlip = true;
				break;
			case right:
				aX+=1;
				ISprite.HFlip = false;
				break;
			case up:
				aY-=1;
				ISprite.HFlip = false;
				break;
			case down:
				aY+=1;
				ISprite.HFlip = false;
				break;
		}
	}
	int frame = (aX)/8 + (aY)/8;
	if (frame<0){frame -= 2*frame;};
	if (IMoveDir!=down && Multidir){
		frame+=2;
		if(IMoveDir==up){
			frame+=2;
		}
	}
	ISprite.AnimFrame = frame;
	calcGrid();
	calcSprite();
	return true;
}

void Mob::onLoad(){
	loadCommon();
	Solid = true;
	ISprite.setGfxBase(16);
	IFaceDir = down;
	HitPoints = 100;
	movecool = 0;
	Attack = 7;
	Cooltime = 25;
	Multidir = false;
}
bool Mob::move(int _mv){
	bool moved = false;
	Block* b;
	switch(_mv){
		case 0:
			b = gp->lvl->getBlock(GridX-1, GridY);
			if (!b->Solid && !(b->IEntity->Solid && b->HasEntity)){
				Moving = true;
				translate(-1, 0);
				aX = gp->lvl->TileSize*8 - 1;
				IMoveDir = left;
				moved = true;
			}
			break;
		case 1:
			b = gp->lvl->getBlock(GridX+1, GridY);
			if (!b->Solid && !(b->IEntity->Solid && b->HasEntity)){
				Moving = true;
				translate(1, 0);
				aX = -gp->lvl->TileSize*8 + 1;
				IMoveDir = right;
				moved = true;
			}
			break;
		case 2:
			b = gp->lvl->getBlock(GridX, GridY-1);
			if (!b->Solid && !(b->IEntity->Solid && b->HasEntity)){
				Moving = true;
				translate(0, -1);
				aY = gp->lvl->TileSize*8 - 1;
				IMoveDir = up;
				moved = true;
			};
			break;
		case 3:
			b = gp->lvl->getBlock(GridX, GridY+1);
			if (!b->Solid && !(b->IEntity->Solid && b->HasEntity)){
				Moving = true;
				translate(0, 1);
				aY = -gp->lvl->TileSize*8 + 1;
				IMoveDir = down;
				moved = true;
			}
			break;
	}
	return moved;
}

void Mob::moveIdle(){
	movecool++;
	if (movecool>(rand()%25) && !Moving){
		movecool = 0;
		int d = rand()%4;
		move(d);
	}
}

bool Mob::checkPlayer(){
	for (int i=0; i<4; i++){
		s8 GX = (i-2)*(i%2);
		s8 GY = (i-1)*((i+1)%2);
		if ((GridY+GY)<gp->lvl->SizeY && (GridX+GX)<gp->lvl->SizeX){
			Block* b = gp->lvl->getBlock(GridX+GX, GridY+GY);
			if (b->HasEntity){
				if (gp->lvl->isPlayer(b->IEntity)){
					return true;
				}
			}
		}
	}
	return false;
}

bool Mob::useOn(Item* _ip, Entity* _ep){
	int oldHP = HitPoints;
	if (gp->lvl->isPlayer(_ep)){
		_ip->getAtt() > HitPoints ? HitPoints = 0 : HitPoints-=_ip->getAtt();
		iprintf("Hit A Foe! %i-%i=%i\n", oldHP, oldHP-HitPoints, HitPoints);
		gp->som.playSFX(5, 1024);
		if (HitPoints<=0){
			onUnload();
			gp->lvl->getBlock(GridX, GridY)->HasEntity = false;
			gp->som.playSFX(0, 1024);
			delete this;
		}
		return true;
	}
	return false;
}

void Skeleton::onLoad(){
	loadCommon();
	Solid = true;
	ISprite.setGfxBase(18);
	IFaceDir = down;
	HitPoints = 200;
	movecool = 0;
	Attack = 11;
	Cooltime = 25;
	Multidir = true;
}

bool Rock::useOn(Item* _ip, Entity* _ep){
	if (gp->lvl->isPlayer(_ep)){
		bool Moved = true;
		Block* b;
		if (_ep->GridX<GridX){
			b = gp->lvl->getBlock(GridX+1, GridY);
			if (!b->Solid && !(b->IEntity->Solid && b->HasEntity)){
				Moving = true;
				translate(1, 0);
				aX = -gp->lvl->TileSize*8 + 1;
				IMoveDir = right;
			}
		}
		else if(_ep->GridX>GridX){
			b = gp->lvl->getBlock(GridX-1, GridY);
			if (!b->Solid && !(b->IEntity->Solid && b->HasEntity)){
				Moving = true;
				translate(-1, 0);
				aX = gp->lvl->TileSize*8 - 1;
				IMoveDir = left;
			}
		}
		else if(_ep->GridY<GridY){
			b = gp->lvl->getBlock(GridX, GridY+1);
			if (!b->Solid && !(b->IEntity->Solid && b->HasEntity)){
				Moving = true;
				translate(0, 1);
				aY = -gp->lvl->TileSize*8 + 1;
				IMoveDir = down;
			}
		}
		else if(_ep->GridY>GridY){
			b = gp->lvl->getBlock(GridX, GridY-1);if (!b->Solid && !(b->IEntity->Solid && b->HasEntity)){
				Moving = true;
				translate(0, -1);
				aY = gp->lvl->TileSize*8 - 1;
				IMoveDir = up;
			}
		}
		else { Moved = false;};
		if (Moved){
			gp->som.playSFX(6, 1024);
		}
	}
	
}

void Rock::onLoad(){
	loadCommon();
	Solid = true;
	ISprite.setGfxBase(32);
}

bool Rock::tick(){
	
	if(Moving){
		switch (IMoveDir){
			case left:
				aX-=1;
				break;
			case right:
				aX+=1;
				break;
			case up:
				aY-=1;
				break;
			case down:
				aY+=1;
				break;
		}
	}
	int frame = (aX)/8 + (aY)/8;
	if (frame<0){frame -= 2*frame;};
	ISprite.AnimFrame = frame;
	calcGrid();
	calcSprite();
	return true;
}