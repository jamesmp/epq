
#include "ClassDeclarations.h"

class Wall: public Block{
	public:
		Wall();
};

class Floor: public Block{
	public:
		Floor();
};

class BlockFactory{
	public:
		Wall makeWall(u16);
		Floor makeFloor(u16);
};
