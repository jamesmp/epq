#include "ClassDeclarations.h"



class Level1: public Level{
	public:
		virtual void onLoad();
		virtual Level* copy();
	private:
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
