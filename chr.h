#include "types.h"

struct Sprite;

void chrLocate(u8 x, u8 y);
struct Sprite *chrPut(u8 c);
void chrPuts(u8 *str);
void chrPutsBuf(u8 *str);
void chrTime(u8 time);
void chrValue(u8 *buf, u8 len, u16 v);
void chrInit(void);
void chrUpdate(void);
void chrFlush(void);
