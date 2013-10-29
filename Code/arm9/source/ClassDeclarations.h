#include <nds.h>
#include <vector>

class Sprite;
class Background;
class Entity;
class Object;
class Level;
class SoundManager;
class ScoreManager;
class Game;

class Sprite{
	public:
		SpriteEntry* SpriteEntry
		SpriteEntry* getSpriteEntry();
		SpriteEntry* setSpriteEntry();
	private:
};

class BackgroundObj{
	public:
		u16* BlitData;
		int BlitWidth;
		int BlitHeight;
		int DrawLayer;
		int TileX;
		int TileY;
		
	private:
};

class Entity{
	public:
		int GridX;
		int GridY;
		int aX;
		int aY;
		Sprite ISprite;
		
		virtual bool useOn(Item*, Entity*)
		virtual bool tick();
	private:
};

class Block{
	public:
		int XPos;
		int YPos;
		bool Solid;
		bool Opaque;
		Entity* IEntity;
		
		virtual void onLoad();
		virtual bool tick();
		virtual bool useOn(Item*, Entity*);
	private:
};

class Level{
	public:
		OamState OamTable;
		int OamIndex;
		int ViewX;
		int ViewY;
		int SizeX;
		int SizeY;
		SpriteMapping SpriteMapModeMain;
		SpriteMapping SpriteMapModeSub;
		vector<Block> Grid;
		
		SpriteEntry* getOamEntry();
		void flipOam();
		virtual bool tick();
		virtual void onLoad();
		virtual void onUnload();
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
				mm_word BGM[(u8)8];
				mm_word SFX[(u8)16];
				bool playingBGM;
				bool playingSFX;
		public:
				SoundManager();
				void playBGM(u8, bool);
				void playSFX(u8, mm_word;);
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
		public:
				BGManager bgm;
				ScoreManager scm;
				SoundManager som;
				Level lvl;

				int ticks;
				touchPosition touch;
				
				Game();        
				void mainLoop();
				void loadLevel(*Level);
};