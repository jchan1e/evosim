#  MinGW
#ifeq "$(OS)" "Windows_NT"
#SFLG= -O3 -Wall
#LIBS=-lglut32cu -lglu32 -lopengl32
#CLEAN=del *.exe *.o *.a
#else
#  OSX
ifeq ("$(shell uname)","Darwin")
SFLG=-Wno-deprecated-declarations $(shell sdl2-config --cflags)
GLIBS=-framework OpenGL $(shell sdl2-config --libs)
CLEAN=rm -rf sim vis Test *.o *.a *.dSYM
#  Linux/Unix/Solaris
else
SFLG=$(shell sdl2-config --cflags) -DGL_GLEXT_PROTOTYPES
GLIBS=-lGLU -lGL -lm $(shell sdl2-config --libs)
CLEAN=rm -rf sim vis Test *.o *.a
endif
#endif
CFLG=-g -O3 -Wall -std=c++11
#CFLG=-g -Wall -std=c++11

compile:sim vis

#all:sim vis test


sim.o:sim.cpp world.h
	g++ -c $(CFLG) $<

#test.o:test.cpp map.h net.h genome.h ant.h
#	g++ -c $(CFLG) $<

objects.o:objects.cpp stdGL.h
	g++ -c $(CFLG) $(SFLG) $<

shader.o:shader.cpp shader.h
	g++ -c $(CFLG) $(SFLG) $<

vis.o:vis.cpp objects.h stdGL.h world.h
	g++ -c $(CFLG) $(SFLG) $<

#genome.o:genome.cpp genome.h net.h
#	g++ -c $(CFLG) $<

text.o:text.cpp text.h stdGL.h
	g++ -c $(CFLG) $<


agent.o:agent.cpp agent.h world.h
	g++ -c $(CFLG) $<

world.o:world.cpp world.h agent.h
	g++ -c $(CFLG) $<

#  link
sim:sim.o agent.o world.o
	g++ -g -o sim $^

vis:vis.o agent.o world.o objects.o
	g++ -g -o vis $^ $(GLIBS)

#Test:test.o sim vis
#	g++ -g -O3 -o Test $^ $(GLIBS)
#
#test:FORCE Test
#	./Test

FORCE:


#  Clean
clean:
	$(CLEAN)
