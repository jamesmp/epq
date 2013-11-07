#include "ClassDeclarations.h"
#include <maxmod9.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "soundbank_bin.h"
#include "soundbank.h"
#include "Tileset.h"
#include "Sprites.h"
extern Game* gp;

//Sprite Methods
Sprite::Sprite(){
	OamID = 127;
	DrawX = DrawY = 0;
	GfxBase = gp->lvl->SpriteBase;
	AnimFrame = 0;
	VFlip = HFlip = false;
};
void Sprite::writeOam(){
	u16* gfx = GfxBase + AnimFrame*4*64;
	oamSet(&oamMain, OamID, DrawX, DrawY, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, gfx, -1, false, false, VFlip, HFlip, false);
	
}
void Sprite::setAnimFrame(u8 _f){
	AnimFrame = _f;
}
void Sprite::setGfxPointer(u16* _g){
	GfxBase = _g;
}
void Sprite::setSpriteEntry(u8 _oid){
	OamID = _oid;
}
//Entity Methods
Entity::Entity(){
	CanMove = true;
}
void Entity::onLoad(){
	SpriteChanged = true;
	ISprite = Sprite();
	u8 _si = gp->lvl->getOamEntry();
	ISprite.setSpriteEntry(_si);
	aX = GridX = 0;
	aY = GridY = 0;
}
bool Entity::useOn(Item* ip, Entity* ep){
	return true;
}
bool Entity::tick(){
	//calculations and such
	calcGrid();
	calcSprite();
	return true;
}
void Entity::calcSprite(){
	if (SpriteChanged){
		u16 vx = gp->lvl->ViewX;
		u16 vy = gp->lvl->ViewY;
		u8 ts = gp->lvl->TileSize * 8;
		
		ISprite.DrawX = GridX * ts - vx * ts - REG_BG0HOFS + ts + aX;
		ISprite.DrawY = GridY * ts - vy * ts - REG_BG0VOFS + ts + aY;
		ISprite.writeOam();
		//SpriteChanged = false;
	}
}
void Entity::calcGrid(){
	if (abs(aX) >= gp->lvl->TileSize*8){
		gp->lvl->getBlock(GridX, GridY)->HasEntity = false;
		GridX += aX/(gp->lvl->TileSize*8);
		aX = aX%(gp->lvl->TileSize*8);
		Moving = false;
		gp->lvl->getBlock(GridX, GridY)->setEntity(this);
	}
	if (abs(aY) >= gp->lvl->TileSize*8){
		gp->lvl->getBlock(GridX, GridY)->HasEntity = false;
		GridY += aY/(gp->lvl->TileSize*8);
		aY = aY%(gp->lvl->TileSize*8);
		Moving = false;
		gp->lvl->getBlock(GridX, GridY)->setEntity(this);
	}
}
//Block Methods
Block::Block(){
	HasEntity = false;
	Solid = Opaque = true;
}
u16 Block::getTileIndex(){
	return TileIndex;
}

void Block::onLoad(int _xp, int _yp){
	XPos = _xp;
	YPos = _yp;
}
bool Block::tick(){
	if (HasEntity){
		IEntity->tick();
	}
	return true;
}
void Block::setEntity(Entity* _ne){
	if (!HasEntity){
		HasEntity = true;
		IEntity = _ne;
		IEntity->GridX = XPos;
		IEntity->GridY = YPos;
		IEntity->SpriteChanged = true;
		onEnter(_ne);
	}
}
bool Block::useOn(Item*, Entity*){return true;};
void Block::onEnter(Entity* _e){};
void Block::onUnload(){
	if (HasEntity){
		delete IEntity;
	}
}
void Block::setTileIndex(u16 _ti){
	TileIndex = _ti;
}

//Level Methods
void Level::onLoad(){
	
	TileSize = 2;
	SizeX = 512/(TileSize*8);
	SizeY = 256/(TileSize*8);
	
	loadCommon();
	
	swiCopy(TilesetTiles, BG_TILE_RAM(0), TilesetTilesLen/2);
	swiCopy(TilesetPal, BG_PALETTE, TilesetPalLen/4);

	swiCopy(SpritesTiles, SpriteBase, SpritesTilesLen/2);
	swiCopy(SpritesPal, SPRITE_PALETTE, SpritesPalLen/4);
	
	
	initBlocks();
	drawLevel();

	gp->som.playSFX(5, 1024);
	//gp->som.playBGM(3, true);
	//gp->som.setBGMVol(64);
}
void Level::loadCommon(){
	ViewX = 0;
	ViewY = 0;
	OamIndex = 0;
	AnimDirty = true;
	
	SpriteMapModeMain = SpriteMapping_1D_256;
	
	REG_BG0HOFS = (u16)(TileSize*8);
	REG_BG0VOFS = (u16)(TileSize*8);
	
	oamInit(&oamMain, SpriteMapModeMain, false);
	oamInit(&oamSub, SpriteMapModeSub, false);
	
	SpriteBase = oamGetGfxPtr(&oamMain, 0);
	
	oamEnable(&oamMain);
	oamEnable(&oamSub);
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
	for (std::vector<Block>::iterator it = Grid.begin(); it != Grid.end(); ++it){
		it->onUnload();
	}
}
bool Level::tick(){
	
	for (std::vector<Block>::iterator it = Grid.begin(); it != Grid.end(); ++it){
		it->tick();
	}
	if (((IPlayer->GridY - ViewY)*TileSize*8  + IPlayer->aY - 64) < 0 && ViewY > 0){
		REG_BG0VOFS -= 1;
	}
	if (((IPlayer->GridY - ViewY)*TileSize*8  + IPlayer->aY + 64) > 192 && ViewY < (SizeY - (192 / (TileSize*8)))){
		REG_BG0VOFS += 1;
	}
	if (((IPlayer->GridX - ViewX)*TileSize*8  + IPlayer->aX - 96) < 0 && ViewX > 0){
		REG_BG0HOFS -= 1;
	}
	if (((IPlayer->GridX - ViewX)*TileSize*8  + IPlayer->aX + 96) > 256 && ViewX < (SizeX - (256 / (TileSize*8)))){
		REG_BG0HOFS += 1;
	}
	if ((REG_BG0HOFS!=(u16)(TileSize*8)) && (REG_BG0HOFS%(TileSize*8)==0)){
		ViewX += (REG_BG0HOFS/(TileSize*8)-1);
		REG_BG0HOFS = (vu16)(TileSize*8);
		AnimDirty = true;
	}
	if ((REG_BG0VOFS!=(u16)(TileSize*8) && REG_BG0VOFS%(TileSize*8)==0)){
		ViewY += (REG_BG0VOFS/(TileSize*8)-1);
		REG_BG0VOFS = (vu16)(TileSize*8);
		AnimDirty = true;
	}
	
	
	
	if (AnimDirty){ drawLevel();};
	oamUpdate(&oamMain);
	oamUpdate(&oamSub);
	return true;
}
void Level::initBlocks(){
	for (int i=0; i<SizeX*SizeY; i++){ 
		Block b;
		b.TileIndex = (u16)i%32;
		b.onLoad(i%SizeX, i/SizeX);
		Grid.push_back(b);
	}
}

u8 Level::getOamEntry(){
	if (OamIndex<128){
		return OamIndex++;
	}
	else return 127;
}

bool Level::isPlayer(Entity* to){
	return to==IPlayer;
}
Block* Level::getBlock(int _x, int _y){
	if (_x<(SizeX) && _y<(SizeY) && _x>=0 && _y>=0){
		return (Block*)&Grid[_x + _y * SizeX];
	}
	return (Block*)&Grid[0];
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
	SFX[0] = SFX_EXPLODE;
	SFX[1] = SFX_HIT;
	SFX[2] = SFX_PICKUP;
	SFX[3] = SFX_PICKUP2;
	SFX[4] = SFX_SELECT;
	SFX[5] = SFX_WHIP;
	for (int i=0; i<sizeof(BGM)/sizeof(mm_word); i++){
		mmLoad(BGM[i]);
	}
	for (int i=0; i<sizeof(SFX)/sizeof(mm_word); i++){
		mmLoadEffect(SFX[i]);
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
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	REG_BG0CNT = BG_64x32 | BG_COLOR_256 | BG_MAP_BASE(8) | BG_TILE_BASE(0) | BG_PRIORITY(3);
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

