CC = gcc
DEBUG = yes
EXE = salut
SRC = $(wildcard *.c)#Génération de la liste des fichiers sources
INC = $(wildcard *.h)#Génération de la liste des fichiers includes
OBJ = $(SRC:.c=.o)#Génération de la liste des fichiers objets
LIBS = #Pour les bibliothèques

ifeq ($(DEBUG), yes) #Condtions
$(info "Mode debug")
CFLAGS = -g -W -Wall -O2
else
$(info "Mode Release")
CFLAGS = -W -Wall -O2
endif


all: $(EXE)
$(EXE) : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


%.o:%.c #Tous les fichiers .o: Tous les fichiers .c correspondant cependant pas de dépendances
	$(CC) -c $< $(CFLAGS)
clean :
	rm -f *.o
	rm *~* #Fichier temporaire d'emacs
depend:
	makedepend $(SRC)
doc:
	doxygen
