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
	g.som.playBGM(2, true);
	iprintf("Hello World!\n");
	u16 tempo = 1024;
	u16 vol = 512;
	u8 song = 2;
	u8 oldsong = song;
	while(1) {
		scanKeys();
		swiWaitForVBlank();
		if (keysDown() & KEY_UP){ vol += 64; };
		if (keysDown() & KEY_DOWN){ vol -= 64; };
		
		if (keysDown() & KEY_LEFT){ tempo -= 64; };
		if (keysDown() & KEY_RIGHT){ tempo += 64; };
		
		if (keysDown() & KEY_A){ song++; };
		if (keysDown() & KEY_B){ song--; };
		if (vol<0){ vol = 0;};
		if (vol>1024){ vol = 1024;};
		if (tempo<512){ tempo = 512;};
		if (tempo>2048){ tempo = 2048;};
		if (song<0){ song = 8;};
		if (song>8){ song = 0;};
		if (oldsong!=song){ g.som.playBGM(song, true); };
		g.som.setBGMVol(vol);
		g.som.setBGMTempo(tempo);
		oldsong = song;
		iprintf("Song: %i Volume: %i Tempo: %i\n", song, vol, tempo);
	}

}
