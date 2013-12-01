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
		bool checkPlayer();
		void moveIdle();
		bool move(int);
		bool Multidir;
		u16 cooldown;
		u16 movecool;
		dir IMoveDir;
		dir IFaceDir;
		int Attack;
		int Cooltime;
};

class Skeleton: public Mob{
	public:
		virtual void onLoad();
};

class Rock: public Entity{
	public:
		virtual bool tick();
		virtual void onLoad();
		virtual bool useOn(Item*, Entity*);
		bool move();
		dir IMoveDir;
};