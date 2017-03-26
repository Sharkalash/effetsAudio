#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "prototypes.h"
#include "audio.h"


float maxTabAbs(float *tab)
{
  float max=fabs(tab[0]);
  unsigned int i;

  for(i=0;i<2*FRAME_PER_BUFFER;i++)
    {
      if(max>fabs(tab[i]))
	max = fabs(tab[i]);
    }

  return max;
}

void copie(float *src, float* dest)
{
  unsigned int i;
  for(i=0;i<2*FRAME_PER_BUFFER;i++){
    dest[i] = src[i];
  }
}

void fuzz(float* in,float *out, float gain, float mix)
{
  float *q = malloc(sizeof(float)*2*FRAME_PER_BUFFER);
  float *z = malloc(sizeof(float)*2*FRAME_PER_BUFFER);
  //float maxIN = maxTabAbs(in); Normalisation ne marche pas
  unsigned int i;
  
  copie(in,q);
  
  //Distortion du signal
  for(i=0;i<2*FRAME_PER_BUFFER;i++)
    {
      q[i] = gain*in[i]; //Normalisation et ajout du gain
      z[i] = SIGN(-q[i])*(1-exp(SIGN(-q[i])*q[i])); //Signal distordu
    }
      


  //Mix du signal, mix = 0 -> son clean, mix = 1 seulement distordu. mix est dans [0,1]
  for(i=0;i<2*FRAME_PER_BUFFER;i++)
    out[i] = (mix*z[i]) + (1-mix)*in[i];



  //Normalisation
  /*for(i=0;i<FRAME_PER_BUFFER;i++)
    out[i] = out[i]*maxIN/maxOUT;*/

  free(q);
  free(z);

}


void echo(float *in, float *out, float gain, float retard){
  const int delayLine = (int)retard*SAMPLE_RATE/1000;
  int i;
  //int indEcho =delayLine/TMAX;
  //float gain1 = 0,gain2 = 1;

  if(delayLine>TMAX*2*FRAME_PER_BUFFER)
    exit(0);

    
  for(i=0;i<2*FRAME_PER_BUFFER;i++){
    int r=i-delayLine;
    if(r>=0){
      out[i] =  in[i] + gain*in[r];
    }
    else{
      r=-r;
      int numBuffer = listBuffer->dernier - (int)(r/(2*FRAME_PER_BUFFER));
      if(numBuffer<0)
	numBuffer += TMAX-1;
      //int lr = (i%2)==0 && r%(2*FRAME_PER_BUFFER)
      int ind = r%(2*FRAME_PER_BUFFER);
      out[i] =in[i]+ gain*listBuffer->buffer[numBuffer][2*FRAME_PER_BUFFER - ind -1];
      
    }
  }
}

