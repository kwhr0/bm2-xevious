#include "base.h"

u8 keydata;
static u8 frameCount;

void cls(void) {
	memset(GBASE, 0, XN * YN);
}

void printf(const u8 *format, ...) {
	u16 *ap = (u16 *)&format;
	u8 *p = (u8 *)format;
#ifdef __chibicc__
	ap += 2;
#define next_ap	(++ap)
#else
#define next_ap	(--ap)
#endif
	while (*p) {
		u8 c[7];
		u8 a, b, f, i, l, n, r, t, u;
		u16 v;
		u8 *q;
		switch (t = *p++) {
		case '%':
			b = ' ';
			l = n = 0;
			switch (t = *p++) {
				case '%': putchar('%'); continue;
				case '0': b = '0'; t = *p++; break;
				case '-': l = 1; t = *p++; break;
				case 'c': putchar(*next_ap); continue;
			}
			if (t >= '1' && t <= '9')
				for (n = t - '0'; (t = *p++) >= '0' && t <= '9';)
					n = 10 * n + t - '0';
			if (t == 's')
				for (q = *(u8 **)next_ap, i = 0; q[i]; i++)
					;
			else {
				if (u = t == 'u') t = *p++;
				switch (t) {
					case 'd': r = 10; break;
					case 'o': r = 8; u = 1; break;
					case 'x': r = 16; u = 1; a = 0x27; break;
					case 'X': r = 16; u = 1; a = 7; break;
					default: continue;
				}
				v = *next_ap;
				if (f = !u && (s16)v < 0) v = -v;
				c[i = sizeof(c) - 1] = 0;
				do {
					u16 d = v / r, t1 = v - d * r + '0';
					c[--i] = t1 > '9' ? t1 + a : t1;
					v = d;
				} while (v);
				if (f) c[--i] = '-';
				q = &c[i];
			}
			i = sizeof(c) - 1 - i;
			if (l) n -= i;
			else while (n-- > i) putchar(b);
			while (*q) putchar(*q++);
			if (l) while (n--) putchar(' ');
			break;
		default:
			putchar(t);
			break;
		}
	}
#undef next_ap
}

static u32 rndv = 12345;

u16 rand(void) { // xorshift
	rndv = rndv ^ (rndv << 13);
	rndv = rndv ^ (rndv >> 17);
	rndv = rndv ^ (rndv << 5);
	return rndv & 0x7fff;
}

u16 R(u16 n) {
	return (u32)n * rand() >> 15;
}

void waitN(u8 frames) {
	u8 dif = (u8)interruptCount - frameCount;
	u8 f, t = frameCount + frames;
	if (t >= frameCount)
		while ((f = interruptCount) >= frameCount && f < t)
			wait();
	else
		while ((f = interruptCount) >= frameCount || f < t)
			wait();
	frameCount = f;
}

void setkey(void) {
	//                         e  x  s  d  j  k
	static const u8 port[] = { 2, 1, 1, 2, 6, 7 };
	static const u8 mask[] = { 4, 1, 2, 2, 2, 2 };
	u8 i, d = 0;
	for (i = 0; i < 6; i++) {
		_KEY = port[i];
		if (!(_KEY & mask[i])) d |= 0x40;
		d >>= 1;
	}
	keydata = d;
}
