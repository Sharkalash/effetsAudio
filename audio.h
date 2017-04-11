#ifndef DEF_AUDIO
#define DEF_AUDIO

#define PI 3.14159265359
#define SIGN(X) ((X<0)?-1:1)

/**
 *\fn float maxTabAbs(float *tab)
 *\brief Maximum d'un tableau en valeur absolu
 *\param tab Un tableau de flottant
 *\return Le maximum du tableau en valeur absolu
 */
float maxTabAbs(float *tab);

/**
 *\fn void copie(float *src, float* dest)
 *\brief Copie un tableau dans un autre
 *\param src Tableau source
 *\param dest Tableau de destination
 */
void copie(float *src, float* dest);

/**
 *\fn void fuzz(float* in,float *out, float gain, float mix)
 *\brief Applique un effet fuzz au son
 *\param in Buffer d'entree du son
 *\param out buffer de sortie du son
 *\param gain Montant de distortion
 *\param mix Mix du signal distordu et original
 *\warning mix doit être comprix entre 0 et 1
 */
void fuzz(float* in,float *out, float gain, float mix);


void overdrive(float *in, float *out, int drive);

void wahwah(float *in, float *out,int fw, float *wah, int * monte);

void tremolo(float *in, float *out, float alpha, float fc, int *trem);

void echo(float *in, float *out, float gain, float retard, Buffer *listBuffer);
void flanger(float *in, float* out,float amp, Buffer *listBuffer, int *flange);

void chorus (float *in, float *out, float gain, Buffer *listBuffer);
#endif 

