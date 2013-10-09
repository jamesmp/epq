#include <nds.h>
#include <vector>

class Object;
class DrawableObject: public Object;
class Background;
class Sprite;
class BGManager;
class ScoreManager;
class SoundManager;
class Level;
class Game;

class Object{
	private:
		
	public:
		virtual void tick();
		virtual void draw();
};
class DrawableObject; public Object{
	private:
		
	public:
		int XPos;
		int YPos;
		bool Solid;
		bool Opaque;
};
class Background{
	private:
		
	public:
		u16* BlitData;
		int BlitWidth;
		int BlitHeight;
		int DrawLayer;
		int SpriteX;
		int SpriteY;
		
		void draw();
		
};
class Sprite{
	private:
		
	public:
		void draw();
};


class BGManager{
	private:
		bool Dirty;
		*Game GameInstance;
	public:
		BGManager(*Game);
		void bgmFlipBuffer();

		
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
		mm_word BGM[8];
		mm_word SFX[16];
		*Game GameInstance;
	public:
		void playBGM(int, bool);
		void playSFX(int);
		void stopBGM();
		void setBGMVol(mm_word);
		void setBGMRepeat(bool);
		void setBGMTempo(mm_word);
		const char* getBGMName(int);
		
};


class Level{
	private:
		std::vector<Object*> ObjList;
		Game* GameInstance;
		bool MainScreen;
		OamState* OamInstance
		SpriteMapping SpriteMapMode;
	public:
		Level(*Game, bool);
		void onLoad();
		void onUnload();
		
};
class Game{
	private:

	public:
		BGManager bgm;
		ScoreManager scm;
		SoundManager som;
		Level lvl;

		int ticks;
		touchPosition touch;
		
		Game();	
		void mainLoop();
		void loadLevel(Level*);
};
