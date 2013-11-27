#include "ClassDeclarations.h"
#include <maxmod9.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include "soundbank_bin.h"
#include "soundbank.h"
#include "Tileset.h"
#include "Sprites.h"
#include "Levels.h"
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
	u16* gfx = oamGetGfxPtr(&oamMain, ((int)GfxBase - (int)gp->lvl->SpriteBase)/256 + AnimFrame);
	oamSet(&oamMain, OamID, DrawX, DrawY, 1, 0, SpriteSize_16x16, SpriteColorFormat_256Color, gfx, -1, false, false, VFlip, HFlip, false);
}
void Sprite::setAnimFrame(u8 _f){
	AnimFrame = _f;
}
void Sprite::setGfxBase(int _i){
	GfxBase = oamGetGfxPtr(&oamMain, _i);
}
void Sprite::setSpriteEntry(u8 _oid){
	OamID = _oid;
}
void Sprite::onUnload(){
	oamSet(&oamMain, OamID, 0, 0, 1, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 0, -1, false, true, VFlip, HFlip, false);
}
//Item Methods
u8 Item::getAtt(){
	return Att;
}
u8 Item::getDef(){
	return Def;
}
Item::Item(u8 _att, u8 _def){
	Def = _def;
	Att = _att;
}
Item::Item(){
	Def = 0;
	Att = 0;
}

//Entity Methods
Entity::Entity(){
	CanMove = true;
}
void Entity::loadCommon(){
	SpriteChanged = true;
	Moving = false;
	ISprite = Sprite();
	u8 _si = gp->lvl->getOamEntry();
	ISprite.setSpriteEntry(_si);
	aX = GridX = 0;
	aY = GridY = 0;
}
void Entity::onLoad(){
	loadCommon();
	Solid = true;
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
	if (aX==0 && aY==0){
		Moving = false;
	}
}

void Entity::translate( int _rx, int _ry){
	gp->lvl->getBlock(GridX, GridY)->HasEntity = false;
	GridX+=_rx;
	GridY+=_ry;
	gp->lvl->getBlock(GridX, GridY)->setEntity(this);
}

void Entity::onUnload(){
	ISprite.onUnload();
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
	LightValue=0;
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
void Block::useOn(Item* _ip, Entity* _ep){
	if (HasEntity){
		IEntity->useOn(_ip, _ep);
	}
};
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
	swiCopy(TilesetPal, BG_PALETTE, TilesetPalLen/2);

	swiCopy(SpritesTiles, SpriteBase, SpritesTilesLen/2);
	swiCopy(SpritesPal, SPRITE_PALETTE, SpritesPalLen/2);
	
	
	initBlocks();
	drawLevel();

	gp->som.playSFX(5, 1024);
	
}
void Level::loadCommon(){
	ViewX = 0;
	ViewY = 0;
	OamIndex = 0;
	AnimDirty = true;
	
	
	SpriteMapModeMain = SpriteMapping_1D_256;
	
	REG_BG0HOFS = (u16)(TileSize*8);
	REG_BG0VOFS = (u16)(TileSize*8);
	
	REG_BG1HOFS = (u16)(TileSize*8);
	REG_BG1VOFS = (u16)(TileSize*8);
	
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
			u16 tI = Grid[tX+tY*SizeX]->getTileIndex();
			u8 lI = Grid[tX+tY*SizeX]->LightValue;
			
			
			for (int x=0; x<TileSize; x++){
				for (int y=0; y<TileSize; y++){
					if (j*TileSize<32){
						BG_MAP_RAM(8)[(j*TileSize)+x + (i*TileSize+y)*32] = (u16)(tI*TileSize*TileSize+x+(TileSize*y));
						BG_MAP_RAM(10)[(j*TileSize)+x + (i*TileSize+y)*32] = (u16)(TileSize*TileSize*(((255-lI)/255)+31));
					}
					else{
						BG_MAP_RAM(8)[(j*TileSize)+x + (i*TileSize+y)*32+992] = (u16)(tI*TileSize*TileSize+x+(TileSize*y));
						BG_MAP_RAM(10)[(j*TileSize)+x + (i*TileSize+y)*32+992] = (u16)(TileSize*TileSize*(((255-lI)/255)+31));
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
	for (int i=0; i<Grid.size(); i++){
		Grid[i]->onUnload();
		delete Grid[i];
	}
}
bool Level::tick(){
	
	for (int i=0; i<Grid.size(); i++){
		Grid[i]->tick();
		Grid[i]->LightValue = 0;
	}
	calcLight(IPlayer->GridX, IPlayer->GridY, 7);
	if (((IPlayer->GridY - ViewY)*TileSize*8  + IPlayer->aY - 64) < 0 && ViewY > 0){
		REG_BG0VOFS -= 1;
		REG_BG1VOFS -= 1;
	}
	if (((IPlayer->GridY - ViewY)*TileSize*8  + IPlayer->aY + 64) > 192 && ViewY < (SizeY - (192 / (TileSize*8)))){
		REG_BG0VOFS += 1;
		REG_BG1VOFS += 1;
	}
	if (((IPlayer->GridX - ViewX)*TileSize*8  + IPlayer->aX - 96) < 0 && ViewX > 0){
		REG_BG0HOFS -= 1;
		REG_BG1HOFS -= 1;
	}
	if (((IPlayer->GridX - ViewX)*TileSize*8  + IPlayer->aX + 96) > 256 && ViewX < (SizeX - (256 / (TileSize*8)))){
		REG_BG0HOFS += 1;
		REG_BG1HOFS += 1;
	}
	
	if ((REG_BG0HOFS!=(u16)(TileSize*8)) && (REG_BG0HOFS%(TileSize*8)==0)){
		ViewX += (REG_BG0HOFS/(TileSize*8)-1);
		REG_BG0HOFS = (vu16)(TileSize*8);
		REG_BG1HOFS = (vu16)(TileSize*8);
		AnimDirty = true;
	}
	if ((REG_BG0VOFS!=(u16)(TileSize*8) && REG_BG0VOFS%(TileSize*8)==0)){
		ViewY += (REG_BG0VOFS/(TileSize*8)-1);
		REG_BG0VOFS = (vu16)(TileSize*8);
		REG_BG1VOFS = (vu16)(TileSize*8);
		AnimDirty = true;
	}
	
	
	
	if (AnimDirty){ drawLevel();};
	oamUpdate(&oamMain);
	oamUpdate(&oamSub);
	return true;
}
void Level::initBlocks(){
	for (int i=0; i<SizeX*SizeY; i++){ 
		Block* b = new Block();;
		b->TileIndex = (u16)i%32;
		b->onLoad(i%SizeX, i/SizeX);
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
		return Grid[_x + _y * SizeX];
	}
	return Grid[0];
}
bool Level::testSolid(int _x, int _y){
	return getBlock(_x, _y)->Solid;
}
void Level::calcLight(int x, int y, u8 intensity){
	Block* b;
	Block* c = getBlock(x, y);
	if (!(c->Opaque)){
		for (int i=0; i<4; i++){
			s8 GX = (i-2)*(i%2);
			s8 GY = (i-1)*((i+1)%2);
			b = getBlock(x+GX, y+GY);
			
			if ((b->LightValue<(intensity-1)) && (intensity>1)){
				b->LightValue = intensity-1;
				calcLight(x+GX, y+GY, intensity-1);
			}
		}
	}
}
Level::~Level(){};
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
	ChangeLevel = false;
	
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE| DISPLAY_BG1_ACTIVE);
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	REG_BG0CNT = BG_64x32 | BG_COLOR_256 | BG_MAP_BASE(8) | BG_TILE_BASE(0) | BG_PRIORITY(3);
	REG_BG1CNT = BG_64x32 | BG_COLOR_256 | BG_MAP_BASE(10) | BG_TILE_BASE(0) | BG_PRIORITY(0);
	iprintf("setup\n");
}

void Game::loadLevel(Level* nLvl){
	if (LevelLoaded){
		lvl->onUnload();
		delete lvl;
	}
	lvl = nLvl;
	lvl->onLoad();
	LevelLoaded = true;	
	LevelPaused = false;
}

void Game::setLevel(Level* nlvl){
	newlvl = nlvl;
	ChangeLevel = true;
}

void Game::mainLoop(){
	scanKeys();
	touchRead(&touch);
	
	if (ChangeLevel){
		loadLevel(newlvl);
		ChangeLevel = false;
	}
	if (LevelLoaded && !LevelPaused){
		lvl->tick();
	}
}

