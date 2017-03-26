#ifndef DEF_PROTOTYPES
#define DEF_PROTOTYPES

#define SAMPLE_RATE 44100
#define FRAME_PER_BUFFER 256
#define PA_SAMPLE_TYPE paFloat32
#define NB_EFFETS 2
#define TMAX 200
#define TAILLE_BUFFER TMAX*2*FRAME_PER_BUFFER

//Structure pour mémoriser les buffers d'entrée
typedef struct Buffer{
  float buffer[TMAX][2*FRAME_PER_BUFFER]; //Acces au buffer i par buffer[premier+i %TMAX][j]
  int premier;
  int dernier;
}Buffer;

Buffer * listBuffer;
typedef enum FX {FUZZ,ECHO,OFF}FX;

#endif
