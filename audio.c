#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "prototypes.h"
#include "audio.h"


float maxTabAbs(float *tab)
{
  float max=fabs(tab[0]);
  unsigned int i;

  for(i=1;i<2*FRAME_PER_BUFFER;i++)
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

void overdrive(float *in, float *out, int drive)
{
  if(drive >= 1)
    {
      float th = 1./3.;
      unsigned int i;  
  
      for(i=0;i<2*FRAME_PER_BUFFER;i++)
	{
	  if (fabs(in[i]) < th)
	    {
	      out[i] = 2 * in[i];
	    }
	  else if (fabs(in[i]) >= th && fabs(in[i]) < 2 * th)
	    {
	      out[i] = SIGN(in[i])*(3 - powf((2 - 3 * fabs(in[i])), 2)) / 3.;
	    }
	  else
	    {
	      out[i] = SIGN(in[i]);
	    }

	}
      copie(out,in);
      overdrive(in,out,drive-1);
    }

}

/*float wah = 500;
  int monte = 1;*/

void wahwah(float *in, float *out,int fw, float *wah, int * monte)
{
  float *outH = malloc(sizeof(float)*2*FRAME_PER_BUFFER),*outL = malloc(sizeof(float)*2*FRAME_PER_BUFFER), *fc = malloc(sizeof(float)*2*FRAME_PER_BUFFER); //Highpass et Lowpass
  int i=0;
  float damp = 0.05,f1,q1;
  int maxf=3000, minf = 500;

  float delta = fw/(float)(SAMPLE_RATE);
 
  while(i<2*FRAME_PER_BUFFER)
    {
      
      while(i<2*FRAME_PER_BUFFER && (*wah)<=maxf && *monte){
	//printf("%f\n", (*wah));
	fc[i++] = (*wah);
	(*wah)+=delta;
      }

      if(*wah>maxf)
	*monte = !(*monte);
      
      while(i<2*FRAME_PER_BUFFER && *wah>=minf && !(*monte)){
	fc[i++] = (*wah);
	(*wah)-=delta;
      }

      if((*wah)<minf)
	*monte = !(*monte);
      
    }

  f1 = 2*sin((PI*fc[0])/SAMPLE_RATE);

  q1 = 2*damp;

  outH[0] = in[0];
  out[0] = f1*outH[0];
  outL[0] = f1*out[0];

  for(i=1;i<2*FRAME_PER_BUFFER;i++)
    {
      outH[i] = in[i] - outL[i-1] - q1*out[i-1];
      out[i] = f1*outH[i] + out[i-1];
      outL[i] = f1*out[i] + outL[i-1];
      f1 = 2*sin((PI*fc[i])/SAMPLE_RATE);
    }
}

//int trem = 0;
//const int tremMax = 8192;

void tremolo(float *in, float *out, float alpha, float fc, int *trem)
{
  int i;

  for(i=0;i<2*FRAME_PER_BUFFER;i++)
    out[i] = in[i] * (1 + (alpha*sin(2*PI*((*trem)++)*(fc/(float)(SAMPLE_RATE)))));


  (*trem)%=8192;
}


void echo(float *in, float *out, float gain, float retard, Buffer *listBuffer){
  const int delayLine = (int)2*retard*SAMPLE_RATE/1000;
  int i;

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
 
      int ind = r%(2*FRAME_PER_BUFFER);
      out[i] =in[i]+ gain*listBuffer->buffer[numBuffer][2*FRAME_PER_BUFFER - ind -1];
      
    }
  }
}

void flanger(float *in, float* out,float amp, Buffer *listBuffer, int *flange)
{
  float max_time_delay=0.003;
  int rate = 1; //Pourcentage de flange en Hz
  unsigned int i;
  int max_sample_delay = round(max_time_delay*SAMPLE_RATE);
 

  for(i=0;i<FRAME_PER_BUFFER;i++){
    int delay =  ceil(fabs(sin(2*PI*((*flange)++)*((float)(rate)/(float)(SAMPLE_RATE)))*max_sample_delay));

    //printf("%f\n", (float)(rate)/(float)(SAMPLE_RATE));
    
    int r = i-delay;

    if(r>=0){
      out[2*i] = amp*(in[2*i] + in[2*r]);
      out[2*i+1] = amp*(in[2*i+1] + in[2*r+1]);
    }
    else{
      r=-r;
      int numBuffer = listBuffer->dernier - (int)(r/(2*FRAME_PER_BUFFER));
      
      if(numBuffer<0)
	numBuffer += TMAX-1;
 
      int ind = r%(FRAME_PER_BUFFER);
      out[2*i] = amp*(in[2*i] + listBuffer->buffer[numBuffer][2*FRAME_PER_BUFFER - 2*ind -2]);
      out[2*i+1] = amp*(in[2*i+1] + listBuffer->buffer[numBuffer][2*FRAME_PER_BUFFER - 2*ind -1]);
      
    }
  }
  
}
    
