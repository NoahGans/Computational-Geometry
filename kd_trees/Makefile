PLATFORM = $(shell uname)


## Compilation flags
##comment out one or the other 
##debugging 
CFLAGS = -g 
##release
#CFLAGS = -O3 -DNDEBUG
LDFLAGS=

CFLAGS+= -Wall

ifeq ($(PLATFORM),Darwin)
## Mac OS X
CFLAGS += -m64 -isystem/usr/local/include  -Wno-deprecated 
LDFLAGS+= -m64 -lc -framework AGL -framework OpenGL -framework GLUT -framework Foundation
else
## Linux
CFLAGS += -m64
INCLUDEPATH  = -I/usr/include/GL/ 
LIBPATH = -L/usr/lib64 -L/usr/X11R6/lib
LDFLAGS+=  -lGL -lglut -lrt -lGLU -lX11 -lm  -lXmu -lXext -lXi
endif


CC = g++ -O3 -Wall $(INCLUDEPATH)


PROGS = mondrian

default: $(PROGS)

mondrian: mondrian.o kdTree.o	
	$(CC) -o $@ mondrian.o kdTree.o $(LDFLAGS)

mondrian.o: mondrian.cpp 
	$(CC) -c $(INCLUDEPATH) $(CFLAGS)   mondrian.cpp  -o $@

kdTree.o: kdTree.cpp kdTree.h
	$(CC) -c $(INCLUDEPATH) $(CFLAGS)   kdTree.cpp  -o $@

clean::	
	rm *.o
	rm mondrian



