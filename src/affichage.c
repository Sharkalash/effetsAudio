#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string.h>
#include "../header/prototypes.h"
#include "..//header/affichage.h"


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

void set_color(Pedale *p, ETAT etat)
{
  TTF_Font *police = NULL;
  SDL_Color rouge = {255,0,0}, vert = {0,255,0};
  
  police = TTF_OpenFont("Lato-Heavy.ttf",12);

  //ON : VERT, OFF : ROUGE
  if(etat == OFF)
    p->texte = TTF_RenderText_Blended(police,p->nom,rouge);
  else
    p->texte = TTF_RenderText_Blended(police,p->nom,vert);

  TTF_CloseFont(police);

}

int get_pedale(int x, int y, Pedale pedalier[])
{
  int i=0;

  int xtmp = x - X_INIT; //On enlève les coordonnées initiales
  int ytmp = y - Y_INIT;

  if(xtmp<0 || ytmp<0) //Négatif : ce n'est pas une pédale
    return -1; // Ce n'est pas une pédale
  
  x = x - xtmp%X_INTERVALLE; //Calcul des coordonnées d'une pédale
  y = y - ytmp%Y_INTERVALLE;

  //On cherche si ces coordonnées correspondent à une pédale existante
  while((x!=pedalier[i].position.x || y!=pedalier[i].position.y) && ++i<NB_EFFETS);

  if(i>=NB_EFFETS)
    return -1; //Ce n'est pas une pédale

  return i;  //On renvoie le numéro de la pédale
}
  
int selectionParametre(SDL_Surface *ecran, Pedale pedalier[],FX pedale, Data *data)
{
  TTF_Font *police = NULL;
  SDL_Color blanc = {255,255,255};
  int continuer = 1;
  int e;
  SDL_Event event;

  police = TTF_OpenFont("Lato-Heavy.ttf",12);
  pedalier[pedale].texte = TTF_RenderText_Blended(police,pedalier[pedale].nom,blanc);

  TTF_CloseFont(police);

  SDL_BlitSurface(pedalier[pedale].texte, NULL, ecran, &pedalier[pedale].position);
  SDL_Flip(ecran);

  SDL_EnableKeyRepeat(100,100); //Activation de la répétition des touches

  while(continuer)
    {
      SDL_WaitEvent(&event);

      switch(event.type)
	{
	case SDL_QUIT:
	  return 0;
	  break;
	case SDL_MOUSEBUTTONDOWN:
	  continuer = 0;
	  break;
	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym)
	    {
	    case SDLK_q:
	    case SDLK_ESCAPE:
	      continuer = 0;
	      break;
	    case SDLK_RIGHT:
	    case SDLK_LEFT:
	    case SDLK_UP:
	    case SDLK_DOWN:
	    case SDLK_a:
	    case SDLK_b:
	    case SDLK_t:
	      e = event.key.keysym.sym;

	      switch (pedale)
		{
		case WAH: 
		  data->wah_fw += ((e==SDLK_RIGHT) - (e==SDLK_LEFT && data->wah_fw > 0))*50;
		  break;
		case TREMOLO:
		  data->tremolo_fc += (e==SDLK_RIGHT) - (e==SDLK_LEFT);
		  data->tremolo_alpha += (float)((e==SDLK_UP) - (e==SDLK_DOWN))/10.;
		  break;
		case FUZZ:
		  data->fuzz_gain += (e==SDLK_RIGHT) - (e==SDLK_LEFT);
		  data->fuzz_mix += (float)((e==SDLK_UP && data->fuzz_mix < 1) - (e==SDLK_DOWN && data->fuzz_mix > 0))/10.;
		  break;
		case OVERDRIVE:
		  data->overdrive_drive += (e==SDLK_RIGHT) - (e==SDLK_LEFT && data->overdrive_drive > 0);
		  break;
		case SHELVING:
		  data->shelving_gain += (float) ((e==SDLK_UP) - (e==SDLK_DOWN))/10.;
		  data->shelving_fc += ((e==SDLK_RIGHT) - (e==SDLK_LEFT && data->shelving_fc>0))*50;

		  switch(e)
		    {
		    case SDLK_t:
		      data->shelving_type = TREBLE;
		      break;
		    case SDLK_b:
		      data->shelving_type = BASS;
		      break;
		    case SDLK_a:
		      data->shelving_type = ALL;
		      break;
		    }
		  
		case VIBRATO:
		  data->vibrato_modfreq += (e==SDLK_UP) - (e==SDLK_DOWN && data->vibrato_modfreq > 0);
		  data->vibrato_width += ((e==SDLK_RIGHT) - (e==SDLK_LEFT && data->vibrato_width > 0))*10;		    
		case FLANGER:
		  data->flanger_rate += (float)((e==SDLK_UP) - (e==SDLK_DOWN && data->flanger_rate > 0))/10.;
		  data->flanger_max_time_delay +=  (float)((e==SDLK_RIGHT && data->flanger_max_time_delay < 0.015) - (e==SDLK_LEFT && data->flanger_max_time_delay > 0.003))/1000.;
		  break;
		case CHORUS:
		  data->chorus_gain += (float) ((e==SDLK_UP) - (e==SDLK_DOWN))/10.;
		  data->chorus_change += (e==SDLK_RIGHT) - (e==SDLK_LEFT && data->chorus_change > 1);
		  break;
		case ECHO:
		  data->echo_retard += ((e==SDLK_RIGHT) - (e==SDLK_LEFT && data->echo_retard>50))*50;
		  data->echo_gain += (float)((e==SDLK_UP) - (e==SDLK_DOWN))/10.;
		  break;
		}
	      
	      break;
	    }
	}
    }

  SDL_EnableKeyRepeat(0,0); //Désactivation de la répétition des touches
  set_color(pedalier+pedale, data->effets[pedale]);
  return 1;
}

void pedalier(SDL_Surface *ecran,Data *data)
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

  creerPedale(pedalier+SHELVING,"SHELVING",x,y);
  
  x+=X_INTERVALLE;
  y=Y_INIT;

  creerPedale(pedalier+VIBRATO,"VIBRATO",x,y);

  y+=Y_INTERVALLE;
  
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

	  if(event.button.button == SDL_BUTTON_RIGHT && pedale != -1)
	    {
	      continuer = selectionParametre(ecran,pedalier,pedale,data);
	      pedale = -1;
	    }
	  
	  break;
	case SDL_KEYDOWN: //Sélection clavier
	  switch(event.key.keysym.sym)
	    {
	    case SDLK_q:
	    case SDLK_ESCAPE:
	      continuer = 0; //On quitte le programme
	      break;
	    case SDLK_w:
	      pedale = WAH;
	      break;
	    case SDLK_t:
	      pedale = TREMOLO;
	      break;
	    case SDLK_f:
	      pedale = FUZZ;
	      break;
	    case SDLK_o:
	      pedale = OVERDRIVE;
	      break;
	    case SDLK_s:
	      pedale = SHELVING;
	      break;
	    case SDLK_v:
	      pedale = VIBRATO;
	      break;
	    case SDLK_g:
	      pedale = FLANGER;
	      break;
	    case SDLK_c:
	      pedale = CHORUS;
	      break;
	    case SDLK_e:
	      pedale = ECHO;
	      break;
	    }
	}

      if(pedale!=-1)
	{
	  data->effets[pedale] = (data->effets[pedale]==ON)?OFF:ON; //Activation ou désactivation de l'effet
	  set_color(pedalier+pedale, data->effets[pedale]); //On met à jour la couleur de la pédale
	}

      SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,0,0,0)); //Fond de la fenetre
      
      //Collage de chaque pédale à l'écran
      for (i = 0; i < NB_EFFETS; i++) {
	SDL_BlitSurface(pedalier[i].texte, NULL, ecran, &pedalier[i].position);
      }
      
      SDL_Flip(ecran); //Actualisation de l'écran
    }

  libererPedale(pedalier); //On libère toutes les pédales
}
      
	
