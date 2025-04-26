#include "base.h"
#include "bg.h"

#define BG_XN		884
#define BG_YN		460

#define STAGE		1
#define LOOP		6
#define PRE_BLANK1	90
#define PRE_BLANK2	64
#define ADJUST		71

static u8 stage;
static s16 xofs, xn;
static const u8 yofs[] = {
	58, 90, 13, 77, 32, 66, 0, 
	87, 52, 7, 77, 87, 40, 13, 67, 90
};

void bgInit(void) {
	stage = STAGE - 1;
	xofs = 0;
}

s16 bgOfs(void) {
	return stage * (BG_XN + PRE_BLANK2) + xofs - ADJUST;
}

u8 bgStage(void) {
	return stage + 1;
}

u8 bgUpdate(void) {
	if (++xofs >= xn) {
		xofs = 0;
		xn = BG_XN + PRE_BLANK2;
		if (++stage >= sizeof(yofs)) {
			stage = LOOP;
			return 1;
		}
	}
	return 0;
}

void bgStart(void) {
	if (xofs >= 7 * (BG_XN + PRE_BLANK2) / 10 && ++stage >= sizeof(yofs)) stage = LOOP;
	xofs = -PRE_BLANK1;
	xn = BG_XN + PRE_BLANK2;
}
