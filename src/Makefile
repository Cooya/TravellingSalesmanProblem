#Compiler avec la commande "make"

GTK_FLAGS = $(shell pkg-config --cflags gtk+-2.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-2.0)

CC = gcc
CFLAGS = -O3 -Wall -std=c99 #$(GTK_FLAGS)
LDFLAGS = -lm $(GTK_LIBS) -lpthread
SOURCE = $(wildcard *.c) # tous .c
OBJETS = $(SOURCE:.c=.o)
DEP = $(SOURCE:.c=.d)
EXEC = VDC

SUBSOURCE = $(wildcard gui/*.c algos/*.c)
SUBOBJ = $(SUBSOURCE:.c=.o)

all : $(EXEC)

# @ pour masquer les lignes de commande

VDC : $(OBJETS)
	make -C gui
	make -C algos
	$(CC) -o bin/$@ $(OBJETS) $(SUBOBJ) $(LDFLAGS)


%.d: %.c
	$(CC) -MM $*.c -MF $*.d

%.o: %.c %.d #transformer .o en .c
	$(CC) $(CFLAGS) -c $<

clean : #supprimer .o
	make -C gui clean
	make -C algos clean
	rm  -f $(OBJETS)
	rm  -f $(DEP)

cleanAll : clean # effacer exec
	rm -rf bin/VDC

doc :
	doxygen doxy_config