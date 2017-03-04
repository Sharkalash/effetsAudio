#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <math.h>
#define SAMPLE_RATE 44100
#define FRAME_PER_BUFFER 256
#define PA_SAMPLE_TYPE paFloat32


typedef struct
{
  float left_phase;
  float right_phase;
}paTestData;

static paTestData data;
int choix=0;

/*
static int patestCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
			  const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
  paTestData *data = (paTestData*)userData;
  float *out = (float*)outputBuffer;
  unsigned int i;
  (void) inputBuffer;

  for(i=0;i<framesPerBuffer;i++)
    {
      *out++ = data->left_phase;
      *out++ = data->right_phase;
      data->left_phase += 0.01f;

      if(data->left_phase >= 1.0f)
	data->left_phase -= 2.0f;
      data->right_phase+=0.03f;
      if(data->right_phase >= 1.0f)
	data->right_phase -= 2.0f;
    }
  
  return 0;
  }*/

/*

static float pas=0.01;
static int incr=0;

static int patestCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
			  const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
  paTestData *data = (paTestData*)userData;
  float *out = (float*)outputBuffer;
  unsigned int i;
  (void) inputBuffer;
  data->right_phase=0;
  for(i=0;i<framesPerBuffer;i++)
    {
      *out++ = data->left_phase;
      *out++ = data->left_phase;
      data->left_phase += pas;
      
      if(data->left_phase >= 1.0f)
	data->left_phase -= 2.0f;
      if(data->left_phase <= -1.0f)
	data->left_phase += 2.0f;

      
      }

  if(incr==1)
    pas+=0.0001;
  else
    pas-=0.0001;

  if(pas==0.005 && !incr || pas ==-0.005 && incr)
    pas = -pas; 
    
  
  if(pas>=0.06f)
    incr=0;
  if(pas<=-0.06f)
    incr=1;

//printf("%f\n",pas);
  return 0;
  }
*/

static int patestCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
			  const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
  paTestData *data = (paTestData*)userData;
  float *out = (float*)outputBuffer;
  float *in = (float*)inputBuffer;
  unsigned int i;
  float t=0.01;
  const unsigned int a=35, b=4,M=20/SAMPLE_RATE;
  
  for(i=0;i<2*framesPerBuffer;i++)
    {
      switch (choix) {
      case 1: {
	*out++ = (in[i]/fabs(in[i]))*(1-exp(a*powf(in[i],2)/fabs(in[i])));
	break;
      }
      default:
	*out++=*in++;
	break;
      }
      
    }
  
  return 0;
  }

int main(int argc, char *argv[])
{
  PaStreamParameters inputParameters, outputParameters;
  PaStream *stream;
  PaError err;

  //Ouvrir un flux audio I/O
  
  err=Pa_Initialize();

  if(err!=paNoError ) goto error;
  /*
  inputParameters.device = Pa_GetDefaultInputDevice(); //Périohériques d'entrée par défault

  if(inputParameters.device == paNoDevice)
    goto error;

  inputParameters.channelCount = 2; //Stereo
  inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  outputParameters.device = Pa_GetDefaultOutputDevice();

  if(outputParameters.device == paNoDevice)
    goto error;

  outputParameters.channelCount = 2; //Stereo
  outputParameters.sampleFormat = PA_SAMPLE_TYPE;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  outputParameters.device = Pa_GetDefaultOutputDevice();*/
  
  err = Pa_OpenDefaultStream(&stream,
			     2, //input channels
			     2, //output. 2 pour stereo
			     PA_SAMPLE_TYPE, // 32 bit floating point output
			     SAMPLE_RATE,
			     FRAME_PER_BUFFER,// frames per buffer 
			     patestCallback, //Callback function
			     &data);//Pointer passé au callback

  /*err = Pa_OpenStream(&stream,
			     &inputParameters, //input channels
			     &outputParameters, //output. 2 pour stereo
			     SAMPLE_RATE,
			     FRAME_PER_BUFFER,// frames per buffer
			     0,
			     patestCallback, //Callback function
			     NULL);//Pointer passé au callback
  */
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
