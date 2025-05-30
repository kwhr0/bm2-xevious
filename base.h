#include "types.h"

#define putchar(c)		(PRINTER = c)

#define XN				32
#define YN				192

#ifdef __CHIBICC__
#define sleep()	asm(".byte 0x1a")
#define wait()	asm("wai")
#define ei()	asm("cli")
#define di()	asm("sei")
#else
void sleep(void);
void wait(void);
void ei(void);
void di(void);
#endif

extern u8 keydata, interruptCount;

void memset(void *, int, int);
void memcpy(void *, const void *, int);
void cls(void);
void clrs(void);
void printf(const u8 *format, ...);
u16 rand(void);
u16 R(u16 n);
void waitN(u8 frames);
void setkey(void);
