/*---------------------------------------------------------------------------------

	Basic template code for starting a DS app

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include "ClassDeclarations.h"
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	consoleDemoInit();
	Game g;
	Level l;
	Level* lp = &l;
	g.loadLevel(lp);
	iprintf("Hello World!\n");
	while(1) {
		scanKeys();
		swiWaitForVBlank();
		//if (keysDown() & KEY_RIGHT){REG_BG0HOFS+=1;};
		//if (keysDown() & KEY_LEFT){REG_BG0HOFS-=1;};
		//if (keysDown() & KEY_UP){REG_BG0VOFS-=1;};
		//if (keysDown() & KEY_DOWN){REG_BG0VOFS+=1;};
		g.lvl->tick();
		
	}

}
