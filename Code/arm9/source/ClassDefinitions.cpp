#include "ClassDefinitions.h"

//Sprite Methods

//BackgroundObj Methods

//Entity Methods

//Block Methods

//Level Methods
virtual void Level::onLoad(){
	Grid = vector<Block>;
	oamInit(&oamMain, SpriteMapModeMain, false);
	oamInit(&oamSub, SpriteMapModeSub, false);
};
//ScoreManager Methods

//SoundManager Methods
SoundManager::SoundManager(){
		GameInstance = _gameInstance;
		mmInitDefaultMem((mm_addr)soundbank_bin);
		
}

//Game Methods
Game::Game(){
		bgm = BGManager(this);
		som = SoundManager(this);
		scm = ScoreManager(this);
}

Game::loadLevel(*Level){
		
}