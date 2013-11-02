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
		SpriteEntry* ISpriteEntry;
		SpriteEntry* getSpriteEntry();
		void setSpriteEntry(SpriteEntry*);
	private:
};



class Entity{
	public:
		int GridX;
		int GridY;
		int aX;
		int aY;
		bool SpriteChanged;
		Sprite ISprite;
		
		Entity();
		virtual void onLoad();
		virtual bool useOn(Item*, Entity*);
		virtual bool tick();
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
		virtual void onLoad();
		virtual bool tick();
		virtual bool useOn(Item*, Entity*);
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
		bool AnimDirty;
		bool OamDirty;
		Entity* Player;
		SpriteMapping SpriteMapModeMain;
		SpriteMapping SpriteMapModeSub;
		SpriteEntry OamEntryMain[128];
		SpriteEntry OamEntrySub[128];
		std::vector<Block> Grid;
		
		SpriteEntry* getOamEntry();
		void flipOam();
		virtual bool tick();
		virtual void onLoad();
		virtual void onUnload();
	private:
		void drawLevel();
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
				mm_word SFX[(u8)16];
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