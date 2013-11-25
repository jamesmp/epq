#include <stdio.h>
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
			}
		}
		else if (keysDown() & KEY_A){
			for (int i=0; i<4; i++){
				s8 GX = (i-2)*(i%2);
				s8 GY = (i-1)*((i+1)%2);
				if ((GridY+GY)<gp->lvl->SizeY && (GridX+GX)<gp->lvl->SizeX){
					Block* b = gp->lvl->getBlock(GridX+GX, GridY+GY);
					b->useOn((Item*)&MainHand, (Entity*)this);
					gp->som.playSFX(5, 1024);
					
				}
			}
		}
	}
	else{
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
	calcGrid();
	calcSprite();
	return true;
}
void Player::onLoad(){
	loadCommon();
	Solid = true;
	IFaceDir = down;
	MainHand = Item(4, 1);
}

bool Mob::tick(){
	moveIdle();
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

void Mob::onLoad(){
	loadCommon();
	Solid = true;
	ISprite.setGfxBase(16);
	IFaceDir = down;
	HitPoints = 100;
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
	if(!Moving){
		int i = ((GridX * GridY) + (gp->lvl->IPlayer->GridX))%5;
		move(i);
	}
}

bool Mob::useOn(Item* _ip, Entity* _ep){
	int oldHP = HitPoints;
	if (gp->lvl->isPlayer(_ep)){
		_ip->getAtt() > HitPoints ? HitPoints = 0 : HitPoints-=_ip->getAtt();
		iprintf("Hit A Foe! %i-%i=%i\n", oldHP, oldHP-HitPoints, HitPoints);
		return true;
	}
	return false;
}