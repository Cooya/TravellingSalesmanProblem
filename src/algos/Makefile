#Compiler avec la commande "make"

CC = gcc
CFLAGS = -O3 -Wall -std=c99 
LDFLAGS = -lm -lpthread
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
