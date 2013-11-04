#include "Levels.h"
#include <maxmod9.h>
#include <vector>
#include <stdio.h>

#include "soundbank_bin.h"
#include "soundbank.h"
#include "Tileset.h"
#include "Sprites.h"

void Level1::initBlocks(){
	for (int i=0; i<SizeX*SizeY; i++){ 
		Block b;
		b.TileIndex = (u16)0;
		b.onLoad(i%SizeX, i/SizeX);
		Grid.push_back(b);
	}
}