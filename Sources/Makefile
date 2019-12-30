PROGRAM   = match
CC        = gcc
CFLAGS    = -g -Wall
LDFLAGS   = -lGL -lGLU -lglut

$(PROGRAM): main.o coordinates.o draw.o lighting.o
	$(CC) -o $(PROGRAM) main.o coordinates.o draw.o lighting.o $(LDFLAGS) -lm

coordinates.o: coordinates.c
	$(CC) -c $(CFLAGS) -o coordinates.o coordinates.c $(LIBS)
lighting.o: lighting.c
	$(CC) -c $(CFLAGS) -o lighting.o lighting.c $(LIBS)
draw.o: draw.c
	$(CC) -c $(CFLAGS) -o draw.o draw.c $(LIBS)
.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
