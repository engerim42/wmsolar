CC     = g++
CFLAGS = -O2 -Wall
INCDIR = -I/usr/X11R6/include/X11 -I/usr/X11R6/include
DESTDIR= /usr/X11R6
LIBDIR = -L/usr/X11R6/lib
# for linux
LIBS   = -lXpm -lX11 -lXext -lm
# for Solaris
# LIBS   = -lXpm -lX11 -lXext -lsocket
OBJS   = wmSolar.o \
         xutils.o \
		AstroOps.o \
		DateOps.o \
		DateOpsIntl.o \
		PlanetData.o \
		Lunar.o \
		Pluto.o \
		MathOps.o \
		RiseSet.o \
		Vsop.o
.c.o:
	$(CC) $(CFLAGS) -D$(shell echo `uname -s`) -c $< -o $*.o $(INCDIR)


all:	wmSolar.o wmSolar

wmSolar.o: wmSolar_master.xpm wmSolar_mask.xbm
wmSolar:	$(OBJS) 
	$(CC) $(CFLAGS) $(SYSTEM) -o wmSolar $^ $(INCDIR) $(LIBDIR) $(LIBS)

clean:
	for i in $(OBJS) ; do \
		rm -f $$i; \
	done
	rm -f wmSolar

install:: wmSolar
	install -c -s -m 0755 wmSolar $(DESTDIR)/bin

