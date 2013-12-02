#include "ClassDeclarations.h"



class Level1: public Level{
	public:
		virtual void onLoad();
		virtual Level* copy();
	private:
};

class Level2: public Level{
	public:
		virtual void onLoad();
		virtual Level* copy();
};

class Level3: public Level{
	public:
		virtual void onLoad();
		virtual Level* copy();
};

class Level4: public Level{
	public:
		virtual void onLoad();
		virtual Level* copy();
};

class Level5: public Level{
	public:
		virtual void onLoad();
		virtual Level* copy();
};

class Level6: public Level{
	public:
		virtual void onLoad();
		virtual Level* copy();
};

class LevelMainMenu: public Level{
	public:
		virtual void onLoad();
		virtual Level* copy();
};

class LevelWinLevel: public Level{
	public:
		virtual void onLoad();
		virtual Level* copy();
};
