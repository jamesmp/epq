#include "ClassDefinitions.h"

// Level Methods
Level::Level(Game* _gameInstance, bool _mainScreen){
	GameInstance = _gameInstance;
	MainScreen = _mainScreen;
}

void Level::onLoad(){
	ObjList = ObjList<Object*>
	oamInit(&oamMain, SpriteMapMode, false);
	oamInit(&oamSub, SpriteMapMode, false);
}

void Level::onUnload(){
	for (std::vector<Object*>::iterator it = ObjList.begin(); it != ObjList.end(); ++it){
		*it->unLoad();
	}
}

//Object Methods
bool Object::onLoad(*Level _levelInstance){
	LeveInstance = _levelInstance;
}

//DrawableObject Methods
bool DrawableObject::onLoad(*Level _levelInstance){
	
}
//BGManager Methods
BGManager::BGManager(Game* _gameInstance){
	GameInstance = _gameInstance;
	
}

//SoundManager Methods
SoundManager::SoundManager(Game* _gameInstance){
	GameInstance = _gameInstance;
	mmInitDefaultMem((mm_addr)soundbank_bin);
	
}

//ScoreManager Methods
ScoreManager::ScoreManager(Game* _gameInstance){
	GameInstance = _gameInstance;
}

//Game Methods
Game::Game(){
	bgm = BGManager(this);
	som = SoundManager(this);
	scm = ScoreManager(this);
}

Game::loadLevel(*Level){
	
}