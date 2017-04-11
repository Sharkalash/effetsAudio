#ifndef DEF_PROTOTYPES
#define DEF_PROTOTYPES

#define SAMPLE_RATE 44100
#define FRAME_PER_BUFFER 256
#define PA_SAMPLE_TYPE paFloat32
#define NB_EFFETS 6
#define TMAX 200

#define TAILLE_BUFFER TMAX*2*FRAME_PER_BUFFER

//Structure pour mémoriser les buffers d'entrée
typedef struct Buffer{
  float buffer[TMAX][2*FRAME_PER_BUFFER]; 
  int premier;
  int dernier;
}Buffer;

typedef struct Data{
  /* DATA tremolo */
  int trem;

  /* DATA Wah */
  float wah;
  int monte;

  /* DATA flanger */
  int flange;

  Buffer * listBuffer;
}Data;


typedef enum FX {FUZZ,OVERDRIVE,WAH,TREMOLO,ECHO,FLANGER,OFF}FX;

#endif
