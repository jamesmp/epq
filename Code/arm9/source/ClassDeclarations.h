#ifndef _CLASS_DEFINITIONS
#define _CLASS_DEFINITIONS

#include <nds.h>
#include <maxmod9.h>
#include <vector>

class Sprite;
class Item;
class Entity;
class Block;
class Level;
class SoundManager;
class ScoreManager;
class Game;

class Sprite{
	public:
		u16* GfxBase;
		u8 AnimFrame;
		u8 OamID;
		SpriteSize Size;
		SpriteColorFormat Colour;
		int DrawX;
		int DrawY;
		bool HFlip;
		bool VFlip;
	
		void writeOam();
		void setGfxPointer(u16*);
		void setAnimFrame(u8);
		void setSpriteEntry(u8);
		Sprite();
	private:
};



class Entity{
	public:
		int GridX;
		int GridY;
		int aX;
		int aY;
		bool SpriteChanged;
		bool CanMove;
		bool Moving;
		Sprite ISprite;
		
		Entity();
		virtual void onLoad();
		virtual bool useOn(Item*, Entity*);
		virtual bool tick();
		void calcSprite();
		void calcGrid();
	private:

};

class Block{
	public:
		int XPos;
		int YPos;
		u16 TileIndex;
		bool Solid;
		bool Opaque;
		bool HasEntity;
		Entity* IEntity;
		
		Block();
		virtual void onLoad(int, int);
		virtual bool tick();
		virtual bool useOn(Item*, Entity*);
		virtual void onUnload();
		virtual void onEnter(Entity*);
		void setTileIndex(u16);
		void setEntity(Entity*);
		u16 getTileIndex();
	private:
};

class Level{
	public:
		OamState OamTable;
		u8 OamIndex;
		int ViewX;
		int ViewY;
		int SizeX;
		int SizeY;
		u16 TileSize;
		u16* SpriteBase;
		bool AnimDirty;
		Entity* IPlayer;
		SpriteMapping SpriteMapModeMain;
		SpriteMapping SpriteMapModeSub;
		std::vector<Block> Grid;
		
		u8 getOamEntry();
		bool isPlayer(Entity*);
		virtual bool tick();
		virtual void initBlocks();
		virtual void onLoad();
		virtual void onUnload();
		Block* getBlock(int, int);
		void loadCommon();
		void drawLevel();
	private:
};

class ScoreManager{
		private:
				u16 HighScores[128];
				u16 ScoreIndex;
		public:
				void clearScores();
				void addScore(const char*, u16);
				u16 getHighscore(u16);
};

class SoundManager{
		private:
				mm_word BGM[(u8)11];
				mm_word SFX[(u8)6];
				bool playingBGM;
				bool playingSFX;
		public:
				SoundManager();
				void playBGM(u8, bool);
				void playSFX(u8, mm_word);
				void stopBGM();
				void setBGMVol(mm_word);
				//void setBGMRepeat(bool);
				void setBGMTempo(mm_word);
				//const char* getBGMName(int);
};

class Game{
		private:
				bool LevelLoaded;
				bool LevelPaused;
				int BG0h, BG1h, BG2h, BG3h, BG0h_sub, BG1h_sub, BG2h_sub, BG3h_sub;
				
		public:
				ScoreManager scm;
				SoundManager som;
				Level* lvl;

				int ticks;
				touchPosition touch;
				
				Game();        
				void mainLoop();
				void loadLevel(Level*);
};

#endif