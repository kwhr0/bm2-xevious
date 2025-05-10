#include "game.h"
#include "base.h"
#include "sp.h"
#include "emitter.h"
#include "bg.h"
#include "chr.h"
#include "play.h"
#include "sprite.h"
#include "pattern.h"
#include "music.h"

#define RESERVE		2
#define WAITFRAME	3

#if RESERVE > 0
#define SPRITE_RN	RESERVE
#else
#define SPRITE_RN	1
#endif

static Sprite spriteR[SPRITE_RN];
static u8 playf, trigger;
static u16 score, scoreNext = 2000, hiscore = 4000;
static u8 scoreBuf[6], hiscoreBuf[6];
static SpriteContext ctx_r;
static s8 reserve;

u16 scoreGet(void) {
	return score;
}

void scorePrint(void) {
	chrTime(1);
	chrLocate(8, 0);
	chrPuts("1UP");
	chrLocate(18, 0);
	chrPuts("HISCORE");
	chrLocate(4, 2);
	chrValue(scoreBuf, sizeof(scoreBuf), score);
	chrPutsBuf(scoreBuf);
	chrPut('0');
	chrLocate(20, 2);
	chrValue(hiscoreBuf, sizeof(hiscoreBuf), hiscore);
	chrPutsBuf(hiscoreBuf);
	chrPut('0');
}

void scoreUpdate(void) {
	static u8 timer;
	if (!(++timer & 7)) {
		chrTime(1);
		chrLocate(8, 0);
		chrPuts(timer & 8 ? "1UP" : "   ");
	}
}

static void reserveSet(Pattern *pat) {
	Sprite *p;
	spriteContext(&ctx_r);
	p = spriteCreate(0, pat);
	if (p) {
		p->x = 2 << PS;
		p->y = 2 + (6 * reserve) << PS;
		p->animSpeed = 1 << AS - 1;
		p->flags = SF_ERASE_NO_ANIM;
	}
	spriteContext(nil);
}

void reserveInc(void) {
	reserveSet(pat_solvalou_r);
	reserve++;
}

u8 reserveDec(void) {
	reserve--;
	reserveSet(pat_solvalou_ro);
	return reserve < 0;
}

void scoreAdd(u16 v) {
	score += v;
	chrValue(scoreBuf, sizeof(scoreBuf), score);
	if (hiscore < score) hiscore = score;
	chrValue(hiscoreBuf, sizeof(hiscoreBuf), hiscore);
	if (score >= scoreNext) {
		if (scoreNext == 2000) scoreNext = 6000;
		else scoreNext += 6000;
		playStart(6, MUSIC_1UP, 0);
		reserveInc();
	}
}

u8 triggerGet(void) {
	u8 r = ~trigger & KEY & (KEY_A | KEY_B);
	trigger = KEY;
	return r;
}

static void gameStart(void) {
	u8 buf[3];
	chrTime(60);
	chrLocate(10, 18);
	chrPuts("READY!");
	chrValue(buf, sizeof(buf), reserve);
	chrLocate(0, 22);
	chrPuts(buf);
	chrPuts(" SOLVALOU LEFT");
	playStart(6, MUSIC_START, 0);
	playf = 0;
}

static u8 gameUpdate(void) {
	u8 f = playing(6) == MUSIC_START;
	if (playf && !f) playStart(0, MUSIC_FLY, PF_LOOP);
	playf = f;
	spriteContext(&ctx_r);
	spriteUpdate();
	spriteContext(nil);
	return 1;
}

void gameInit(void) {
	u8 i;
	reserve = 0;
	spriteContext(&ctx_r);
	spriteSetupArray(spriteR);
	spriteView(154, 6);
	spriteContext(nil);
#if RESERVE
	for (i = 0; i < RESERVE; i++) reserveInc();
#endif
	cls();
	chrInit();
	scorePrint();
	bgInit();
	emitterInit();
}

void gameMain(void) {
	u8 r;
	spInit();
	gameStart();
	bgStart();
	emitterStart(); // must be after bgStart()
	do {
#ifndef BG_ENABLE
		clrs();
#endif
		setkey();
		if (bgUpdate()) emitterStart();
		emitterUpdate();
		r = spUpdate() && gameUpdate();
		spriteUpdate();
		scoreUpdate();
		chrUpdate();
		waitN(WAITFRAME);
	} while (r);
	playStopAll();
}
