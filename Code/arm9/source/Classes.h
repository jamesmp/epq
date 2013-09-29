#include <nds.h>
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
class BackgroundObject: public DrawableObject {
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
class SpriteObject: public DrawableObject{
	private:
		
	public:
		void draw();
};


class BGManager{
	private:
		
	public:

};
class ScoreManager{
	private:

	public:

};
class SoundManager{
	private:

	public:

};


class Level{
	private:

	public:
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
		
		void mainLoop();
		void loadLevel(Level*);
};
