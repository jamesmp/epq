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
	//consoleDemoInit();
	Game g; 
	gp = &g;
	Level* lp = new LevelMainMenu();
	gp->setLevel(lp);
	iprintf("Hello World!\n");
	while(true) {
		gp->mainLoop();
		swiWaitForVBlank();
	}

}
