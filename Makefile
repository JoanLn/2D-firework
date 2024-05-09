## Compiler
CC=g++
## Linker
LD=$(CC)
## Flags
CPPFLAGS = -Wall -g -DLINUX
LFLAGS = -lglut -L/usr/lib -L/usr/X11R6/lib -lXi -lXmu -lGL -lGLU -lm -lGLEW -lglfw

TARGETS = $(PROGFILES:.cpp=)

PROGFILES = \
        main.cpp \
        $(NULL)

targets default: $(TARGETS)

$(PROGFILES:.cpp=): main.o 
	$(CC) -o firework main.o ${LFLAGS}

depend :
	makedepend ${PROGFILES}
# DO NOT DELETE
