CC = gcc
DEBUG = yes
EXE = audioFX
SRC = $(wildcard *.c)#Génération de la liste des fichiers sources
OBJ = $(SRC:.c=.o)#Génération de la liste des fichiers objets
LIBS = libportaudio.a -lrt -lm -lasound -ljack -pthread #Bibliothèques

ifeq ($(DEBUG), yes) #Condtions
$(info "Mode debug")
CFLAGS = -g -W -Wall -O2
else
$(info "Mode Release")
CFLAGS = -W -Wall -O2
endif


all: $(EXE)
$(EXE) : $(OBJ)
	$(CC) -o $@ $^ $(LIBS)


%.o:%.c git
	$(CC) -c $< $(CFLAGS)
clean :
	rm -f *~* #Fichier temporaire d'emacs
	rm -f *#*
cleanO :
	rm -f *.o
depend:
	makedepend $(SRC)
doc:
	doxygen
push :
	git push https://github.com/Sharkalash/effetsAudio
# DO NOT DELETE

audio.o: /usr/include/stdlib.h /usr/include/features.h
audio.o: /usr/include/stdc-predef.h /usr/include/alloca.h
audio.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
audio.o: /usr/include/wchar.h /usr/include/math.h prototypes.h audio.h
main.o: /usr/include/stdio.h /usr/include/features.h
main.o: /usr/include/stdc-predef.h /usr/include/libio.h
main.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/stdlib.h
main.o: /usr/include/alloca.h /usr/include/math.h prototypes.h audio.h
