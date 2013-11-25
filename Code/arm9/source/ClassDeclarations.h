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
		void setGfxBase(int);
		void setAnimFrame(u8);
		void setSpriteEntry(u8);
		Sprite();
	private:
};

class Item{
	public:
		u8 Def;
		u8 Att;
		u8 getDef();
		u8 getAtt();
		Item();
		Item(u8, u8);
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
		bool Solid;
		Sprite ISprite;
		
		Entity();
		virtual void onLoad();
		virtual bool useOn(Item*, Entity*);
		virtual bool tick();
		void loadCommon();
		void calcSprite();
		void calcGrid();
		void translate(int, int);
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
		virtual void useOn(Item*, Entity*);
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
		std::vector<Block*> Grid;
		
		u8 getOamEntry();
		bool isPlayer(Entity*);
		virtual bool tick();
		virtual void initBlocks();
		virtual void onLoad();
		virtual void onUnload();
		Block* getBlock(int, int);
		bool testSolid(int, int);
		void loadCommon();
		void drawLevel();
		
		virtual ~Level();
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
				bool ChangeLevel;
		public:
				ScoreManager scm;
				SoundManager som;
				Level* lvl;
				Level* newlvl;

				int ticks;
				touchPosition touch;
				
				Game();        
				void mainLoop();
				void loadLevel(Level*);
				void setLevel(Level*);
};

#endif