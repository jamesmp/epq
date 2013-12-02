
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

class Podium: public Block{
	public:
		Podium();
		void linkToLevel(Level*);
		virtual void useOn(Item*, Entity*);
		Level* lp;
		bool LinkedLevel;
};

class Triggerable: public Block{
	public:
		bool Triggered;
		bool Active;
		virtual void trigger(bool);
		Triggerable();
};

class Plate: public Block{
	public:
		bool LastState;
		Triggerable* tp;
		Plate();
		virtual bool tick();
		void linkTrigger(Triggerable*);
};

class Actuator: public Triggerable{
	public:
		Actuator();
		bool Up;
		bool Locked;
		virtual void trigger(bool);
};
class Latch: public Triggerable{
	public:
		bool Triggered;
		Latch();
		virtual void trigger(bool);
};
class BlockFactory{
	public:
		Wall* makeWall(u16);
		Floor* makeFloor(u16);
		Door* makeDoor(u16);
		Actuator* makeActuator(u16);
		Plate* makePlate(u16, Triggerable*);
		Plate* makePlate(u16);
		Latch* makeLatch(u16);
		Podium* makePodium(u16, Level*);
};
