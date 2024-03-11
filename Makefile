all: world
CXX?=g++
CXXFLAGS?=--std=c++17

INCLUDES:=

OBJS:= \
	objs/main.o

PROPERTY_DIR:=.
include ./Makefile.inc

world: example

$(shell mkdir -p objs)

objs/main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

example: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -L. $(LIBS) $^ -o $@;

.PHONY: clean
clean:
	@rm -rf objs
	@rm -f example
