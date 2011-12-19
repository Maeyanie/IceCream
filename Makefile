OBJS=icecream.o jar.o list.o mod.o ui.o web.o

all: icecream

%.o: %.cpp icecream.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

icecream: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS) -lcurl -lyaml-cpp -lzip -lz

clean:
	rm -f $(OBJS)
	rm -f icecream
