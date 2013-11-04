#include "Levels.h"
#include <maxmod9.h>
#include <vector>
#include <stdio.h>

#include "soundbank_bin.h"
#include "soundbank.h"
#include "Tileset.h"
#include "Sprites.h"
extern Game* gp;

void Level1::initBlocks(){
	for (int i=0; i<SizeX*SizeY; i++){ 
		Block b;
		b.TileIndex = (u16)i%32;
		b.onLoad(i%SizeX, i/SizeX);
		Grid.push_back(b);
	}
}

void Level1::onLoad(){
	TileSize = 2;
	SizeX = 512/(TileSize*8);
	SizeY = 256/(TileSize*8);
	
	loadCommon();
	
	swiCopy(TilesetTiles, BG_TILE_RAM(0), TilesetTilesLen/2);
	swiCopy(TilesetPal, BG_PALETTE, TilesetPalLen/4);

	swiCopy(SpritesTiles, SpriteBase, SpritesTilesLen/2);
	swiCopy(SpritesPal, SPRITE_PALETTE, SpritesPalLen/4);
	
	
	initBlocks();
	
	Entity* p = new Player;
	p->onLoad();
	Grid[SizeX + 1].setEntity(p);
	IPlayer = p;
	
	drawLevel();

	gp->som.playSFX(5, 1024);
}

bool Player::tick(){
	if (!Moving){
		if((keysDown() & KEY_LEFT) && GridX>0){
			Moving = true;
			IMoveDir = move_left;
		}
		if((keysDown() & KEY_RIGHT) && GridX<gp->lvl->SizeX - 1){
			Moving = true;
			IMoveDir = move_right;
		}
		if((keysDown() & KEY_UP) && GridY>0){
			Moving = true;
			IMoveDir = move_up;
		}
		if((keysDown() & KEY_DOWN) && GridY<gp->lvl->SizeY - 1){
			Moving = true;
			IMoveDir = move_down;
		}
	}
	else{
		switch (IMoveDir){
			case move_left:
				aX-=1;
				break;
			case move_right:
				aX+=1;
				break;
			case move_up:
				aY-=1;
				break;
			case move_down:
				aY+=1;
				break;
		}
	}
	if (abs(aX) >= gp->lvl->TileSize*8){
		GridX += aX/(gp->lvl->TileSize*8);
		aX = aX%(gp->lvl->TileSize*8);
		Moving = false;
	}
	if (abs(aY) >= gp->lvl->TileSize*8){
		GridY += aY/(gp->lvl->TileSize*8);
		aY = aY%(gp->lvl->TileSize*8);
		Moving = false;
	}
	calcSprite();
	return true;
}