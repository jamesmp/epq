#include "ClassDeclarations.h"

enum dir{ left, right, up, down };

class Level1: public Level{
	public:
		virtual void initBlocks();
		virtual void onLoad();
	private:
};

class Player: public Entity{
	public:
		virtual bool tick();
		dir IMoveDir;
};