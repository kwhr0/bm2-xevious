#include "chr.h"
#include "sprite.h"
#include "pattern.h"

#define SPRITE_CN		48

typedef struct {
	Sprite s;
	s8 timer;
	u8 *ptr;
} SpriteC;

static SpriteC spriteC[SPRITE_CN];
static SpriteContext ctx_c;
static u8 chrX, chrY;
static s8 time;

void chrTime(u8 t) {
	time = t ? t : -1;
}

void chrValue(u8 *buf, u8 n, u16 v) {
	u8 *p = buf + n;
	*--p = 0;
	do {
		u16 d = v / 10;
		*--p = v - d * 10 + '0';
		v = d;
	} while (v);
	while (p > buf) *--p = ' ';
}

void chrLocate(u8 x, u8 y) {
	chrX = 150 - (y << 2);
	chrY = 2 + 3 * x;
}

static u8 behaviorChr(Sprite *p) {
	SpriteC *s = (SpriteC *)p;
	if (!s->ptr) return s->timer < 0 || s->timer--;
	p->flags = s->timer ? 0 : SF_HIDDEN;
	u8 c = *s->ptr;
	if (c) {
		s16 f = c - ' ' << AS;
		if (p->frame != f) {
			spriteFrame(p, f);
			s->timer = 1;
		}
		else if (s->timer) s->timer--;
	}
	return 1;
}

Sprite *chrPut(u8 c) {
	spriteContext(&ctx_c);
	SpriteC *s = (SpriteC *)spriteCreate(0, pat_chr);
	if (s) {
		s->s.behavior = behaviorChr;
		s->s.x = chrX << PS;
		s->s.y = chrY << PS;
		s->s.animSpeed = 0;
		spriteFrame(&s->s, c - ' ' << AS);
		s->timer = time;
		s->ptr = nil;
		chrY += 6;
	}
	spriteContext(nil);
	return &s->s;
}

void chrPuts(u8 *str) {
	u8 c;
	while (c = *str++) chrPut(c);
}

void chrPutsBuf(u8 *str) {
	u8 c;
	while (c = *str) {
		SpriteC *s = (SpriteC *)chrPut(c);
		s->timer = 1;
		s->ptr = str++;
	}
}

static void chrInitSub(void) {
	spriteContext(&ctx_c);
	spriteSetupArray(spriteC);
	spriteContext(nil);
}

void chrInit(void) {
	chrX = 100;
	chrY = 0;
	time = 0;
	chrInitSub();
}

void chrUpdate(void) {
	spriteContext(&ctx_c);
	spriteUpdate();
	spriteContext(nil);
}

void chrFlush(void) {
	chrUpdate();
	chrInitSub();
}
