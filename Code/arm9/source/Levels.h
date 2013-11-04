#include "ClassDeclarations.h"

enum moveDir{ move_left, move_right, move_up, move_down };

class Level1: public Level{
	public:
		virtual void initBlocks();
		virtual void onLoad();
	private:
};

class Player: public Entity{
	public:
		virtual bool tick();
		moveDir IMoveDir;
		bool Moving;
};