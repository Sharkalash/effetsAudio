#ifndef DEF_AUDIO
#define DEF_AUDIO

#define PI 3.14159265359
#define SIGN(X) ((X<0)?-1:1)


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

/**
 *\fn void overdrive(float *in, float *out, int drive)
 *\brief Applique l'effet overdrive au son
 *\brief On appelle la fonction récursivement pour augmenter l'effet
 *\param in le buffer d'entrée
 *\param out le buffer de sortie
 *\param drive Nombre d'appel récursif
 */

void overdrive(float *in, float *out, int drive);

/**
 *\fn void wahwah(float *in, float *out,int fw, float *wah, int * monte)
 *\brief Applique un effet wah-wah automatique au signal
 *\param in Le buffer d'entrée
 *\param out Le buffer de sortie
 *\param fw La fréquence de modulation
 *\param wah La fréquence actuelle
 *\param Booleen precisant la montée ou la descente de la fréquence
 */

void wahwah(float *in, float *out,int fw, float *wah, int * monte);

/**
 *\fn void tremolo(float *in, float *out, float alpha, float fc, int *trem)
 *\brief Applique un effet tremolo au signal
 *\param in Le buffer d'entrée
 *\param out Le buffer de sortie
 *\param alpha Mixage signal d'entrée et modulation. Il est compris entre 0 et 1 sachant que 0 eteint la modulation et 1 donne le maximum de la modulation
 *\param fc La fréquence de modulation
 *\param trem Pour appliquer le tremolo en continu et ne pas recommencer à 0 sur chaque échantillon
 */

void tremolo(float *in, float *out, float alpha, float fc, int *trem);

/**
 *\fn void echo(float *in, float *out, float gain, float retard, Buffer *listBuffer)
 *\brief Applique un effet echo au signal
 *\param in Le buffer d'entrée
 *\param out le buffer de sortie
 *\param gain Niveau de gain
 *\param retard Le retard en ms de l'echo
 *\param listBuffer La liste des buffer en mémoire
 */

void echo(float *in, float *out, float gain, float retard, Buffer *listBuffer);

/**
 *\fn void flanger(float *in, float* out,float amp, Buffer *listBuffer,float max_time_delay,float rate, int *flange)
 *\brief Applique l'effet flanger au signal
 *\param in Le buffer d'entrée
 *\param out Le buffer de sortie
 *\param amp Niveau d'amplification
 *\param listBuffer La liste des buffer en mémoire
 *\param max_time_delay Le delay du retard en secondes compris entre 3ms et 15ms
 *\param rate Pourcentage de flange en Hz
 *\param flange Pour éviter de revenir au début de la sinusoide à chaque nouveau buffer
 */

void flanger(float *in, float* out,float amp, Buffer *listBuffer,float max_time_delay,float rate, int *flange);

/**
 *\fn void chorus (float *in, float *out, float gain, Buffer *listBuffer, int *choeur, int *retard, int change)
 *\brief Applique l'effet chorus au signal
 *\param in Le buffer d'entrée
 *\param out Le buffer de sortie
 *\param gain Niveau de gain
 *\param listBuffer La liste des buffer en mémoire
 *\param choeur Compteur du nombre d'échantillon traités
 *\param retard Le retard de l'effet (calculé dans la fonction)
 *\param change Indicateur du nombre d'échantillons à traiter avant de changer le retard
 */

void chorus (float *in, float *out, float gain, Buffer *listBuffer, int *choeur, int *retard, int change);

/**
 *\fn void vibrato(float *in, float *out, float modfreq, float width, int *vibre)
 *\brief Applique un effet vibrato au signal
 *\param in Le buffer d'entrée
 *\param out Le buffer de sortie
 *\param modfreq La fréquence de modulation en Hz
 *\param width La largeur du retard en ms.
 *\param vibre Compteur pour éviter de revenir au début de la sinusoide à chaque buffer
 */

void vibrato(float *in, float *out, float modfreq, float width, int *vibre);

/**
 *\fn void shelving(float *in, float *out, float gain, float fc,EQ type)
 *\brief Applique un filtre shelving au signal
 *\param in Le buffer d'entrée
 *\param out Le buffer de sortie
 *\param gain Niveau de gain. Un gain positif provoque un boost des fréquences et un gain négatif provoque un cut.
 *\param fc La fréquence centrale
 *\param type Le type d'égalisation. (BASS, TREBLE ou ALL)
 */

void shelving(float *in, float *out, float gain, float fc,EQ type);

/**
 *\fn void filter(float *b, float *a,int nb, int na,float *in, float *out)
 *\brief Effectue un filtrage linéaire selon une fonction de transfert rationelle.
 *\brief \f$ a[0]y[n] = b[0]x[n] + b[1]x[n-1] + ... + b[nb-1]x[n-(nb-1)] - a[1]x[n-1] - ... - a[na-1]x[n-(na-1)] \f$
 *\param b Coefficient du numérateur
 *\param a Coefficient du dénominateur
 *\param nb Taille de b
 *\param na Taille de a
 *\param in Le buffer d'entrée
 *\param out Le buffer de sortie
 */ 

void filter(float *b, float *a,int nb, int na,float *in, float *out);

/**
 *\fn void reverb(float *in, float *out, int n, float gain_allpass_filter, float gain_signal)
 *\brief Applique l'effet reverb au signal
 *\param in Le buffer d'entrée
 *\param out Le buffer de sortie
 *\param gain_allpass_filter Le gain du allpass
 *\param gain_signal Le gain du signal d'entrée
 */

void reverb(float *in, float *out, int n, float gain_allpass_filter, float gain_signal);

#endif 

