START = 2100
AS = as6800
CC = chibicc
LD = ld6800
LDFLAGS = -b -C0x$(START) -m a.map
LIBS = /opt/chibicc/lib/libc.a

OBJS = crt0.o main.o base.o mathi.o game.o \
	emitter.o es.o sp.o sprite.o pattern.o bg.o chr.o

all: pattern.h $(OBJS)
	$(LD) $(LDFLAGS) -o a.out $(OBJS) $(LIBS)
	dd if=a.out of=a.bin bs=`echo "ibase=16;$(START)"|bc` skip=1 >/dev/null 2>&1
	f9dasm -6800 -offset $(START) a.bin > a.lst
	./mkadr.pl > a.adr
	./total.sh

crt0.o: crt0.asm
	$(AS) -l crt0.lst $<

es.c: essrc mkes.pl
	./mkes.pl $< > $@

pattern.h pattern.s: mkpat.pl sprite.tim
	./mkpat.pl sprite.tim
	
.c.o:
	$(CC) -S $<
	$(AS) -l $(<:.c=.lst) $(<:.c=.s)

clean:
	rm -f es.c pattern.*
	rm -f *.{lst,o,s} a.{adr,bin,map,out}
