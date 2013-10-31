#include "ClassDeclarations.h"
#include <maxmod9.h>
#include <vector>
#include <stdio.h>

#include "soundbank_bin.h"
#include "soundbank.h"
#include "Tileset.h"


//Sprite Methods

//Entity Methods

//Block Methods
u16 Block::getTileIndex(){
	return TileIndex;
}

void Block::onLoad(){};
bool Block::tick(){return true;};
bool Block::useOn(Item*, Entity*){return true;};
//Level Methods
void Level::onLoad(){
	
	TileSize = 2;
	SizeX = 512/(TileSize*8);
	SizeY = 256/(TileSize*8);
	ViewX = 0;
	ViewY = 0;
	AnimDirty = false;
	
	swiCopy(TilesetTiles, BG_TILE_RAM(0), TilesetTilesLen/2);
	swiCopy(TilesetPal, BG_PALETTE, TilesetPalLen/4);
	REG_BG0HOFS = (TileSize*8);
	REG_BG0VOFS = (TileSize*8);
	
	oamInit(&oamMain, SpriteMapModeMain, false);
	oamInit(&oamSub, SpriteMapModeSub, false);
	
	for (int i=0; i<SizeX*SizeY; i++){ 
		Block b;
		b.TileIndex = i%32;
		Grid.push_back(b);
	};
	drawLevel();
}
void Level::drawLevel(){
	if (REG_BG0HOFS!=(TileSize*8) && REG_BG0HOFS%(TileSize*8)==0){
		ViewX += (REG_BG0HOFS/(TileSize*8)-1);
		REG_BG0HOFS = (TileSize*8);
	}
	if (REG_BG0VOFS!=(TileSize*8) && REG_BG0VOFS%(TileSize*8)==0){
		ViewY += (REG_BG0VOFS/(TileSize*8)-1);
		REG_BG0VOFS = (TileSize*8);
	}
	
	for (int i=0; i<(192/(TileSize*8)+2); i++){
		for (int j=0; j<(256/(TileSize*8)+2); j++){
			int tX = j + ViewX-1;
			int tY = i + ViewY-1;
			u16 tI = Grid[tX+tY*SizeX].getTileIndex();
			
			
			for (int x=0; x<TileSize; x++){
				for (int y=0; y<TileSize; y++){
					if (j*TileSize<32){
						BG_MAP_RAM(8)[(j*TileSize)+x + (i*TileSize+y)*32] = (u16)(tI*TileSize*TileSize+x+(TileSize*y));
					}
					else{
						BG_MAP_RAM(8)[(j*TileSize)+x + (i*TileSize+y)*32+992] = (u16)(tI*TileSize*TileSize+x+(TileSize*y));
					}
				}
			}
			
			
		}
	}
	iprintf("draw\n");
	AnimDirty = false;
}

void Level::onUnload(){};
bool Level::tick(){
		if (REG_BG0HOFS!=(TileSize*8) && REG_BG0HOFS%(TileSize*8)==0){
		ViewX += (REG_BG0HOFS/(TileSize*8)-1);
		REG_BG0HOFS = (TileSize*8);
		AnimDirty = true;
	}
	if (REG_BG0VOFS!=(TileSize*8) && REG_BG0VOFS%(TileSize*8)==0){
		ViewY += (REG_BG0VOFS/(TileSize*8)-1);
		REG_BG0VOFS = (TileSize*8);
		AnimDirty = true;
	}
	for (std::vector<Block>::iterator it = Grid.begin(); it != Grid.end(); ++it){
		it->tick();
	}
	if (AnimDirty){ drawLevel();};
	return true;
};
//ScoreManager Methods

//SoundManager Methods
SoundManager::SoundManager(){
	mmInitDefaultMem((mm_addr)soundbank_bin);
	BGM[0] = MOD_KEYG_SUBTONAL;
	BGM[1] = MOD_PIGLUMP;
	BGM[2] = MOD_PURPLE_MOTION_INSPIRATION;
	BGM[3] = MOD_REZ_MONDAY;
	BGM[4] = MOD_SO_CLOSE_;
	BGM[5] = MOD_THE_NIMBYS;
	BGM[6] = MOD_ULTRASYD_DUNGEON;
	BGM[7] = MOD_ULTRASYDXCRZN;
	BGM[8] = MOD_ULTRASYDXSELVMORD;
	BGM[9] = MOD_RNDD;
	BGM[10] = MOD_NECROMANCERS_CASTLE;
	for (int i=0; i<sizeof(BGM)/sizeof(mm_word); i++){
		mmLoad(BGM[i]);
	}
	for (int i=0; i<sizeof(SFX)/sizeof(mm_word); i++){
		//mmLoadEffect(SFX[i]);
	}
	playingBGM = playingSFX = false;
}

void SoundManager::playBGM(u8 ind, bool loop){
	int elements = (u8)sizeof(BGM)/sizeof(mm_word);
	ind > elements-1 ? ind = elements-1 : ind = ind;
	if (playingBGM){ mmStop(); };
	if (loop){
		mmStart(BGM[ind], MM_PLAY_LOOP);
	}
	else{
		mmStart(BGM[ind], MM_PLAY_ONCE);
	}
};

void SoundManager::stopBGM(){
	if (playingBGM){ mmStop(); };
	playingBGM = false;
}

void SoundManager::setBGMVol(mm_word vol){
	vol > 1024 ? vol = 1024 : vol = vol;
	mmSetModuleVolume(vol);
}

void SoundManager::setBGMTempo(mm_word temp){
	temp > 2048 ? temp = 2048 : temp = temp;
	temp < 512 ? temp = 512: temp = temp;
	mmSetModuleTempo(temp);
}

void SoundManager::playSFX(u8 ind, mm_word rate){
	int elements = (u8)sizeof(SFX)/sizeof(mm_word);
	ind > elements-1 ? ind = elements-1 : ind = ind;
	mm_sfxhand effect = mmEffect(SFX[ind]);
	mmEffectRate(effect, rate);
}


//Game Methods
Game::Game(){
	som = SoundManager();
	scm = ScoreManager();
	LevelLoaded = false;
	LevelPaused = true;
	
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	vramSetBankA(VRAM_A_MAIN_BG);
	REG_BG0CNT = BG_64x32 | BG_COLOR_256 | BG_MAP_BASE(8) | BG_TILE_BASE(0);
}

void Game::loadLevel(Level* nLvl){
	if (LevelLoaded){
		lvl->onUnload();
	}
	lvl = nLvl;
	lvl->onLoad();
	LevelLoaded = true;	
	LevelPaused = false;
}

void Game::mainLoop(){
	if (LevelLoaded && !LevelPaused){
		lvl->tick();
	}
}