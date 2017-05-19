#ifndef DEF_PROTOTYPES
#define DEF_PROTOTYPES

#define SAMPLE_RATE 44100 //Fréquence d'échantillonage
#define FRAME_PER_BUFFER 256 //Taille des buffers. x2 Pour stéréo
#define PA_SAMPLE_TYPE paFloat32
#define NB_EFFETS 10 //Nombre des effets implémentés
#define TMAX 200 //Nombre de buffer conservés en mémoire
#define HAUTEUR 480 //Hauteur de la fenetre
#define LARGEUR 640 //Largeur de la fenetre

#define TAILLE_BUFFER TMAX*2*FRAME_PER_BUFFER //Taille totale du type Buffer

/**
 *\enum FX
 *\brief Enumération des effets implémentés
 */

typedef enum FX {WAH,TREMOLO,FUZZ,OVERDRIVE,SHELVING,VIBRATO,FLANGER,CHORUS,ECHO,REVERB}FX;

typedef enum ETAT{ON,OFF}ETAT;

typedef enum EQ{BASS,TREBLE,ALL}EQ;


/**
 *\struct Buffer
 *\brief Structure de données permettant de mémoriser les buffer afin de pouvoir les réutiliser
 *\brief On mémorise TMAX buffer. Chaque buffer est enfilé à la suite. On accède à l'indice du premier avec premier et à l'indice du dernier avec dernier.
 */

typedef struct Buffer{
  float buffer[TMAX][2*FRAME_PER_BUFFER]; 
  int premier;
  int dernier;
}Buffer;

/**
 *\struct Data
 *\brief Paramètres des effets.
 */

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
  float flanger_rate;
  float flanger_max_time_delay;

  /* DATA chorus */
  float chorus_gain;
  int choeur;
  int chorus_retard;
  int chorus_change;

  /* DATA echo */
  float echo_gain;
  float echo_retard;

  /* DATA fuzz */
  float fuzz_gain;
  float fuzz_mix;

  /* DATA overdrive */
  int overdrive_drive;

  /* DATA vibrato */
  float vibrato_modfreq;
  float vibrato_width;
  int vibre;

  /* DATA shelving */
  float shelving_gain;
  float shelving_fc;
  float shelving_Q;
  EQ shelving_type;

  /* DATA Reverb */
  int reverb_nbAllpass;
  float reverb_gainAllpass;
  float reverb_gain;  

  Buffer * listBuffer;
  ETAT effets[NB_EFFETS];
}Data;

/**
 *\fn Buffer * creerBuffer()
 *\brief Creer et initialise une file de buffer afin de les mémoriser.
 *\return La file de buffer.
 */

Buffer * creerBuffer();

/**
 *\fn void push(Buffer *listBuffer, float *in)
 *\brief Enfile un buffer
 *\param listBuffer La file des buffers
 *\param in Le buffer à ajouter
 */

void push(Buffer *listBuffer, float *in);

/**
 *\fn void libererBuffer(Buffer *listBuffer)
 *\brief Libère la file des buffer
 *\param listBuffer La file des buffer
 */ 

void libererBuffer(Buffer *listBuffer);

/**
 *\fn Data initData()
 *\brief Initialise toutes les données des effets
 *\return La structure avec toutes les données initialisées
 */

Data initData();

#endif
