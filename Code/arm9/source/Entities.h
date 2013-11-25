#include "ClassDeclarations.h"

enum dir{ left=0, right=1, up=2, down=3 };

class Player: public Entity{
	public:
		Item MainHand;
		virtual bool tick();
		virtual void onLoad();
		dir IMoveDir;
		dir IFaceDir;
};

class Mob: public Entity{
	public:
		virtual bool tick();
		virtual void onLoad();
		virtual bool useOn(Item*, Entity*);
		void moveIdle();
		bool move(int);
		dir IMoveDir;
		dir IFaceDir;
		int HitPoints;
};