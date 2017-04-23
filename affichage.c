#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "prototypes.h"

void pedalier(SDL_Surface *ecran, FX effets[])
{
  int continuer=1;
  SDL_Event event;

  while(continuer)
    {
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
	      break;
	    case SDLK_o:
	      effets[1]=effets[1]==OFF?OVERDRIVE:OFF;
	      break;
	    case SDLK_w:
	      effets[2]=effets[2]==OFF?WAH:OFF;
	      break;
	    case SDLK_t:
	      effets[3]=effets[3]==OFF?TREMOLO:OFF;
	      break;
	    case SDLK_e:
	      effets[4]=effets[4]==OFF?ECHO:OFF;
	      break;
	    case SDLK_g:
	      effets[5]=effets[5]==OFF?FLANGER:OFF;
	      break;
	    case SDLK_c:
	      effets[6] = (effets[6] == OFF)? CHORUS : OFF;
	      break;
	    }
	}
    }
}
      
	
