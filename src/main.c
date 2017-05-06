/**
 *\file main.c
 *\brief Projet effet audio - Fichier principal
 *\brief L'objectif est de reproduire les effets audios souvent utilisés dans la musique.
 *\author David BALDASSIN
 *\author Sylvain BOURGEA
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <math.h>
#include <SDL/SDL.h>
#include <time.h>
#include <SDL/SDL_ttf.h>
#include "../header/prototypes.h"
#include "../header/audio.h"
#include "../header/affichage.h"
 
Buffer * creerBuffer(){
  Buffer * listBuffer = malloc(sizeof(Buffer));

  listBuffer->premier = 0;
  listBuffer->dernier = 0;

  return listBuffer;
}

void push(Buffer *listBuffer, float *in){
  listBuffer->dernier = (listBuffer->dernier+1)%TMAX;
  
  if(listBuffer->premier == listBuffer->dernier){
    listBuffer->premier = (listBuffer->premier+1)%TMAX;
  }

  copie(in,listBuffer->buffer[listBuffer->dernier]);
}

void libererBuffer(Buffer *listBuffer){
  free(listBuffer);
}

static int audioFXCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
			  const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
  //Empecher d'avoir des warnings unused variable
  Data * data = (Data *)userData;
  timeInfo = (void *)timeInfo;
  statusFlags = statusFlags;
  int i;
  float *out = (float*)outputBuffer; //Buffer de sortie
  float *in = (float*)inputBuffer; //Buffer d'entrée
  float *copy = (float *)malloc(sizeof(float)*2*framesPerBuffer); //On copie le buffer après chaque effet pour pouvoir les combiner 

  for(i=0;i<FRAME_PER_BUFFER;i++)
    in[2*i +1] = in[2*i]; //On copie pour avoir le stéréo, car le micro  est en mono
  
  copie(in,out);


  if(data->effets[WAH]==ON){
    copie(out,copy);
    wahwah(copy,out,data->wah_fw,&(data->wah),&(data->monte));
  }

  if(data->effets[TREMOLO]==ON){
    copie(out,copy);
    tremolo(copy,out,data->tremolo_alpha,data->tremolo_fc,&(data->trem));
  }
  
  if(data->effets[FUZZ]==ON){
    copie(out,copy);
    fuzz(copy,out,data->fuzz_gain,data->fuzz_mix);
  }

  if(data->effets[OVERDRIVE]==ON){
    copie(out,copy);
    overdrive(copy,out,data->overdrive_drive);
  }

  if(data->effets[VIBRATO]==ON){
    copie(out,copy);
    vibrato(copy,out,data->vibrato_modfreq,data->vibrato_width, &data->vibre);
  }

  if(data->effets[FLANGER]==ON){
    copie(out,copy);
    flanger(copy,out,data->flanger_amp,data->listBuffer,data->flanger_max_time_delay, data->flanger_rate,&(data->flange));
  }

  if(data->effets[CHORUS] == ON)
    {
      copie(out,copy);
      chorus(copy, out, data->chorus_gain, data->listBuffer, &(data->choeur), &(data->chorus_retard), data->chorus_change);
    }
  
  if(data->effets[ECHO]==ON){
    copie(out,copy);
    echo(copy,out,data->echo_gain,data->echo_retard,data->listBuffer);
  }
  
  push(data->listBuffer,out);
  free(copy);
  return 0;
}

Data initData(){
  Data data;
  int i;
  
  data.trem = 0;
  data.tremolo_fc = 5;
  data.tremolo_alpha = 0.5;
  data.wah = 500;
  data.monte = 1;
  data.wah_fw = 3000;
  data.listBuffer = creerBuffer();
  data.flange = 0;
  data.flanger_amp = 0.5;
  data.flanger_rate = 1;
  data.flanger_max_time_delay = 0.003;
  data.chorus_gain = 0.5;
  data.chorus_change = 8;
  data.choeur = 0;
  data.echo_retard = 200;
  data.echo_gain = 0.5;
  data.fuzz_gain = 11;
  data.fuzz_mix = 0.5;
  data.overdrive_drive = 5;
  data.vibrato_modfreq = 5;
  data.vibrato_width = 200;
  data.vibre = 0;

  /* Initilisation des effets à OFF */
  
  for(i=0;i<NB_EFFETS;i++)
    data.effets[i] = OFF;

  return data;
}

int main()
{
  SDL_Surface *ecran = NULL;
  PaStream *stream;
  PaError err;
  Data data = initData(); //Initialisation des données

  creerBuffer();

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  srand(time(NULL));
  
  ecran = SDL_SetVideoMode(LARGEUR,HAUTEUR, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  
  err=Pa_Initialize();

  if(err!=paNoError ) goto error;
  
  err = Pa_OpenDefaultStream(&stream,
			     2, //input channels
			     2, //output. 2 pour stereo
			     PA_SAMPLE_TYPE, // 32 bit floating point output
			     SAMPLE_RATE, //Fréquence d'échantillonage
			     FRAME_PER_BUFFER,// frames per buffer 
			     audioFXCallback, //Callback function
			     &data);//Data passées au callback


  if (err != paNoError) {
    goto error;
  }

  if((err=Pa_StartStream(stream))!=paNoError) goto error;
  
  pedalier(ecran,&data);

  err = Pa_StopStream(stream);
  if(err!=paNoError)
    goto error;

  err = Pa_CloseStream(stream);
  if(err!=paNoError)
    goto error;
  
  err = Pa_Terminate();
  if (err != paNoError) {
    printf("PortAudio error : %s\n", Pa_GetErrorText(err));
  }

  libererBuffer(data.listBuffer);

  SDL_FreeSurface(ecran);
  TTF_Quit();
  SDL_Quit();
  
  return EXIT_SUCCESS;

 error:
  libererBuffer(data.listBuffer);
  Pa_Terminate();
  fprintf(stderr, "ERROR ERROR ERROR\n");
  fprintf( stderr, "Error number: %d\n", err );
  fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
  SDL_FreeSurface(ecran);
  TTF_Quit();
  SDL_Quit();
  return err;
}
