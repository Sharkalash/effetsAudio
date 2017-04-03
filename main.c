#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <math.h>
#include "prototypes.h"
#include "audio.h"
 

static FX effets[NB_EFFETS];


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


  if(effets[2]==WAH){
    copie(out,copy);
    wahwah(copy,out,3000,&(data->wah),&(data->monte));
  }

  if(effets[3]==TREMOLO){
    copie(out,copy);
    tremolo(copy,out,0.5,5,&(data->trem));
  }
  
  if(effets[0]==FUZZ){
    copie(out,copy);
    fuzz(copy,out,gain,mix);
  }

  if(effets[1]==OVERDRIVE){
    copie(out,copy);
    overdrive(copy,out);
  }
  
  if(effets[4]==ECHO){
    copie(out,copy);
    echo(copy,out,0.5,200,data->listBuffer);
  }
  
  push(data->listBuffer,in);
  free(copy);
  return 0;
}

Data initData(){
  Data data;
  data.trem = 0;
  data.wah = 500;
  data.monte = 1;
  data.listBuffer = creerBuffer();

  return data;
}

int main()
{
  PaStream *stream;
  PaError err;
  Data data = initData(); //Initialisation des données

  int i;
  
  /* Initilisation des effets à OFF */
  
  for(i=0;i<NB_EFFETS;i++)
    effets[i] = OFF;
  
  
  creerBuffer();
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
    case 'e':
      effets[4]=effets[4]==OFF?ECHO:OFF;
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

  libererBuffer(data.listBuffer);
  return 0;

 error:
  libererBuffer(data.listBuffer);
  Pa_Terminate();
  fprintf(stderr, "ERROR ERROR ERROR\n");
  fprintf( stderr, "Error number: %d\n", err );
  fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
  return err;
}
