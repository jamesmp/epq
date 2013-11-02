#include "ClassDeclarations.h"
#include <maxmod9.h>
#include <vector>
#include <stdio.h>

#include "soundbank_bin.h"
#include "soundbank.h"
#include "Tileset.h"
extern Game* gp;

//Sprite Methods
void Sprite::setSpriteEntry(SpriteEntry* _sp){
	ISpriteEntry = _sp;
}
SpriteEntry* Sprite::getSpriteEntry(){
	return ISpriteEntry;
}
//Entity Methods
Entity::Entity(){
	SpriteChanged = true;
}
void Entity::onLoad(){
	ISprite = Sprite();
	SpriteEntry* _sp = gp->lvl->getOamEntry();
	ISprite.setSpriteEntry(_sp);
}
bool Entity::useOn(Item* ip, Entity* ep){
	return true;
}
bool Entity::tick(){
	//calculations and such
	if (SpriteChanged){
		u16 vx = gp->lvl->ViewX;
		u16 vy = gp->lvl->ViewY;
		u8 ts = gp->lvl->TileSize * 8;
		
		ISprite.ISpriteEntry->x = GridX - vx + REG_BG0HOFS - ts;
		ISprite.ISpriteEntry->y = GridY - vy + REG_BG0VOFS - ts;
		gp->lvl->OamDirty = true;
	}
	return true;
}
//Block Methods
Block::Block(){
	HasEntity = false;
}
u16 Block::getTileIndex(){
	return TileIndex;
}

void Block::onLoad(){
	
};
bool Block::tick(){
	if (HasEntity){
		IEntity->tick();
	}
	return true;
}
bool Block::useOn(Item*, Entity*){return true;};
//Level Methods
void Level::onLoad(){
	
	TileSize = 2;
	SizeX = 512/(TileSize*8);
	SizeY = 256/(TileSize*8);
	ViewX = 0;
	ViewY = 0;
	OamIndex = 0;
	AnimDirty = true;
	OamDirty = true;
	
	swiCopy(TilesetTiles, BG_TILE_RAM(0), TilesetTilesLen/2);
	swiCopy(TilesetPal, BG_PALETTE, TilesetPalLen/4);
	REG_BG0HOFS = (vu16)(TileSize*8);
	REG_BG0VOFS = (vu16)(TileSize*8);
	
	oamInit(&oamMain, SpriteMapModeMain, false);
	oamInit(&oamSub, SpriteMapModeSub, false);
	
	
	for (u8 i=0; i<128; i++){
		OamEntryMain[i].blendMode = OBJMODE_NORMAL;
		OamEntryMain[i].colorMode = OBJCOLOR_256;
		OamEntryMain[i].priority = OBJPRIORITY_0;
		OamEntryMain[i].hFlip = false;
		OamEntryMain[i].vFlip = false;
		OamEntryMain[i].isRotateScale = false;
		OamEntryMain[i].isSizeDouble = false;
		OamEntryMain[i].shape = OBJSHAPE_SQUARE;
		OamEntryMain[i].size = OBJSIZE_16;
		OamEntryMain[i].x = 0;
		OamEntryMain[i].y = 0;
	}
	
	for (int i=0; i<SizeX*SizeY; i++){ 
		Block b;
		b.TileIndex = (u16)i%32;
		b.onLoad();
		Grid.push_back(b);
	};
	drawLevel();
	oamEnable(&oamMain);
	oamEnable(&oamSub);
	gp->som.playBGM(3, true);
}
void Level::drawLevel(){
	
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
	
	AnimDirty = false;
}

void Level::onUnload(){
	oamDisable(&oamMain);
	oamDisable(&oamSub);
}
bool Level::tick(){
	if (((vu16)REG_BG0HOFS!=(u16)(TileSize*8)) && (REG_BG0HOFS%(TileSize*8)==0)){
		iprintf("%i\n", REG_BG0HOFS);
		ViewX += (REG_BG0HOFS/(TileSize*8)-1);
		REG_BG0HOFS = (vu16)(TileSize*8);
		AnimDirty = true;
	}
	if ((REG_BG0VOFS!=(u16)(TileSize*8) && REG_BG0VOFS%(TileSize*8)==0)){
		ViewY += (REG_BG0VOFS/(TileSize*8)-1);
		REG_BG0VOFS = (vu16)(TileSize*8);
		AnimDirty = true;
	}
	for (std::vector<Block>::iterator it = Grid.begin(); it != Grid.end(); ++it){
		it->tick();
	}
	if (AnimDirty){ drawLevel();};
	if (OamDirty){
		swiCopy(OamEntryMain, &oamMain, (128 * sizeof(SpriteEntry)) / 2);
		swiCopy(OamEntrySub, &oamSub, (128 * sizeof(SpriteEntry)) / 2);
		OamDirty = false;
	}
	return true;
};

SpriteEntry* Level::getOamEntry(){
	if (OamIndex<128){
		return (SpriteEntry*)&OamEntryMain[OamIndex++];
	}
	else {return (SpriteEntry*)&OamEntryMain[127];};
}
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
	iprintf("setup\n");
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