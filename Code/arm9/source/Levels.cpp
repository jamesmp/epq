#include "Levels.h"
#include "LevelMainMenu.h"
#include "LevelTest.h"
#include <maxmod9.h>
#include <vector>
#include <stdio.h>

#include "soundbank_bin.h"
#include "soundbank.h"
#include "Tileset.h"
#include "Sprites.h"
#include "Blocks.h"
#include "Entities.h"
extern Game* gp;
extern BlockFactory bf;

void Level1::initBlocks(){
	for (int i=0; i<SizeX*SizeY; i++){
		int id = (LevelTestBitmap[i/2] & (15 << (i%2)*4)) >> (i%2)*4;
		Block* b;
		Entity* e;
		switch (id){
			default:
				b = bf.makeFloor((u16)32);
				e = 0;
				break;
			case 0:
				b = bf.makeWall((u16)1);
				e = 0;
				break;
			case 6:
				b = bf.makeFloor((u16)21);
				e = 0;
				break;
			case 3:
				b = bf.makePlate((u16) 18);
				e = 0;
				break;
			case 4:
				b = bf.makeActuator((u16)33);
				e = 0;
				break;
			case 2:
				b = bf.makeDoor((u16)33);
				e = 0;
				break;
			case 1:
				b = bf.makeFloor((u16)21);
				e = new Rock();
				break;
			case 5:
				SpawnX = i%SizeX;
				SpawnY = i/SizeX;
				iprintf("%i %i", SpawnX, SpawnY);
				b = bf.makeFloor((u16)21);
				break;
		}
		b->onLoad(i%SizeX, i/SizeX);
		Grid.push_back(b);
		if (e!=0){
			e->onLoad();
			b->setEntity(e);
		}
	}
	((Plate*)Grid[11 + 5*SizeX])->linkTrigger((Triggerable*)Grid[4 + 7*SizeX]);
	((Plate*)Grid[12 + 4*SizeX])->linkTrigger((Triggerable*)Grid[13 + 7*SizeX]);

}

void Level1::onLoad(){
	lcdMainOnTop();
	TileSize = 2;
	AmbientLight = 0;
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
	Grid[SpawnY*SizeX + SpawnX]->setEntity(p);
	IPlayer = p;
	
	drawLevel();

	gp->som.playBGM(6, true);
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
	AmbientLight = 250;
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
	gp->som.playBGM(10, true);
	gp->som.setBGMVol(128);
}
