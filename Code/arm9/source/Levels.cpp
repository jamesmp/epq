#include "Levels.h"
#include "LevelMainMenu.h"
#include "LevelMainMenuBind.h"
#include "LevelTest.h"
#include "LevelTestBind.h"
#include "Level2.h"
#include "Level2Bind.h"
#include "Level3.h"
#include "Level3Bind.h"
#include "Level4.h"
#include "Level4Bind.h"
#include "Level5.h"
#include "Level5Bind.h"
#include "Level6.h"
#include "Level6Bind.h"
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
#include "EndImage.h"
extern Game* gp;
extern BlockFactory bf;

//Level1
void Level1::onLoad(){
	lcdMainOnTop();
	TileSize = 2;
	AmbientLight = 0;
	DFloor = 17;
	ViewDist = 13;
	SizeX = 512/(TileSize*8);
	SizeY = 256/(TileSize*8);
	
	swiCopy(WallBitmap, bgGetGfxPtr(gp->BG3), WallBitmapLen/2);
	swiCopy(WallPal, BG_PALETTE_SUB, WallPalLen/2);
	
	loadCommon();
	
	initBlocks(LevelTestBitmap, LevelTestBindings, LevelTestBindingsLen);
	
	((Plate*)Grid[11 + 5*SizeX])->linkTrigger((Triggerable*)Grid[4 + 7*SizeX]);
	((Plate*)Grid[12 + 4*SizeX])->linkTrigger((Triggerable*)Grid[13 + 7*SizeX]);
	
	((Door*)Grid[17 + 7*SizeX])->linkToLevel(new Level2());
	
	Entity* p = new Player;
	p->onLoad();
	Grid[SpawnY*SizeX + SpawnX]->setEntity(p);
	IPlayer = p;
	
	drawLevel();

	gp->som.playBGM(6, true);
	gp->som.setBGMVol(128);
}
Level* Level1::copy(){ return new Level1();};

//Level2
void Level2::onLoad(){
	lcdMainOnTop();
	TileSize = 2;
	AmbientLight = 0;
	ViewDist = 13;
	DFloor = 17;
	SizeX = 512/(TileSize*8);
	SizeY = 512/(TileSize*8);
	
	loadCommon();
	
	initBlocks(Level2Bitmap, Level2Bindings, Level2BindingsLen);
	
	((Plate*)Grid[11 + 5*SizeX])->linkTrigger((Triggerable*)Grid[16 + 4*SizeX]);
	
	((Door*)Grid[18 + 4*SizeX])->linkToLevel(new Level3());
	
	Entity* p = new Player;
	p->onLoad();
	Grid[SpawnY*SizeX + SpawnX]->setEntity(p);
	IPlayer = p;
	
	drawLevel();
}
Level* Level2::copy(){ return new Level2();};

//Level3
void Level3::onLoad(){
	lcdMainOnTop();
	TileSize = 2;
	AmbientLight = 0;
	ViewDist = 11;
	DFloor = 22;
	SizeX = 512/(TileSize*8);
	SizeY = 512/(TileSize*8);
	
	loadCommon();
	
	initBlocks(Level3Bitmap, Level3Bindings, Level3BindingsLen);
	
	((Plate*)Grid[7 + 2*SizeX])->linkTrigger((Triggerable*)Grid[13 + 3*SizeX]);
	((Plate*)Grid[7 + 3*SizeX])->linkTrigger((Triggerable*)Grid[14 + 3*SizeX]);
	((Plate*)Grid[9 + 2*SizeX])->linkTrigger((Triggerable*)Grid[15 + 3*SizeX]);
	((Plate*)Grid[9 + 3*SizeX])->linkTrigger((Triggerable*)Grid[16 + 3*SizeX]);
	((Plate*)Grid[9 + 4*SizeX])->linkTrigger((Triggerable*)Grid[17 + 3*SizeX]);
	
	((Door*)Grid[1 + 3*SizeX])->linkToLevel(new Level4());
	
	Entity* p = new Player;
	p->onLoad();
	Grid[SpawnY*SizeX + SpawnX]->setEntity(p);
	IPlayer = p;
	
	gp->som.playBGM(2, true);
	drawLevel();
}
Level* Level3::copy(){ return new Level3();};

//Level4
void Level4::onLoad(){
	lcdMainOnTop();
	TileSize = 2;
	AmbientLight = 0;
	ViewDist = 4;
	DFloor = 19;
	SizeX = 512/(TileSize*8);
	SizeY = 512/(TileSize*8);
	
	loadCommon();
	
	initBlocks(Level4Bitmap, Level4Bindings, Level4BindingsLen);
	
	((Door*)Grid[8 + 26*SizeX])->linkToLevel(new Level5());
	
	Entity* p = new Player;
	p->onLoad();
	Grid[SpawnY*SizeX + SpawnX]->setEntity(p);
	IPlayer = p;
	
	drawLevel();
}
Level* Level4::copy(){ return new Level4();};

//Level5
void Level5::onLoad(){
	lcdMainOnTop();
	TileSize = 2;
	AmbientLight = 0;
	ViewDist = 11;
	DFloor = 22;
	SizeX = 512/(TileSize*8);
	SizeY = 512/(TileSize*8);
	
	loadCommon();
	
	initBlocks(Level5Bitmap, Level5Bindings, Level5BindingsLen);
	
	((Plate*)Grid[5 + 8*SizeX])->linkTrigger((Triggerable*)Grid[3 + 7*SizeX]);
	((Plate*)Grid[7 + 8*SizeX])->linkTrigger((Triggerable*)Grid[5 + 5*SizeX]);
	((Plate*)Grid[9 + 8*SizeX])->linkTrigger((Triggerable*)Grid[13 + 5*SizeX]);
	((Plate*)Grid[11 + 8*SizeX])->linkTrigger((Triggerable*)Grid[15 + 7*SizeX]);
	((Plate*)Grid[13 + 8*SizeX])->linkTrigger((Triggerable*)Grid[9 + 10*SizeX]);
	
	((Plate*)Grid[15 + 14*SizeX])->linkTrigger((Triggerable*)Grid[3 + 14*SizeX]);
	
	((Door*)Grid[1 + 14*SizeX])->linkToLevel(new Level6());
	
	Entity* p = new Player;
	p->onLoad();
	Grid[SpawnY*SizeX + SpawnX]->setEntity(p);
	IPlayer = p;
	
	drawLevel();
}
Level* Level5::copy(){ return new Level5();};

//Level6
void Level6::onLoad(){
	lcdMainOnTop();
	TileSize = 2;
	AmbientLight = 0;
	ViewDist = 11;
	DFloor = 22;
	SizeX = 512/(TileSize*8);
	SizeY = 512/(TileSize*8);
	
	loadCommon();
	
	initBlocks(Level6Bitmap, Level6Bindings, Level6BindingsLen);
	
	((Plate*)Grid[9 + 5*SizeX])->linkTrigger((Triggerable*)Grid[8 + 10*SizeX]);
	((Plate*)Grid[9 + 6*SizeX])->linkTrigger((Triggerable*)Grid[8 + 11*SizeX]);
	((Plate*)Grid[11 + 5*SizeX])->linkTrigger((Triggerable*)Grid[8 + 12*SizeX]);
	((Plate*)Grid[13 + 5*SizeX])->linkTrigger((Triggerable*)Grid[8 + 13*SizeX]);
	((Plate*)Grid[13 + 6*SizeX])->linkTrigger((Triggerable*)Grid[8 + 14*SizeX]);
	
	((Plate*)Grid[20 + 18*SizeX])->linkTrigger((Triggerable*)Grid[20 + 17*SizeX]);
	((Plate*)Grid[23 + 16*SizeX])->linkTrigger((Triggerable*)Grid[2 + 4*SizeX]);
	
	((Plate*)Grid[5 + 26*SizeX])->linkTrigger((Triggerable*)Grid[6 + 25*SizeX]);
	((Plate*)Grid[7 + 26*SizeX])->linkTrigger((Triggerable*)Grid[6 + 27*SizeX]);
	((Plate*)Grid[9 + 26*SizeX])->linkTrigger((Triggerable*)Grid[10 + 25*SizeX]);
	((Plate*)Grid[11 + 26*SizeX])->linkTrigger((Triggerable*)Grid[10 + 27*SizeX]);
	((Plate*)Grid[13 + 26*SizeX])->linkTrigger((Triggerable*)Grid[14 + 25*SizeX]);
	((Plate*)Grid[15 + 26*SizeX])->linkTrigger((Triggerable*)Grid[14 + 27*SizeX]);
	
	((Podium*)Grid[23 + 26*SizeX])->linkToLevel(new LevelWinLevel());
	
	
	
	Entity* p = new Player;
	p->onLoad();
	Grid[SpawnY*SizeX + SpawnX]->setEntity(p);
	IPlayer = p;
	
	gp->som.playBGM(4, true);
	drawLevel();
}
Level* Level6::copy(){ return new Level6();};

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
	
	((Door*)Grid[7 + 4*SizeX])->linkToLevel(new Level1());
	((Door*)Grid[8 + 4*SizeX])->linkToLevel(new Level1());
	
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
	
	swiCopy(EndImageBitmap, bgGetGfxPtr(gp->BG3), EndImageBitmapLen/2);
	swiCopy(EndImagePal, BG_PALETTE_SUB, EndImagePalLen/2);
	
	drawLevel();

	gp->som.playSFX(0, 1024);
	gp->som.playBGM(0, true);
	gp->som.setBGMVol(128);
}
Level* LevelWinLevel::copy(){ return new LevelWinLevel();};
