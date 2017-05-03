#ifndef DEF_PROTOTYPES
#define DEF_PROTOTYPES

#define SAMPLE_RATE 44100
#define FRAME_PER_BUFFER 256
#define PA_SAMPLE_TYPE paFloat32
#define NB_EFFETS 7
#define TMAX 200
#define HAUTEUR 480
#define LARGEUR 640

#define TAILLE_BUFFER TMAX*2*FRAME_PER_BUFFER

typedef enum FX {WAH,TREMOLO,FUZZ,OVERDRIVE,FLANGER,CHORUS,ECHO}FX;
typedef enum ETAT{ON,OFF}ETAT;
typedef enum EQ{BASS,TREBLE,ALL}EQ;

//Structure pour mémoriser les buffers d'entrée
typedef struct Buffer{
  float buffer[TMAX][2*FRAME_PER_BUFFER]; 
  int premier;
  int dernier;
}Buffer;

typedef struct Data{
  /* DATA tremolo */
  int trem;
  float tremolo_fc;
  float tremolo_alpha;

  /* DATA Wah */
  float wah;
  int monte;
  float wah_fw;

  /* DATA flanger */
  int flange;
  float flanger_amp;

  /* DATA chorus */
  float chorus_gain;

  /* DATA echo */
  float echo_gain;
  float echo_retard;

  /* DATA fuzz */
  float fuzz_gain;
  float fuzz_mix;

  /* DATA overdrive */
  int overdrive_drive;

  Buffer * listBuffer;
  ETAT effets[NB_EFFETS];
}Data;




#endif