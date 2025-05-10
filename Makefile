#CC = chibicc
CC = fcc
MPU = 6800
#MPU = 6303

START = 2100
AS = as6800
LD = ld6800
LDFLAGS = -b -C0x$(START) -m a.map

ifeq ($(CC),chibicc)
OBJS = crt0.o
LIBS = /opt/chibicc/lib/libc.a
else
CFLAGS = -m$(MPU) -DBG_ENABLE
OBJS = crt0-f.o memset-f.o bgdata.o
ifneq ($(MPU),6800)
OBJS += cleanup.o
endif
LIBS = /opt/fcc/lib/$(MPU)/lib$(MPU).a
endif

OBJS += main.o base.o mathi.o game.o \
	emitter.o es.o sp.o sprite.o pattern.o bg.o chr.o

.SUFFIXES: .asm

all: pattern.h $(OBJS)
	$(LD) $(LDFLAGS) -o a.out $(OBJS) $(LIBS)
	dd if=a.out of=a.bin bs=`echo "ibase=16;$(START)"|bc` skip=1 >/dev/null 2>&1
	f9dasm -$(MPU) -offset $(START) a.bin > a.lst
	./mkadr.pl > a.adr
	./total.sh

.asm.o:
	$(AS) -l $(<:.asm=.lst) $<

bgdata.s: mkbg.pl map.bmp
	./mkbg.pl map.bmp

es.c: essrc mkes.pl
	./mkes.pl $< > $@

pattern.h pattern.s: mkpat.pl sprite.tim
	./mkpat.pl sprite.tim
	
.c.o:
	$(CC) $(CFLAGS) -S $<
	$(AS) -l $(<:.c=.lst) $(<:.c=.s)

clean:
	rm -f bgconf.h es.c pattern.*
	rm -f *.{lst,o,s} a.{adr,bin,map,out}
