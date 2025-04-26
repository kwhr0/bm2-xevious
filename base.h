#include "types.h"

#define putchar(c)		(PRINTER = c)

#define XN				32
#define YN				192

extern u8 keydata, interruptCount;

void memset(void *, int, int);
void cls(void);
void clrs(void);
void printf(const u8 *format, ...);
u16 rand(void);
u16 R(u16 n);
void waitN(u8 frames);
void setkey(void);
