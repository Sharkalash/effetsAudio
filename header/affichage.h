#ifndef DEF_AFFICHAGE
#define DEF_AFFICHAGE

#define X_INIT 25 //Début sur l'axe horizontale du pédalier
#define Y_INIT 200 //Début sur l'axe vertical du pédalier
#define X_INTERVALLE 128 //Interval x entre les pédales
#define Y_INTERVALLE 80 //Interval y entre les pédales

/**
 *\struc pedale
 *\brief Contient les inforamtions concernant un pédale : sa position et son nom
 */

typedef struct Pedale
{
  char nom[20]; //Nom de la pédale
  SDL_Rect position; //Position de la pédale
  SDL_Surface *texte; //Affichage de la pédale à l'écran
}Pedale;


/**
 *\fn void pedalier(SDL_Surface *ecran,Data *data)
 *\brief Implémentation de l'intéraction et de l'affichage des effets
 *\param ecran Surface sur laquelle on affiche les pédales
 *\param data Données concernant les effets
 */ 
void pedalier(SDL_Surface *ecran,Data *data);

/**
 *\fn void creerPedale(Pedale *p, char *nom, int x, int y)
 *\brief Fonction de création d'une pédale
 *\param p Pointeur sur un pedale
 *\param nom Nom de la pédale
 *\param x Position sur l'axe horizontal en px
 *\param y Position sur l'axe vertical en px
 */

void creerPedale(Pedale *p, char *nom, int x, int y);

/**
 *\fn int selectionParametre(SDL_Surface *ecran, Pedale pedalier[],FX pedale, Data *data)
 *\brief Permet à l'utilisateur d'ajuster les paramètres des effets
 *\param ecran L'écran
 *\param pedalier Tableau des pédales
 *\param pedale La pédale sélectionnée
 *\param data Données concernant les effets
 *\return Booléen. Vrai, on continue l'execution, sinon on quitte le programme
 */

int selectionParametre(SDL_Surface *ecran, Pedale pedalier[],FX pedale, Data *data);

/**
 *\fn int get_pedale(int x, int y, Pedale pedalier[])
 *\brief Permet de connaitre quelle pédale a été sélectionnée par clic de la souris.
 *\param x Position horizontale du curseur
 *\param y Position verticale du curseur
 *\param pedalier Tableau des pédales
 *\return La pédale sélectionnée ou -1 si ce n'est pas une pédale
 */

int get_pedale(int x, int y, Pedale pedalier[]);

/**
 *\fn void set_color(Pedale *p, ETAT etat);
 *\brief Donne une couleur à une pédale en fonction de son état. Vert si elle est allumée, rouge sinon.
 *\param p Pointeur sur la pédale
 *\param etat L'état actuel de la pédale
 */

void set_color(Pedale *p, ETAT etat);

/**
 *\fn void libererPedale(Pedale p[])
 *\brief Libère un tableau de pédale
 *\param p Tableau de pédale
 */

void libererPedale(Pedale p[]);
#endif
