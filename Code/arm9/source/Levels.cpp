#include "Levels.h"
#include <maxmod9.h>
#include <vector>
#include <stdio.h>

#include "soundbank_bin.h"
#include "soundbank.h"
#include "Tileset.h"
#include "Sprites.h"
#include "Blocks.h"
#include "LevelTest.h"
extern Game* gp;
extern BlockFactory bf;

void Level1::initBlocks(){
	for (int i=0; i<SizeX*SizeY; i++){
		int id = (LevelTestBitmap[i/2] & (15 << (i%2)*4)) >> (i%2)*4;
		Block b;
		switch (id){
			default:
				b = bf.makeFloor((u16)32);
				break;
			case 1:
				b = bf.makeWall((u16)6);
				break;
			case 0:
				b = bf.makeFloor((u16)21);
				break;
		}
		b.onLoad(i%SizeX, i/SizeX);
		Grid.push_back(b);
	}
	Block b = bf.makeWall((u16)4);
	b.onLoad(3, 3);
	Grid[3 + 3 * SizeX] = b;
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

	gp->som.playSFX(0, 1024);
}

bool Player::tick(){
	if (!Moving){
		if((keysHeld() & KEY_LEFT) && GridX>0){
			if (!gp->lvl->getBlock(GridX-1, GridY)->Solid){
				Moving = true;
				IMoveDir = left;
				gp->som.playSFX(2, 1024);
			}
		}
		else if((keysHeld() & KEY_RIGHT) && GridX<gp->lvl->SizeX - 1){
			if (!gp->lvl->getBlock(GridX+1, GridY)->Solid){
				Moving = true;
				IMoveDir = right;
				gp->som.playSFX(3, 1024);
			}
		}
		else if((keysHeld() & KEY_UP) && GridY>0){
			if (!gp->lvl->getBlock(GridX, GridY-1)->Solid){
				Moving = true;
				IMoveDir = up;
				gp->som.playSFX(4, 1024);
			};
		}
		else if((keysHeld() & KEY_DOWN) && GridY<gp->lvl->SizeY - 1){
			if (!gp->lvl->getBlock(GridX, GridY+1)->Solid){
				Moving = true;
				IMoveDir = down;
				gp->som.playSFX(5, 1024);
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