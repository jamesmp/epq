#include "ClassDeclarations.h"



class Level1: public Level{
	public:
		virtual void initBlocks();
		virtual void onLoad();
	private:
};

class LevelMainMenu: public Level{
	public:
		virtual void initBlocks();
		virtual void onLoad();
};

