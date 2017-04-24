#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string.h>
#include "prototypes.h"

typedef struct Pedale
{
  char nom[20];
  SDL_Rect position;
  SDL_Surface *texte;
}Pedale;


void creerPedale(Pedale *p, char *nom, int x, int y){
  TTF_Font *police = NULL;
  SDL_Color rouge = {255,0,0};
  
  police = TTF_OpenFont("Lato-Heavy.ttf", 12);

  strcpy(p->nom, nom);

  p->position.x = x;
  p->position.y = y;

  p->texte = TTF_RenderText_Blended(police,nom,rouge);

  TTF_CloseFont(police);
}

void libererPedale(Pedale p[])
{
  unsigned int i;

  for(i=0;i<NB_EFFETS;i++)
    {
      SDL_FreeSurface(p[i].texte);
      //free(p[i]);
    }
}

void set_etat(Pedale *p, FX etat)
{
  TTF_Font *police = NULL;
  SDL_Color rouge = {255,0,0}, vert = {0,255,0};
  
  police = TTF_OpenFont("Lato-Heavy.ttf",12);

  if(etat == OFF)
    p->texte = TTF_RenderText_Blended(police,p->nom,rouge);
  else
    p->texte = TTF_RenderText_Blended(police,p->nom,vert);

  TTF_CloseFont(police);

}

void pedalier(SDL_Surface *ecran, FX effets[])
{
  TTF_Font *police = NULL;
  int continuer=1;
  SDL_Event event;
  Pedale pedalier[NB_EFFETS];
  int i,j;
  

  creerPedale(pedalier+2,"WAH",25,200);
  creerPedale(pedalier+3,"TREMOLO",25,280);
  creerPedale(pedalier,"FUZZ",153,200);
  creerPedale(pedalier+1,"OVERDRIVE",153,280);
  creerPedale(pedalier+5,"FLANGER",281,200);
  creerPedale(pedalier+6,"CHORUS",281,280);
  creerPedale(pedalier+4,"ECHO",409,200);
  //creerPedale(pedalier+7,"REVERB",409,280);
  
  while(continuer)
    {
      j=-1;
      SDL_WaitEvent(&event);

      switch(event.type)
	{
	case SDL_QUIT:
	  continuer = 0;
	  break;
	case SDL_KEYDOWN:
	  switch(event.key.keysym.sym)
	    {
	    case SDLK_q:
	    case SDLK_ESCAPE:
	      continuer = 0;
	      break;
	    case SDLK_f:
	      effets[0]=effets[0]==OFF?FUZZ:OFF;
	      j=0;
	      break;
	    case SDLK_o:
	      effets[1]=effets[1]==OFF?OVERDRIVE:OFF;
	      j=1;
	      break;
	    case SDLK_w:
	      effets[2]=effets[2]==OFF?WAH:OFF;
	      j=2;
	      break;
	    case SDLK_t:
	      effets[3]=effets[3]==OFF?TREMOLO:OFF;
	      j=3;
	      break;
	    case SDLK_e:
	      effets[4]=effets[4]==OFF?ECHO:OFF;
	      j=4;
	      break;
	    case SDLK_g:
	      effets[5]=effets[5]==OFF?FLANGER:OFF;
	      j=5;
	      break;
	    case SDLK_c:
	      effets[6] = (effets[6] == OFF)? CHORUS : OFF;
	      j=6;
	      break;
	    }
	}

      if(j!=-1)
	set_etat(pedalier+j,effets[j]);

      SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,0,0,0));
      for (i = 0; i < NB_EFFETS; i++) {
	SDL_BlitSurface(pedalier[i].texte, NULL, ecran, &pedalier[i].position);
      }
      
      SDL_Flip(ecran);
    }

  libererPedale(pedalier);
}
      
	
