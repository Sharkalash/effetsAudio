#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <math.h>
#include "prototypes.h"
#include "audio.h"


static FX effets[NB_EFFETS];



void creerBuffer(){
  listBuffer = malloc(sizeof(Buffer));

  listBuffer->premier = 0;
  listBuffer->dernier = 0;
}

void push(float *in){
  listBuffer->dernier = (listBuffer->dernier+1)%TMAX;
  
  if(listBuffer->premier == listBuffer->dernier){
    listBuffer->premier = (listBuffer->premier+1)%TMAX;
  }

  copie(in,listBuffer->buffer[listBuffer->dernier]);
}

void libererBuffer(){
  free(listBuffer);
}

static int audioFXCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
			  const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
  //Empecher d'avoir des warnings unused variable
  userData = (void *)userData;
  timeInfo = (void *)timeInfo;
  statusFlags = statusFlags;
  //framesPerBuffer *=1;
  float *out = (float*)outputBuffer; //Buffer de sortie
  float *in = (float*)inputBuffer; //Buffer d'entrée
  //unsigned int i;
  float gain = 11,mix = 1;
  float *copy = (float *)malloc(sizeof(float)*2*framesPerBuffer);

  copie(in,out);

  if(effets[2]==WAH){
    copie(out,copy);
    wahwah(copy,out,3000);
  }

  if(effets[3]==TREMOLO){
    copie(out,copy);
    tremolo(copy,out,0.5,5);
  }
  
  if(effets[0]==FUZZ){
    copie(out,copy);
    fuzz(copy,out,gain,mix);
    }

  if(effets[1]==OVERDRIVE){
    copie(out,copy);
    overdrive(copy,out);
  }

  
  push(in);
  free(copy);
  return 0;
}

int main()
{
  PaStream *stream;
  PaError err;
  int i;
  creerBuffer();
  for(i=0;i<NB_EFFETS;i++)
    effets[i] = OFF;
  
  
  err=Pa_Initialize();

  if(err!=paNoError ) goto error;
  
  err = Pa_OpenDefaultStream(&stream,
			     2, //input channels
			     2, //output. 2 pour stereo
			     PA_SAMPLE_TYPE, // 32 bit floating point output
			     SAMPLE_RATE,
			     FRAME_PER_BUFFER,// frames per buffer 
			     audioFXCallback, //Callback function
			     NULL);//Pointer passé au callback


  if (err != paNoError) {
    goto error;
  }

  if((err=Pa_StartStream(stream))!=paNoError) goto error;
  char c = 0;
  do{
    c = getc(stdin);
    while(getc(stdin)!='\n');//On vide le buffer

    switch(c){
    case 'f':
      effets[0]=effets[0]==OFF?FUZZ:OFF;
      break;
    case 'o':
      effets[1]=effets[1]==OFF?OVERDRIVE:OFF;
      break;
    case 'w':
      effets[2]=effets[2]==OFF?WAH:OFF;
      break;
    case 't':
      effets[3]=effets[3]==OFF?TREMOLO:OFF;
      break;
    }
   
  }while(c!='q');
  
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

  libererBuffer();
  return 0;

 error:
  libererBuffer();
  Pa_Terminate();
  fprintf(stderr, "ERROR ERROR ERROR\n");
  fprintf( stderr, "Error number: %d\n", err );
  fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
  return err;
}
