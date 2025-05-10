#include "base.h"
#include "bg.h"

#define BG_XN		884
#define BG_YN		460

#define STAGE		1
#define LOOP		6
#define PRE_BLANK1	90
#define PRE_BLANK2	64
#define ADJUST		71
#define SRC_XN		12

typedef struct {
	u8 *p;
	u16 r;
	u8 d, c, n;
} ExpandWork;

static ExpandWork w[SRC_XN << 3];
static u8 *yofsp;
static u8 stage;
static s16 xofs, xn;
static const u8 yofs[] = {
	58, 90, 13, 77, 32, 66, 0, 
	87, 52, 7, 77, 87, 40, 13, 67, 90
};
#define BG		((u8 *)0xf000)
#define BGSIZE	(SRC_XN * 136)
static u8 *bgp;

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

#ifdef BG_ENABLE

static void setup(void) {
	extern const u16 bgdata[];
	int ofs = (*yofsp << 2) + 2;
	ExpandWork *p = w;
	u16 *op = &bgdata[ofs];
	u8 x;
	for (x = 0; x < SRC_XN << 3; x++) {
		p->p = &bgdata[*op++];
		p->d = 0; // compressed datum
		p->n = 0; // bit count of compressed datum
		p->c = 0; // current pixel ON/OFF
		p->r = 0; // run length
		p++;
	}
	if (++yofsp >= yofs + sizeof(yofs)) yofsp = (u8 *)yofs + LOOP;
}

static int get1bit(ExpandWork *p) {
	int r;
	if (!p->n) {
		p->d = *p->p++;
		p->n = 8;
	}
	r = (p->d & 0x80) != 0;
	p->d <<= 1;
	p->n--;
	return r;
}

static void bgDraw() {
	ExpandWork *p = w;
	u8 *dp = bgp;
	int x;
	for (x = 0; x < SRC_XN; x++) {
		u8 d = 0;
		int i;
		for (i = 0; i < 8; i++) {
			if (!p->r) {
				if (get1bit(p)) { // 2-511pixels
					int j, r = 0;
					for (j = 0; j < 9; j++) r = r << 1 | get1bit(p);
					p->r = r;
				}
				else p->r = 1; // 1pixel
				p->c = !p->c;
			}
			p->r--;
			d <<= 1;
			if (!p->c) d |= 1;
			p++;
		}
		*dp++ = d;
	}
}

#endif

u8 bgUpdate(void) {
#ifdef BG_ENABLE
	int i, size;
	u8 *sp, *dp;
	if (xofs >= 0) bgDraw();
	size = BG + BGSIZE - bgp;
	sp = bgp;
	dp = GBASE + XN * 18 + 10;
	for (i = size / SRC_XN; i > 0; i--) {
		dp[0] = sp[0]; dp[1] = sp[1]; dp[2] = sp[2]; dp[3] = sp[3];
		dp[4] = sp[4]; dp[5] = sp[5]; dp[6] = sp[6]; dp[7] = sp[7];
		dp[8] = sp[8]; dp[9] = sp[9]; dp[10] = sp[10]; dp[11] = sp[11];
		sp += SRC_XN;
		dp += XN;
	}
	sp = BG;
	for (i = (BGSIZE - size) / SRC_XN; i > 0; i--) {
		dp[0] = sp[0]; dp[1] = sp[1]; dp[2] = sp[2]; dp[3] = sp[3];
		dp[4] = sp[4]; dp[5] = sp[5]; dp[6] = sp[6]; dp[7] = sp[7];
		dp[8] = sp[8]; dp[9] = sp[9]; dp[10] = sp[10]; dp[11] = sp[11];
		sp += SRC_XN;
		dp += XN;
	}
	bgp -= SRC_XN;
	if (bgp < BG) bgp = &BG[BGSIZE - SRC_XN];
#endif
	if (++xofs >= xn) {
		xofs = 0;
		xn = BG_XN + PRE_BLANK2;
#ifdef BG_ENABLE
		setup();
#endif
		if (++stage >= sizeof(yofs)) {
			stage = LOOP;
			return 1;
		}
	}
	return 0;
}

void bgStart(void) {
#ifdef BG_ENABLE
	memset(BG, 0, BGSIZE);
	bgp = &BG[BGSIZE - SRC_XN];
#endif
	if (xofs >= 7 * (BG_XN + PRE_BLANK2) / 10 && ++stage >= sizeof(yofs)) stage = LOOP;
	xofs = -PRE_BLANK1;
	xn = BG_XN + PRE_BLANK2;
#ifdef BG_ENABLE
	yofsp = (u8 *)yofs + stage;
	setup();
#endif
}
