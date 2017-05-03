CC = gcc
DEBUG = yes
EXE = audioFX
SRC = $(wildcard src/*.c)#Génération de la liste des fichiers sources
OBJ = $(SRC:.c=.o)#Génération de la liste des fichiers objets
LIBS = libportaudio.a -lrt -lm -lasound -ljack -pthread -lSDL -lSDL_ttf #Bibliothèques

ifeq ($(DEBUG), yes) #Condtions
$(info "Mode debug")
CFLAGS = -g -W -O2
else
$(info "Mode Release")
CFLAGS = -W -O2
endif


all: $(EXE)
$(EXE) : $(OBJ)
	$(CC) -o $@ $^ $(LIBS)


%.o:%.c git
	$(CC) -c $< $(CFLAGS)
cleanTmp :
	rm -f */*~* #Fichier temporaire d'emacs
	rm -f */*#*
	rm -f *#* *~*

clean : cleanTmp
	rm -f src/*.o

depend:
	makedepend $(SRC)
doc:
	doxygen
push :
	git push https://github.com/Sharkalash/effetsAudio
# DO NOT DELETE

src/audio.o: /usr/include/stdlib.h /usr/include/features.h
src/audio.o: /usr/include/stdc-predef.h /usr/include/alloca.h
src/audio.o: /usr/include/stdio.h /usr/include/libio.h
src/audio.o: /usr/include/_G_config.h /usr/include/wchar.h
src/audio.o: /usr/include/math.h header/prototypes.h header/audio.h
src/main.o: /usr/include/stdio.h /usr/include/features.h
src/main.o: /usr/include/stdc-predef.h /usr/include/libio.h
src/main.o: /usr/include/_G_config.h /usr/include/wchar.h
src/main.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/math.h
src/main.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
src/main.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
src/main.o: /usr/include/SDL/SDL_platform.h /usr/include/string.h
src/main.o: /usr/include/xlocale.h /usr/include/strings.h
src/main.o: /usr/include/inttypes.h /usr/include/stdint.h
src/main.o: /usr/include/ctype.h /usr/include/endian.h /usr/include/iconv.h
src/main.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
src/main.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
src/main.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
src/main.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
src/main.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
src/main.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
src/main.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
src/main.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
src/main.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
src/main.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
src/main.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_ttf.h
src/main.o: /usr/include/SDL/SDL.h header/prototypes.h header/audio.h
src/main.o: header/affichage.h
src/affichage.o: /usr/include/stdio.h /usr/include/features.h
src/affichage.o: /usr/include/stdc-predef.h /usr/include/libio.h
src/affichage.o: /usr/include/_G_config.h /usr/include/wchar.h
src/affichage.o: /usr/include/stdlib.h /usr/include/alloca.h
src/affichage.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
src/affichage.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
src/affichage.o: /usr/include/SDL/SDL_platform.h /usr/include/string.h
src/affichage.o: /usr/include/xlocale.h /usr/include/strings.h
src/affichage.o: /usr/include/inttypes.h /usr/include/stdint.h
src/affichage.o: /usr/include/ctype.h /usr/include/endian.h
src/affichage.o: /usr/include/iconv.h /usr/include/SDL/begin_code.h
src/affichage.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
src/affichage.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
src/affichage.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
src/affichage.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
src/affichage.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
src/affichage.o: /usr/include/SDL/SDL_active.h
src/affichage.o: /usr/include/SDL/SDL_keyboard.h
src/affichage.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
src/affichage.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
src/affichage.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
src/affichage.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
src/affichage.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL.h
src/affichage.o: header/prototypes.h
