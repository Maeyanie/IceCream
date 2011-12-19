OBJS=icecream.o jar.o list.o mod.o ui.o web.o

all: icecream

%.o: %.cpp icecream.h
	$(CXX) -c -g -Wall -o $@ $<

icecream: $(OBJS)
	$(CXX) -g -Wall -o $@ $(OBJS) -lcurl -lyaml-cpp -lzip

clean:
	rm -f $(OBJS)
	rm -f icecream
