#Compiler avec la commande "make"

GTK_FLAGS = $(shell pkg-config --cflags gtk+-2.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-2.0)

CC = gcc
CFLAGS = -O3 -Wall -std=c99 $(GTK_FLAGS)
LDFLAGS = -lm $(GTK_LIBS)
SOURCE = $(wildcard *.c) # tous .c
OBJETS = $(SOURCE:.c=.o)
DEP = $(SOURCE:.c=.d)

all : $(OBJETS)

# @ pour masquer les lignes de commande

%.d: %.c
	$(CC) -MM $*.c -MF $*.d


%.o: %.c %.d #transformer .o en .c
	$(CC) $(CFLAGS) -c $<

clean : #supprimer .o
	rm  -f $(OBJETS)
	rm  -f $(DEP)
