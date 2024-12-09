#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include <math.h>

#define SCREEN_W 1024 //largeur de la fen�tre allegro
#define SCREEN_H 678  //hauteur de la fen�tre allegro

///--------------------- STRUCTURES -----------------------///


//structure utilis�e pour stocker les informations d'une case du terrain
typedef struct Kase
{
    float x1, y1; //coordon�es d'affichage des cases
    int tileType; //type de la case ( 0- Default | 1 - selected |3- route | 4-Habitation| 5- water tower | 6- centrale)
    int building_x, building_y; //coordonn�es pour afficher les batiments en relief
    int roadType; //orientation de la route si la case est une route
    int bat_ID; //identifiant unique si la case fait partie d'un batiment

    int tileToDisplay; //tile a afficher
    int hiddenColor; //couleur de d�tection
    int isSelected; //si la case est s�lectionn�e
    int anchorBat; //si la case est l'ancre d'affichage d'un batiment
    int sommet; //num�ro du sommet de la case dans le graphe utilis� pour Dijsktra
    int isCoWater; //si la case est reli�e � un chateau d'eau
    int isCoElec;//si la case est reli�e � une centrale

} t_kase;

//structure utilis�e pour stocker dynamiquement les informations d'un batiment de ressource
typedef struct ressources
{
    int ID;//start at 2000
    int resType; //type de ressource ( 1 eau | 2 elec)
    int exportTot; //qte d'export de ressource total
    int tabCoord[24][2];//tableau de toutes les coordon�es du batiment
    int *tabDistances; //tableau stockant l'ID des habitations et la distance la plus courte jusqu'� chacune
    int *tabIDSorted; //tableau stockant les ID des habitations tri� en fonction de leur distance
    int connections; //nb d'habitations connect�es � ce batiment

} t_ressources;

//structure utilis�e pour stocker dynamiquement les informations d'une habitation
typedef struct habitation
{
    int ID;//starts at 1000
    int population; //population totale de l'habitation
    int niveau; //niveau de l'habitation (-1 � 4)
    int tabCoord[9][2]; //tableau de toutes les coordon�es de l'habitation
    int in_eau; //arriv�e d'eau (0-1000 )
    int in_elec; //arriv�e d'�lec (0-1)
    int tickUpdate; //variable pour savoir quand faire �voluer l'habitation
    int evoFlag; //variable pour g�rer l'am�lioration

} t_habitation;



/* Alias d'un Graphe */
typedef struct Graphe
{
    int taille;//Nb d'aretes (a noter que chaque arete est presente 2 fois dans le .txt (0-1/1-0) la taille est donc doubl�e

    int ordre;//Nb de sommets(== au nb de case batiments/route presentes sur le plateau)

} Graphe;



//  Structure utilis�e pour stocker les informations de la, partie
typedef struct cityData
{
    int gold; //argent du joueur
    int pop; // nombre d'habitants
    int waterTot; //Qte d'eau dans la ville
    int waterUsed; //Qte d'eau utilis�e
    int elecTot; //Qte d'�lectricit� dans la ville
    int elecUsed; //Qte d'�lec utilis�e
    int month; //Date en jeu
    int year; //Date en jeu
    int sec; //seconde en temps r�el
    int minu; //minutes en temps r�el
    int gameMode; //1 = capitaliste | 2 = communiste
    t_kase plato[35][45]; //terrain de jeu
    int actionJoueur; //variable stockant l'action actuellement effectu�e par le joueur
    int ID_cursorRes; //curseur indiquant le nombre de batiments de ressources
    int ID_cursorHab; //curseur indiquant le nombre d'habitation

    t_ressources * tabBatRessources; //tableau dynamique stockant les informations des batiments de ressources
    t_habitation * tabHabitations; //tableau dynamique stockant les informations des habitations

    int** matAdj; //matrice d'adjacence des cases du plateau de jeu

    Graphe gGraphe; //graphe utilis� pour stocker la taille et l'ordre

} t_cityData;

//  Structure utilis�e pour stocker les polices utilis�es pour le projet
typedef struct polices
{
    FONT * Kabel_28;
    FONT * Kabel_30;
    FONT * Pixel_11;
    FONT * System_14;

} t_polices;


//  Structure utilis�e pour stocker les images utilis�es pour le projet
typedef struct image
{
    BITMAP * HiddenMap;

    //menu
    BITMAP * Fond_Menu; //fond du menu principal
    BITMAP * iconCom; //icone gammemode communiste
    BITMAP * iconCap; //icone gammemode capitaliste

    //actions : niveaux de visualisation
    BITMAP * visuEau;
    BITMAP * visuElec;
    BITMAP * visuRoute;

    //actions : poser un batiment
    BITMAP * actionCentrale;
    BITMAP * actionWaterTower;
    BITMAP * actionRoute;
    BITMAP * actionHabitation;

    //cases et batiments
    BITMAP * tile_Default;//tile normale
    BITMAP * tile_Selected;//tile selectionee
    BITMAP * tile_Occupied;
    BITMAP * tile_Elec;
    BITMAP * tile_Water;
    BITMAP * Tile_Road_X;
    BITMAP * Tile_Road_Y;
    BITMAP * Tile_Road_Turn_Up;
    BITMAP * Tile_Road_Turn_Down;
    BITMAP * Tile_Road_Turn_Left;
    BITMAP * Tile_Road_Turn_Right;
    BITMAP * Tile_Road_3Way_1;
    BITMAP * Tile_Road_3Way_2;
    BITMAP * Tile_Road_3Way_3;
    BITMAP * Tile_Road_3Way_4;
    BITMAP * Tile_Road_Cross;

    BITMAP * tile_ruine;
    BITMAP * tile_terrainVague;
    BITMAP * tile_cabane;
    BITMAP * tile_maison;
    BITMAP * tile_immeuble;
    BITMAP * tile_gratteCiel;

    BITMAP * tile_centrale;
    BITMAP * tile_waterTower;

    BITMAP * tile_visuCentrale;
    BITMAP * tile_visuWaterTower;

    BITMAP * tabCentrale[4];
    BITMAP * tabWaterTower[4];

    BITMAP * play;
    BITMAP * pause;


} t_image;

//  Structure utilis�e pour stocker la bande son du projet
typedef struct son
{
    SAMPLE * menu;
    SAMPLE * jeu;

} t_music;


///--------------------- SOUS-PROGS -----------------------///

//-------- Syst�me -------//
void initAllegro(); //SSPG charg� d'initialiser les fonctions d'allegro
int isMouseInCoord(int x1, int x2, int y1, int y2); //renvoie 1 si la souris est dans les coordonn�es donn�es
int isClickInCoord(int x1, int x2, int y1, int y2);
t_image loadBanqueImage(); //SSPG charg� de load les diff�rentes images n�cessaires au projet dans une structure
t_polices loadBanquePolices(); //SSPG charg� de load les diff�rentes polices n�cessaires au projet dans une structure
t_music loadBanqueSon();
void isCoBat(int i, int j, int batType, t_kase plato[35][45]);
int kaseFreeInRange( int x, int y, int range_x, int range_y, t_kase plato[35][45]);
void loadSavedGame();
void saveGame(t_cityData partie,int eau,int elec,int eau_used,int elec_used,int pop);

//-------- Menu ----------//
void mainMenu(t_cityData partie, t_image banqueImages, t_polices banquePolices, t_music banqueSon ); //SSPG charg� de l'affichage et du fonctionnement du menu principal
void aide (t_image banqueImages, t_polices banquePolices, t_music banqueSon);

//----- LancerPartie -----//
void newGame(t_cityData partie, t_image banqueImages, t_polices banquePolices, t_music banqueSon); //SSPG charg� d'initialiser une nouvelle partie
int selectGameMode(t_image banqueImages, t_polices banquePolices); //SSPG permettant de choisir son mode de jeu
void initNewMap(t_kase plato[35][45]); //SSPG charg� de l'initialisation du plateau de jeu vierge

//------ BoucleDeJeu -----//
void boucleDeJeu(t_cityData partie, t_image banqueImages, t_polices banquePolices, t_music banqueSon); //SSPG charg� du d�roulement de la boucle de jeu
void affUIplateau(BITMAP * buffer2x, t_cityData partie, t_image banqueImages, t_polices banquePolices, int InMenu, int timerM, int timerS, int nivVisu,int*pt_save, int togglePause); //SSPG charg� de l'affichage de l'interface du plateau
void affMapIso(BITMAP * buffer2x, t_kase plato[35][45], t_image banqueImage, int actionJoueur, int niveauVisu, int current_anim, t_cityData partie);
t_cityData poseBatiment(t_cityData partie, int actionJoueur, t_image banqueImage, int tickPoseBat );

//--------AlgoCases-------//
int detectOrientRoute (int x, int y, t_kase tab[35][45]);
t_cityData updateHab(t_cityData partie, int gameMode, int updateID);
t_cityData gestionRes(t_cityData partie);
int** creermMatrAdj (int**tab, Graphe g);//Creer la matrice d'adjacence a partir du.txt de graphe_brut
Graphe graphe_brut(t_kase plateau[35][45],Graphe g);//Creer dans un txt les aretes du graphe
int isConnected (int**matriceAdj, int x_or,int y_or,int x_dest, int y_dest, Graphe gr,t_kase plateau[35][45]);//Detection bool�enne de connexit�
int longueurRoute (int**matriceAdj, int x_or,int y_or,int x_dest, int y_dest, Graphe gr,t_kase plateau[35][45]); //Detection de distance
int calcSmallestDist( t_cityData partie, int IDbat, int IDhab);
t_cityData triTabDist(t_cityData partie, int IDbat);

#endif // HEADER_H_INCLUDED
