# Makefile for Sim

CXX = g++ -Wno-deprecated -Wl,--export-dynamic
#-Wunused-function -Wall

#CFLAGS = -O2 `pkg-config --cflags gtk+-3.0`
#CFLAGS = -O2 -pthread -DGSEAL_ENABLE -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/gtk-3.0

CXXFLAGS = -O2 `pkg-config --cflags gtk+-3.0`

#CXXFLAGS = -pthread -I/usr/include/gtk-3.0 -I/usr/include/pango-1.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/libpng12  

# LDFLAGS = -lhpdf -lsqlite3 -pthread -lgtk-3 -lgdk-3 -latk-1.0 -lcairo-gobject -lgio-2.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0

LDFLAGS = -lsqlite3 `pkg-config --libs gtk+-3.0`

OBJ = main.o Molecule.o Position.o BrownianMotion.o Translation.o

%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

sim: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm *.o sim
