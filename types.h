#ifndef _TYPES_H_
#define _TYPES_H_

#define sleep()	asm(".byte 0x1a")
#define wait()	asm("wai")
#define ei()	asm("cli")
#define di()	asm("sei")

#ifndef nil
#define nil	0
#endif

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

#define KEY				keydata
#define KEY_UP			1
#define KEY_DOWN		2
#define KEY_LEFT		4
#define KEY_RIGHT		8
#define KEY_A			16
#define KEY_B			32

#define GBASE	((u8 *)0x900)
#define PRINTER	(*(u8 *)0xe800)
#define _KEY	(*(u8 *)0xeec0)
#define RRT	(*(u8 *)0xefd0)
#define DISP	(*(u8 *)0xefe0)

#endif
