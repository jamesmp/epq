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
#include "LevelMainMenu.h"
#include "Entities.h"
extern Game* gp;
extern BlockFactory bf;

void Level1::initBlocks(){
	for (int i=0; i<SizeX*SizeY; i++){
		int id = (LevelTestBitmap[i/2] & (15 << (i%2)*4)) >> (i%2)*4;
		Block* b;
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
		b->onLoad(i%SizeX, i/SizeX);
		Grid.push_back(b);
	}
	//Block b = bf.makeWall((u16)4);
	//b.onLoad(3, 3);
	//Grid[3 + 3 * SizeX] = b;
}

void Level1::onLoad(){
	lcdMainOnTop();
	TileSize = 2;
	SizeX = 512/(TileSize*8);
	SizeY = 256/(TileSize*8);
	
	loadCommon();
	
	swiCopy(TilesetTiles, BG_TILE_RAM(0), TilesetTilesLen/2);
	swiCopy(TilesetPal, BG_PALETTE, TilesetPalLen/2);

	swiCopy(SpritesTiles, SpriteBase, SpritesTilesLen/2);
	swiCopy(SpritesPal, SPRITE_PALETTE, SpritesPalLen/2);
	
	
	initBlocks();
	
	Entity* p = new Player;
	p->onLoad();
	Grid[SizeX + 1]->setEntity(p);
	IPlayer = p;
	
	Entity *m = new Mob;
	m->onLoad();
	Grid[SizeX+ 2]->setEntity(m);
	
	drawLevel();

	gp->som.playSFX(0, 1024);
	gp->som.playBGM(3, true);
	gp->som.setBGMVol(128);
}
//MainMenu
void LevelMainMenu::initBlocks(){
	for (int i=0; i<SizeX*SizeY; i++){
		int id = (LevelMainMenuBitmap[i/2] & (15 << (i%2)*4)) >> (i%2)*4;
		Block* b;
		switch (id){
			default:
				b = bf.makeFloor((u16)32);
				break;
			case 1:
				b = bf.makeWall((u16)20);
				break;
			case 0:
				b = bf.makeFloor((u16)26);
				break;
		}
		b->onLoad(i%SizeX, i/SizeX);
		Grid.push_back(b);
	}
	Door* b = bf.makeDoor((u16)33);
	b->onLoad(5, 4);
	b->linkToLevel(new Level1);
	Grid[5 + 4 * SizeX] = b;
}

void LevelMainMenu::onLoad(){
	lcdMainOnBottom();
	TileSize = 2;
	SizeX = 256/(TileSize*8);
	SizeY = 192/(TileSize*8);
	
	loadCommon();
	
	swiCopy(TilesetTiles, BG_TILE_RAM(0), TilesetTilesLen/2);
	swiCopy(TilesetPal, BG_PALETTE, TilesetPalLen/2);

	swiCopy(SpritesTiles, SpriteBase, SpritesTilesLen/2);
	swiCopy(SpritesPal, SPRITE_PALETTE, SpritesPalLen/2);
	
	
	initBlocks();
	
	Entity* p = new Player;
	p->onLoad();
	Grid[4*SizeX + 3]->setEntity(p);
	IPlayer = p;
	
	drawLevel();

	gp->som.playSFX(0, 1024);
	gp->som.playBGM(3, true);
	gp->som.setBGMVol(128);
}
