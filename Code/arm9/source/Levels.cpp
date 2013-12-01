#include "Levels.h"
#include "LevelMainMenu.h"
#include "LevelMainMenuBind.h"
#include "LevelTest.h"
#include "LevelTestBind.h"
#include "WinLevel.h"
#include "WinLevelBind.h"
#include <maxmod9.h>
#include <vector>
#include <stdio.h>

#include "soundbank_bin.h"
#include "soundbank.h"
#include "Tileset.h"
#include "Sprites.h"
#include "Blocks.h"
#include "Entities.h"
#include "MenuImage.h"
#include "Wall.h"
extern Game* gp;
extern BlockFactory bf;

//Level1
void Level1::onLoad(){
	lcdMainOnTop();
	TileSize = 2;
	AmbientLight = 0;
	DFloor = 17;
	SizeX = 512/(TileSize*8);
	SizeY = 256/(TileSize*8);
	
	swiCopy(WallBitmap, bgGetGfxPtr(gp->BG3), WallBitmapLen/2);
	swiCopy(WallPal, BG_PALETTE_SUB, WallPalLen/2);
	
	loadCommon();
	
	initBlocks(LevelTestBitmap, LevelTestBindings, LevelTestBindingsLen);
	
	((Plate*)Grid[11 + 5*SizeX])->linkTrigger((Triggerable*)Grid[4 + 7*SizeX]);
	((Plate*)Grid[12 + 4*SizeX])->linkTrigger((Triggerable*)Grid[13 + 7*SizeX]);
	
	((Door*)Grid[17 + 7*SizeX])->linkToLevel(new LevelWinLevel());
	
	Entity* p = new Player;
	p->onLoad();
	Grid[SpawnY*SizeX + SpawnX]->setEntity(p);
	IPlayer = p;
	
	Skeleton* m = new Skeleton();
	m->onLoad();
	Grid[3*SizeX + 3]->setEntity(m);
	
	drawLevel();

	gp->som.playBGM(6, true);
	gp->som.setBGMVol(128);
}
Level* Level1::copy(){ return new Level1();};
//MainMenu

void LevelMainMenu::onLoad(){
	lcdMainOnBottom();
	TileSize = 2;
	AmbientLight = 250;
	DFloor = 20;
	SizeX = 256/(TileSize*8);
	SizeY = 192/(TileSize*8);
	
	swiCopy(MenuImageBitmap, bgGetGfxPtr(gp->BG3), MenuImageBitmapLen/2);
	swiCopy(MenuImagePal, BG_PALETTE_SUB, MenuImagePalLen/2);
	
	loadCommon();
	
	initBlocks(LevelMainMenuBitmap, LevelMainMenuBindings, LevelMainMenuBindingsLen);
	
	((Door*)Grid[7 + 4*SizeX])->linkToLevel(new Level1);
	((Door*)Grid[8 + 4*SizeX])->linkToLevel(new Level1);
	
	Entity* p = new Player;
	p->onLoad();
	Grid[SpawnY*SizeX + SpawnX]->setEntity(p);
	IPlayer = p;
	
	drawLevel();

	gp->som.playSFX(0, 1024);
	gp->som.playBGM(3, true);
	gp->som.setBGMVol(128);
}
Level* LevelMainMenu::copy(){ return new LevelMainMenu();};
//WinLevel
void LevelWinLevel::onLoad(){
	lcdMainOnBottom();
	TileSize = 2;
	AmbientLight = 250;
	DFloor = 20;
	SizeX = 256/(TileSize*8);
	SizeY = 192/(TileSize*8);
	
	loadCommon();
	
	initBlocks(WinLevelBitmap, WinLevelBindings, WinLevelBindingsLen);
	
	((Door*)Grid[7 + 1*SizeX])->linkToLevel(new LevelMainMenu);
	((Door*)Grid[8 + 1*SizeX])->linkToLevel(new LevelMainMenu);
	
	Entity* p = new Player;
	p->onLoad();
	Grid[SpawnY*SizeX + SpawnX]->setEntity(p);
	IPlayer = p;
	
	drawLevel();

	gp->som.playSFX(0, 1024);
	gp->som.playBGM(0, true);
	gp->som.setBGMVol(128);
}
Level* LevelWinLevel::copy(){ return new LevelWinLevel();};
