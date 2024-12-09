#include "header.h"




//SSPG charger d'initialiser les informations d'une nouvelle partie et de lancer la boucle de jeu
void newGame(t_cityData partie, t_image banqueImages, t_polices banquePolices, t_music banqueSon)
{





    partie.gameMode= selectGameMode(banqueImages, banquePolices);

    initNewMap(partie.plato);

    ///INITIALISATION DES INFOS DE LA PARTIE
    partie.gold=500000;
    partie.elecTot=0;
    partie.elecUsed=0;
    partie.waterTot=0;
    partie.waterUsed=0;
    partie.pop=0;
    partie.month=11;
    partie.year=2022;
    partie.actionJoueur=0;
    partie.ID_cursorRes=0;
    partie.ID_cursorHab=0;
    partie.sec=0;
    partie.minu=0;

    partie.matAdj=NULL;

    //on alloue les tableaux de structures dynamiques
    partie.tabBatRessources = (t_ressources*)malloc(sizeof(t_ressources));

    partie.tabHabitations = (t_habitation*)malloc(sizeof(t_habitation));



    boucleDeJeu(partie, banqueImages, banquePolices, banqueSon);
}


//SSPG chargé de l'affichage de l'écran de sélection du mode de jeu et retourne le choix de mode de jeu du joueur
int selectGameMode(t_image banqueImages, t_polices banquePolices)
{
    BITMAP * buffer2x; //Déclaration du buffer
    buffer2x=create_bitmap(SCREEN_W,SCREEN_H);//creation du bitmap buffer double

    int proceed=0; //témoin pour savoir si l'utilisateur a fini son choix de mode de jeu
    int gameMode=0;

    int colorCap= makecol(24,146,214);
    int colorBlack = makecol(0,0,0);
    int colorWhite = makecol(255,255,255);
    int colorComm = makecol(203,32,38);

    while(!proceed)//tant que le joueur n'as pas confirmé la saisie
    {


    clear_bitmap(buffer2x);//clear du buffer en debut de boucle

    blit(banqueImages.Fond_Menu,buffer2x,0,0,0,0,SCREEN_W,SCREEN_H);//affichage du decor du menu sur le buffer

    ///AFFICHAGE DES CHOIX

    //texte haut
    textout_centre_ex(buffer2x, banquePolices.Kabel_28, "Choisir le mode de jeu", SCREEN_W/2-3, 20+3, colorBlack, -1 );
    textout_centre_ex(buffer2x, banquePolices.Kabel_28, "Choisir le mode de jeu", SCREEN_W/2, 20, colorWhite, -1 );


    //Case Capitalisme
    rectfill(buffer2x, 128-5, 113-5, 384+5, 565+5, colorBlack ); //bordure noire  de la case
    rectfill(buffer2x, 128, 113, 384, 565, colorCap); //rectangle bleu capitaliste

    if(isMouseInCoord(128,384,113,565)) //su on survole la case
    {
        rectfill(buffer2x, 128, 113, 384, 565, makecol(94,186,237)); //rectangle bleu surbrillance
        if(mouse_b&1)
        {
            gameMode=1;
        }
    }
    if(gameMode==1) //si on choisi ce mode
    {
        rectfill(buffer2x, 128, 113, 384, 565, makecol(18,60,83));
    }

    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Capitalisme", 256-2, 113+2, colorBlack, -1 );
    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Capitalisme", 256, 113, colorWhite, -1 );
    masked_blit(banqueImages.iconCap, buffer2x, 0,0 , 128, 211,256,256);


    //Case Communisme
    rectfill(buffer2x, 640-5, 113-5, 896+5, 565+5, colorBlack ); //bordure noire de la case
    rectfill(buffer2x, 640, 113, 896, 565, colorComm); //rectangle rouge communiste
    if(isMouseInCoord(640,896,113,565))
    {
        rectfill(buffer2x, 640, 113, 896, 565, makecol(243,66,72)); //rectangle rouge surbrillance
        if(mouse_b&1)
        {
            gameMode=2;
        }
    }
    if(gameMode==2) //si on choisi ce mode
    {
        rectfill(buffer2x, 640, 113, 896, 565, makecol(98,11,14));
    }
    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Communisme", 768-2, 113+2, colorBlack, -1 );
    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Communisme", 768, 113, colorWhite, -1 );

    masked_stretch_blit(banqueImages.iconCom, buffer2x, 0,0,92,90,640,211,256,256);


    //Case confirmer le choix
    rectfill(buffer2x, 394-5, SCREEN_H-28+5, 630+5, SCREEN_H-92-5, colorBlack); //bordure noire de la case
    rectfill(buffer2x, 394, SCREEN_H-28, 630, SCREEN_H-92, makecol(171, 224, 215)); //rectangle bleu ciel
    if(isMouseInCoord(394,630 , SCREEN_H-92,SCREEN_H-28))
    {
        rectfill(buffer2x, 394, SCREEN_H-28, 630, SCREEN_H-92, makecol(34, 101, 140));
        if(mouse_b&1 && gameMode!=0)
        {
            proceed=1;
        }
    }
    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Suivant", 512-2, SCREEN_H-92+2, colorBlack, -1 );
    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Suivant", 512, SCREEN_H-92, colorWhite, -1 );




    blit(buffer2x, screen, 0,0,0,0, SCREEN_W,SCREEN_H); //on affiche le buffer à l'écran

    }


    return gameMode;
}


//SSPG chargé de remplir les information du tableau de structure qui servira de plateau de jeu
void initNewMap(t_kase plato[35][45])
{
    int tileSize=24; //taille en pixel des cases
    int offset_X = -tileSize/2+SCREEN_W/2+60; //offset de l'affichage par rapport à là ou il est censé etre
    int offset_Y = 50;


    //initialisation du tableau de cases
    for(int i=0; i<35; i++)
    {
        for(int j=0; j<45; j++)
        {
            //on set les coordonées avec une transformation isométrique
            plato[i][j].x1 = 1*i*tileSize/2 + (-1)*j*tileSize/2 +offset_X;
            plato[i][j].y1 = 0.5*i*tileSize/2 + 0.5*j*tileSize/2+ offset_Y;

            //set les coordonées du batiment à afficher
            plato[i][j].building_x= plato[i][j].x1;
            plato[i][j].building_y= plato[i][j].y1 -  (0.5*tileSize/2 + 0.5*tileSize/2);


            //on set le type de tiles à afficher
            plato[i][j].tileToDisplay=0;

            //on set le type de la tile
            plato[i][j].tileType=0;

            //on set le type d'obstacle de la case
            plato[i][j].bat_ID=0;
            plato[i][j].isSelected=0;
            plato[i][j].roadType=0;
            plato[i][j].anchorBat=0;
            plato[i][j].sommet=0;
            plato[i][j].isCoWater=0;
            plato[i][j].isCoElec=0;

            //on set la couleur cachée de detection de la case
            plato[i][j].hiddenColor= makecol(i,j,0);

        }
    }




}
