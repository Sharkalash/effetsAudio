#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "prototypes.h"
#include "audio.h"
 
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
  //framesPerBuffer *=1;
  float *out = (float*)outputBuffer; //Buffer de sortie
  float *in = (float*)inputBuffer; //Buffer d'entrée
  //unsigned int i;
  float gain = 11,mix = 1;
  float *copy = (float *)malloc(sizeof(float)*2*framesPerBuffer);

  copie(in,out);


  if(data->effets[2]==WAH){
    copie(out,copy);
    wahwah(copy,out,3000,&(data->wah),&(data->monte));
  }

  if(data->effets[3]==TREMOLO){
    copie(out,copy);
    tremolo(copy,out,0.5,5,&(data->trem));
  }
  
  if(data->effets[0]==FUZZ){
    copie(out,copy);
    fuzz(copy,out,gain,mix);
  }

  if(data->effets[6] == CHORUS)
    {
      copie(out,copy);
      chorus(copy, out, 0.5, data->listBuffer);
    }
  
  if(data->effets[1]==OVERDRIVE){
    copie(out,copy);
    overdrive(copy,out,5);
  }

  if(data->effets[5]==FLANGER){
    copie(out,copy);
    flanger(copy,out,0.7,data->listBuffer,&(data->flange));
  }
  
  if(data->effets[4]==ECHO){
    copie(out,copy);
    echo(copy,out,0.5,1000,data->listBuffer);
  }
  
  push(data->listBuffer,in);
  free(copy);
  return 0;
}

Data initData(){
  Data data;
  int i;
  
  data.trem = 0;
  data.wah = 500;
  data.monte = 1;
  data.listBuffer = creerBuffer();
  data.flange = 0;

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
  // char c = 0;

  pedalier(ecran,data.effets);
  /*
  do{
    c = getc(stdin);
    while(getc(stdin)!='\n');//On vide le buffer


    switch(c){
    case 'f':
      data.effets[0]=data.effets[0]==OFF?FUZZ:OFF;
      break;
    case 'o':
      data.effets[1]=data.effets[1]==OFF?OVERDRIVE:OFF;
      break;
    case 'w':
      data.effets[2]=data.effets[2]==OFF?WAH:OFF;
      break;
    case 't':
      data.effets[3]=data.effets[3]==OFF?TREMOLO:OFF;
      break;
    case 'e':
      data.effets[4]=data.effets[4]==OFF?ECHO:OFF;
      break;
    case 'g':
      data.effets[5]=data.effets[5]==OFF?FLANGER:OFF;
      break;
    case 'c':
      data.effets[6] = (data.effets[6] == OFF)? CHORUS : OFF;
      break;
    }
   
    }while(c!='q');*/
  
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
  SDL_Quit();
  
  return EXIT_SUCCESS;

 error:
  libererBuffer(data.listBuffer);
  Pa_Terminate();
  fprintf(stderr, "ERROR ERROR ERROR\n");
  fprintf( stderr, "Error number: %d\n", err );
  fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
  SDL_FreeSurface(ecran);
  SDL_Quit();
  return err;
}
