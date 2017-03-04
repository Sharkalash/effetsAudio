#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <math.h>
#define SAMPLE_RATE 44100
#define FRAME_PER_BUFFER 1024
#define PA_SAMPLE_TYPE paFloat32

int choix;


static int audioFXCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
			  const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
  //Empecher d'avoir des warnings unused variable
  userData = (void *)userData;
  timeInfo = (void *)timeInfo;
  statusFlags = statusFlags;
  
  float *out = (float*)outputBuffer; //Buffer de sortie
  float *in = (float*)inputBuffer; //Buffer d'entrée
  unsigned int i;
  //const unsigned int a=35, b=4,M=20/SAMPLE_RATE;
  
  for(i=0;i<2*framesPerBuffer;i++)
    {
      switch (choix) {
	/*case 1: {
	*out++ = (in[i]/fabs(in[i]))*(1-exp(a*powf(in[i],2)/fabs(in[i])));
	break;
	}*/
      default:
	*out++=*in++;
	break;
      }
      
    }
  
  return 0;
  }

int main()
{
  PaStream *stream;
  PaError err;


  
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
  char c;
  do{
    
    fflush(stdout);
    c = getchar();
    printf("%c\n", choix);
    if(c=='f'){
      choix=1;
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
  
  return 0;

 error:
  Pa_Terminate();
  fprintf(stderr, "ERROR ERROR ERROR\n");
  fprintf( stderr, "Error number: %d\n", err );
  fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
  return err;
}
