#ifndef DEF_AFFICHAGE
#define DEF_AFFICHAGE

#define X_INIT 25
#define Y_INIT 200
#define X_INTERVALLE 128
#define Y_INTERVALLE 80

typedef struct Pedale
{
  char nom[20];
  SDL_Rect position;
  SDL_Surface *texte;
}Pedale;


void pedalier(SDL_Surface *ecran,Data *data);
#endif
