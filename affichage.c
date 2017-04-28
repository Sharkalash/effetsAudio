#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string.h>
#include "prototypes.h"
#include "affichage.h"


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
    }
}

void set_etat(Pedale *p, ETAT etat)
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

int get_pedale(int x, int y, Pedale pedalier[])
{
  int i=0;

  int xtmp = x - X_INIT;
  int ytmp = y - Y_INIT;

  if(xtmp<0 || ytmp<0)
    return -1; // Ce n'est pas une pédale
  
  x = x - xtmp%X_INTERVALLE;
  y = y - ytmp%Y_INTERVALLE;

  while((x!=pedalier[i].position.x || y!=pedalier[i].position.y) && ++i<NB_EFFETS);

  if(i>=NB_EFFETS)
    return -1;

  return i;  
}
  
  

void pedalier(SDL_Surface *ecran,ETAT effets[])
{
  int continuer=1;
  SDL_Event event;
  Pedale pedalier[NB_EFFETS];
  int i,pedale,x = X_INIT, y = Y_INIT;

  /*Création des pédales */
  
  creerPedale(pedalier+WAH,"WAH",x,y);
  
  y+=Y_INTERVALLE;
  
  creerPedale(pedalier+TREMOLO,"TREMOLO",x,y);
  
  x+=X_INTERVALLE;
  y=Y_INIT;
  
  creerPedale(pedalier+FUZZ,"FUZZ",x,y);
  
  y+=Y_INTERVALLE;
  
  creerPedale(pedalier+OVERDRIVE,"OVERDRIVE",x,y);
  
  x+=X_INTERVALLE;
  y=Y_INIT;
  
  creerPedale(pedalier+FLANGER,"FLANGER",x,y);
  
  y+=Y_INTERVALLE;
  
  creerPedale(pedalier+CHORUS,"CHORUS",x,y);
  
  x+=X_INTERVALLE;
  y=Y_INIT;
  
  creerPedale(pedalier+ECHO,"ECHO",x,y);
  //creerPedale(pedalier+7,"REVERB",x,y);
  
  while(continuer)
    {
      pedale=-1;
      SDL_WaitEvent(&event);

      switch(event.type)
	{
	case SDL_QUIT:
	  continuer = 0;
	  break;
	case SDL_MOUSEBUTTONDOWN:
	  pedale = get_pedale(event.button.x, event.button.y, pedalier);
	  break;
	  /*case SDL_MOUSEMOTION:
	  pedale = get_pedale(event.motion.x, event.motion.y, pedalier);
	  break;*/
	case SDL_KEYDOWN:
	  switch(event.key.keysym.sym)
	    {
	    case SDLK_q:
	    case SDLK_ESCAPE:
	      continuer = 0;
	      break;
	    case SDLK_w:
	      //effets[0]=effets[0]==OFF?WAH:OFF;
	      pedale=0;
	      break;
	    case SDLK_t:
	      //effets[1]=effets[1]==OFF?TREMOLO:OFF;
	      pedale=1;
	      break;
	    case SDLK_f:
	      //effets[2]=effets[2]==OFF?FUZZ:OFF;
	      pedale=2;
	      break;
	    case SDLK_o:
	      //effets[3]=effets[3]==OFF?OVERDRIVE:OFF;
	      pedale=3;
	      break;
	    case SDLK_g:
	      //effets[4]=effets[4]==OFF?CHORUS:OFF;
	      pedale=4;
	      break;
	    case SDLK_c:
	      // effets[5]=effets[5]==OFF?FLANGER:OFF;
	      pedale=5;
	      break;
	    case SDLK_e:
	      //effets[6] = (effets[6] == OFF)? ECHO : OFF;
	      pedale=6;
	      break;
	    }
	}

      if(pedale!=-1)
	{
	  effets[pedale] = (effets[pedale]==ON)?OFF:ON;
	  set_etat(pedalier+pedale, effets[pedale]);
	}

      SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,0,0,0));
      for (i = 0; i < NB_EFFETS; i++) {
	SDL_BlitSurface(pedalier[i].texte, NULL, ecran, &pedalier[i].position);
      }
      
      SDL_Flip(ecran);
    }

  libererPedale(pedalier);
}
      
	
