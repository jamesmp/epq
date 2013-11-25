
#include "ClassDeclarations.h"

class Wall: public Block{
	public:
		Wall();
};

class Floor: public Block{
	public:
		Floor();
};

class Door: public Block{
	public:
		Door();
		void linkToBlock(int, int);
		void linkToLevel(Level*);
		virtual void useOn(Item*, Entity*);
		Level* lp;
		int bx;
		int by;
		bool LinkedBlock;
		bool LinkedLevel;
};

class BlockFactory{
	public:
		Wall* makeWall(u16);
		Floor* makeFloor(u16);
		Door* makeDoor(u16);
};
