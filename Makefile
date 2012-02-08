OBJS:=icecream.o jar.o list.o mod.o web.o
VERSION:=$(shell ./version.sh)

CXXFLAGS+=-DVERSION=\"$(VERSION)\"

all: icecream icecream-curses

%.o: %.cpp icecream.h mod.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

icecream: $(OBJS) ui_simple.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS) ui_simple.o -lcurl -lyaml-cpp -lzip -lz

icecream-curses: $(OBJS) ui_curses.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS) ui_curses.o -lcurl -lyaml-cpp -lzip -lz -lpdcurses

clean:
	rm -f $(OBJS) ui_simple.o ui_curses.o
	rm -f icecream icecream-curses
