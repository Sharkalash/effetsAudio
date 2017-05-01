/**
 *\file audio.c
 *\brief Fichier gérant l'implémentation des effets
 */

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
      q[i] = gain*in[i]; //Ajout du gain
      z[i] = SIGN(-q[i])*(1-exp(SIGN(-q[i])*q[i])); //Signal distordu
      out[i] = (mix*z[i]) + (1-mix)*in[i]; //Mix du signal, mix = 0 -> son clean, mix = 1 seulement distordu. mix est dans [0,1]
    }
    

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
  int delayLine = (int)2*retard*SAMPLE_RATE/1000;
  int i;

  if(delayLine>TMAX*2*FRAME_PER_BUFFER)
    delayLine = TMAX*2*FRAME_PER_BUFFER;

    
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

    (*flange)%=8192; //Pour eviter un trop grand nombre
    
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


void chorus (float *in, float *out, float gain, Buffer *listBuffer)
{
  const int MIN = 10, MAX = 25;
  int delayLine, i;
  float retard;

  retard = (rand() % (MAX - MIN + 1)) + MIN;
  
  for (i = 0 ; i < FRAME_PER_BUFFER ; i++)
    {
      
      delayLine = (int) retard * SAMPLE_RATE / 1000;
      int r = i - delayLine;
      if ( r >= 0)
	{
	  out[2*i] =  in[2*i] + gain * in[2*r];
	  out[2*i + 1] =  in[2*i + 1] + gain * in[2*r + 1];
	}
      else
	{
	  r = -r;
	  int numBuffer = listBuffer->dernier - (int) (r / (FRAME_PER_BUFFER));
	  if (numBuffer < 0)
	    {
	      numBuffer += TMAX-1;
	    }
 
	  int ind = r % (FRAME_PER_BUFFER);
	  out[2*i] = in[2*i] + gain * listBuffer->buffer[numBuffer][2 * FRAME_PER_BUFFER - 1 - 2*ind];
	  out[2*i+1] = in[2*i+1] + gain * listBuffer->buffer[numBuffer][2 * FRAME_PER_BUFFER - 1 - 2*ind - 1];
	  
	}
    }
}

/*

void filter(float *a, float *b, float *in, float *out)
{
  int i;

  for(i=0;i<FRAME_PER_BUFFER;i++)
    {
    }
      
}

void shelving(float *in, float *out, float gain, float fc, float Q, EQ type)
{
  float k = tan((PI*fc)/SAMPLE_RATE);
  float v0 = powf(10,gain/20);
  float r = 1/Q;
  float a[3], b[3];

  //On inverse le gain si c'est un cut
  if(v0 < 1)
    v0 = 1/v0;

  /* BASS BOOST */ /*

  if(gain > 0 &&  type == BASS)
    {
      float div = (1 + r*k + powf(k,2));
      
      b[0] = (1 + sqrt(v0)*r*k + v0*powf(k,2)) / div;
      b[1] = (2*(v0*powf(k,2) - 1)) / div;
      b[2] = (1 - sqrt(v0)*r*k + v0*powf(k,2)) / div;
      a[1] = (2*(powf(k,2) - 1)) / div;
      a[2] = (1 - r*k + powf(k,2)) / div;
    }
  
  /* BASS CUT */ /*
  
  else if (gain < 0 && type == BASS)
    {
      float div = (1 + r*sqrt(v0)*k + v0*powf(k,2));

      b[0] = (1 + r*k + powf(k,2)) / div;
      b[1] = (2*(powf(k,2) - 1)) / div;
      b[2] = (1 - r*k + powf(k,2)) / div;
      a[1] = (2*(v0*powf(k,2) - 1)) / div;
      a[2] = (1 - r*k*sqrt(v0) + v0*powf(k,2)) / div;
    }

  /* TREBLE BOOST*/ /*

  else if(gain > 0 && type == TREBLE)
    {
      float div = (1 + r*k + powf(k,2));

      b[0] = (V0 + sqrt(v0)*r*k + powf(k,2)) / div;
      b[1] = (2*(powf(k,2) - v0)) / div;
      b[2] = (v0 - sqrt(v0)*r*k + powf(k,2)) / div;
      a[1] = (2*(powf(k,2) - 1)) / div;
      a[2] = (1 - r*k + powf(k,2)) / div;

    }

  /* TREBLE CUT */ /*

  else if(gain < 0 && type == TREBLE)
    {
      float div = (v0 + r*sqrt(v0)*k + powf(k,2));

      b[0] = (1 + r*k + powf(k,2)) / div;
      b[1] = (2*(powf(k,2) - 1)) / div;
      b[2] = (1 - r*k + powf(k,2)) / div;

      div = (1 + r/sqrt(v0)*k + powf(k,2)/v0);
      
      a[1] = (2*(powf(k,2)/v0 - 1)) / div;
      a[2] = (1 - r/sqrt(v0)*k + powf(k,2)/v0) / div;
    }

  /* ALL-PASS */ /*

  else {
    b[0] = v0;
    b[1] = b[2] = a[1] = a[2] = 0;
  }

  a[0] = 1;

  filter(b,a,in,out);

  } */
      
