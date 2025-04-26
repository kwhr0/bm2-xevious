#include "game.h"
#include "base.h"

void main(void) {
	DISP = 0xc0;
	gameInit();
	ei();
	do gameMain();
	while (!reserveDec());
}
