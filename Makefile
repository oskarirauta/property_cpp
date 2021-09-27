all: test

CXX?=g++
CXXFLAGS?=--std=c++17

INCLUDES:=-I./include -I.

MAIN_OBJS:= \
	objs/main.o

LIBS:=

objs/main.o: example/main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

test: $(SHARED_OBJS) $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@;

clean:
	rm -f objs/*.o test
