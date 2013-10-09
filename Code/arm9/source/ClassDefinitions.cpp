#include "ClassDefinitions.h"

// Level Methods
Level::Level(*Game _gameInstance, bool _mainScreen){
	GameInstance = _gameInstance;
	MainScreen = _mainScreen;
}

void Level::onLoad(){
	ObjList = ObjList<*Object>
	MainScreen ? OamInstance = &oamMain : OamInstance = &oamSub;
	oamInit(OamInstance, SpriteMapMode, false);
}

void Level::onUnload(){
	for (std::vector<*Object>::iterator it = ObjList.begin(); it != ObjList.end(); ++it){
		*it->unLoad();
	}
}

//BGManager Methods
BGManager::BGManager(*Game _gameInstance){
	GameInstance = _gameInstance;
	
}

//SoundManager Methods
SoundManager::SoundManager(*Game _gameInstance){
	GameInstance = _gameInstance;
	mmInitDefaultMem((mm_addr)soundbank_bin);
	
}

//Game Methods
Game::Game(){
	bgm = BGManager(this);
	
}