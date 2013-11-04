/*---------------------------------------------------------------------------------

	Basic template code for starting a DS app

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include "ClassDeclarations.h"
#include "Levels.h"
//---------------------------------------------------------------------------------
Game* gp;
int main(void) {
//---------------------------------------------------------------------------------
	consoleDemoInit();
	Game g; 
	gp = &g;
	Level1 l;
	Level* lp = &l;
	gp->loadLevel(lp);
	iprintf("Hello World!\n");
	while(true) {
		scanKeys();
		
		
		if (keysHeld() & KEY_RIGHT){gp->lvl->Player->aX+=1;};
		if (keysHeld() & KEY_LEFT){gp->lvl->Player->aX-=1;};
		if (keysHeld() & KEY_UP){gp->lvl->Player->aY-=1;};
		if (keysHeld() & KEY_DOWN){gp->lvl->Player->aY+=1;};
		gp->lvl->tick();
		swiWaitForVBlank();
	}

}
