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
		
		gp->lvl->tick();
		swiWaitForVBlank();
	}

}
