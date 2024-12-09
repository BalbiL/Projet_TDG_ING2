#include "header.h"


//SSPG chargé de l'initialisation des fonctions relatives à allegro
void initAllegro()
{

    srand(time(NULL)); //on initialise la fonction random (utile pour plus tard)
    allegro_init();    //initialisation allegro
    install_keyboard(); //on permet l'utilisation du clavier
    install_mouse();  //on permet l'utilisation de la souris
    set_color_depth(desktop_color_depth());    //initialisation de la palette de couleur


    //on initialise les fonctions de son
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0) != 0) {
        printf("Error initialising sound: %s\n", allegro_error);
        exit(1);
    }

    if((set_gfx_mode(GFX_AUTODETECT_WINDOWED,SCREEN_W,SCREEN_H,0,0))!=0)      //paramètres fenêtre
    {
        //problème avec fenêtre allegro
        allegro_message("Pb de mode graphique");
        allegro_exit();                                             //fermeture allegro
        exit(EXIT_FAILURE);                                         //sortie forcée du programme
    }

    show_mouse(screen); //on affiche la souris à l'écran




}


//SSPG chargé de déterminer si la souris se trouve dans les coordonnées données (1 oui | 0 non)
int isMouseInCoord(int x1, int x2, int y1, int y2)
{
    return (mouse_x>x1 && mouse_x<x2  && mouse_y>y1 && mouse_y<y2);
}

//SSPG chargé de déterminer si le click gauche de la souris se trouve dans les coordonnées données (1 oui | 0 non)
int isClickInCoord(int x1, int x2, int y1, int y2)
{
    return (isMouseInCoord(x1,x2,y1,y2)&&mouse_b&1);
}


/*
//SSPG chargé de déterminer si la case route envoyée par coordonnées i/j
est connecté à un batiment du type batType (1 chateau d'eau | 2 centrale)
*/
void isCoBat(int i, int j, int batType, t_kase plato[35][45])
{

    switch(batType)
    {
    case 1 : //water tower
        if(i<35)
        {
            if(plato[i+1][j].tileType==5 || plato[i+1][j].isCoWater) plato[i][j].isCoWater=1;
        }


        if(j<35)
        {
           if(plato[i][j+1].tileType==5 || plato[i][j+1].isCoWater)  plato[i][j].isCoWater=1;
        }


        if(i>0)
        {
           if(plato[i-1][j].tileType==5 || plato[i-1][j].isCoWater) plato[i][j].isCoWater=1;
        }


        if(j>0)
        {
           if(plato[i][j-1].tileType==5 || plato[i][j-1].isCoWater) plato[i][j].isCoWater=1;
        }

        break;

    case 2: //centrale
        if(i<35)
        {
           if(plato[i+1][j].tileType==6 || plato[i+1][j].isCoElec) plato[i][j].isCoElec=1;
        }


        if(j<35)
        {
            if(plato[i][j+1].tileType==6 || plato[i][j+1].isCoElec)  plato[i][j].isCoElec=1;
        }


        if(i>0)
        {
            if(plato[i-1][j].tileType==6 || plato[i-1][j].isCoElec)  plato[i][j].isCoElec=1;
        }


        if(j>0)
        {
            if(plato[i][j-1].tileType==6 || plato[i][j-1].isCoElec) plato[i][j].isCoElec=1;
        }


        break;

    }

}





//SSPG chargé de détecter si toutes les cases du plateau comprises entre les coordonnées données sont libre, c-à-d type=0 (1 oui | 0 non)
int kaseFreeInRange( int x, int y, int range_x, int range_y, t_kase plato[35][45])
{
    int flag=1;

    for(int i=0; i<range_x; i++)
    {
        for(int j=0; j<range_y; j++)
        {
            if(plato[x+i][y+j].tileType!=0 || x+range_x>35 || y+range_y>45)
            {
                flag=0;
            }
        }
    }
    return flag;
}

//SSPG chargé de charger les différents sons utilisés dans le projet
t_music loadBanqueSon()
{
    t_music banqueSon;

    banqueSon.jeu= load_wav("son/boucleJeu.wav");
    banqueSon.menu = load_wav("son/menu.wav");

    return banqueSon;
}

//SSPG chargé de charger les différentes images utilisées dans le projet
t_image loadBanqueImage()
{
    t_image banqueImage; //on déclare la structure qui permet de stocker les images

    //On charge les images dans la structure depuis le dossier du projet codeblocks

    banqueImage.Fond_Menu = load_bitmap("images/fondMenu2.bmp", NULL);
    banqueImage.iconCom = load_bitmap("images/iconCom.bmp", NULL);
    banqueImage.iconCap = load_bitmap("images/iconCap.bmp", NULL);
    banqueImage.visuRoute = load_bitmap("images/interface/VisuRoute.bmp", NULL);

    banqueImage.visuEau = load_bitmap("images/interface/VisuEau.bmp", NULL);
    banqueImage.visuElec = load_bitmap("images/interface/VisuElec.bmp", NULL);


    banqueImage.HiddenMap =load_bitmap("images/hiddenMap.bmp", NULL);



    banqueImage.actionCentrale= load_bitmap("images/interface/actionCentrale.bmp", NULL);
    banqueImage.actionWaterTower= load_bitmap("images/interface/actionWaterTower.bmp", NULL);
    banqueImage.actionRoute= load_bitmap("images/interface/actionroute.bmp", NULL);
    banqueImage.actionHabitation = load_bitmap("images/interface/actionHabitation.bmp", NULL);

    banqueImage.tile_Default =load_bitmap("images/Tiles/Tile.bmp", NULL);
    banqueImage.tile_Selected = load_bitmap("images/Tiles/TileSelected.bmp", NULL);
    banqueImage.tile_Occupied = load_bitmap("images/Tiles/TileOccupied.bmp", NULL);

    banqueImage.tile_Elec= load_bitmap("images/Tiles/TileElectricity.bmp", NULL);
    banqueImage.tile_Water = load_bitmap("images/Tiles/TileWater.bmp", NULL);



    banqueImage.Tile_Road_X= load_bitmap("images/Tiles/TileRoad/TileRoadX.bmp", NULL);
    banqueImage.Tile_Road_Y= load_bitmap("images/Tiles/TileRoad/TileRoadY.bmp", NULL);
    banqueImage.Tile_Road_Turn_Up= load_bitmap("images/Tiles/TileRoad/TileRoadTurnUp.bmp", NULL);
    banqueImage.Tile_Road_Turn_Down= load_bitmap("images/Tiles/TileRoad/TileRoadTurnDown.bmp", NULL);
    banqueImage.Tile_Road_Turn_Right= load_bitmap("images/Tiles/TileRoad/TileRoadTurnRight.bmp", NULL);
    banqueImage.Tile_Road_Turn_Left= load_bitmap("images/Tiles/TileRoad/TileRoadTurnLeft.bmp", NULL);
    banqueImage.Tile_Road_3Way_1= load_bitmap("images/Tiles/TileRoad/TileRoad3Way1.bmp", NULL);
    banqueImage.Tile_Road_3Way_2= load_bitmap("images/Tiles/TileRoad/TileRoad3Way2.bmp", NULL);
    banqueImage.Tile_Road_3Way_3= load_bitmap("images/Tiles/TileRoad/TileRoad3Way3.bmp", NULL);
    banqueImage.Tile_Road_3Way_4= load_bitmap("images/Tiles/TileRoad/TileRoad3Way4.bmp", NULL);
    banqueImage.Tile_Road_Cross= load_bitmap("images/Tiles/TileRoad/TileRoadCross.bmp", NULL);


    banqueImage.tile_ruine =load_bitmap("images/Tiles/batiment/ruines.bmp", NULL);
    banqueImage.tile_terrainVague =load_bitmap("images/Tiles/batiment/terrainVague.bmp", NULL);
    banqueImage.tile_cabane =load_bitmap("images/Tiles/batiment/cabane.bmp", NULL);
    banqueImage.tile_maison =load_bitmap("images/Tiles/batiment/maison.bmp", NULL);
    banqueImage.tile_immeuble =load_bitmap("images/Tiles/batiment/immeuble.bmp", NULL);
    banqueImage.tile_gratteCiel =load_bitmap("images/Tiles/batiment/gratteCiel.bmp", NULL);

    banqueImage.tile_centrale =load_bitmap("images/Tiles/batiment/Centrale.bmp", NULL);
    banqueImage.tile_waterTower =load_bitmap("images/Tiles/batiment/WaterTower.bmp", NULL);

    banqueImage.tile_visuCentrale= load_bitmap("images/Tiles/centraleVisu.bmp", NULL);
    banqueImage.tile_visuWaterTower= load_bitmap("images/Tiles/WaterTowerVisu.bmp", NULL);

    banqueImage.tabCentrale[0] = load_bitmap("images/Tiles/batiment/centrale/centrale_1.bmp", NULL);
    banqueImage.tabCentrale[1] = load_bitmap("images/Tiles/batiment/centrale/centrale_2.bmp", NULL);
    banqueImage.tabCentrale[2] = load_bitmap("images/Tiles/batiment/centrale/centrale_3.bmp", NULL);
    banqueImage.tabCentrale[3] = load_bitmap("images/Tiles/batiment/centrale/centrale_4.bmp", NULL);

    banqueImage.tabWaterTower[0] = load_bitmap("images/Tiles/batiment/waterTower/Water_1.bmp", NULL);
    banqueImage.tabWaterTower[1] = load_bitmap("images/Tiles/batiment/waterTower/Water_2.bmp", NULL);
    banqueImage.tabWaterTower[2] = load_bitmap("images/Tiles/batiment/waterTower/Water_3.bmp", NULL);
    banqueImage.tabWaterTower[3] = load_bitmap("images/Tiles/batiment/waterTower/Water_4.bmp", NULL);

    banqueImage.play = load_bitmap("images/interface/play.bmp", NULL);

    banqueImage.pause = load_bitmap("images/interface/pause.bmp", NULL);


    return banqueImage; //on renvoie la structure remplie
}

//SSPG chargé de charger les différentes polices de texte utilisées dans le projet
t_polices loadBanquePolices()
{
    t_polices banquePolices; //on déclare la structure qui permet de stocker les polices

    //On charge les polices dans la structure depuis le dossier du projet codeblocks

    banquePolices.Kabel_28= load_font("polices/kabelMono_28.pcx", NULL, NULL);
    banquePolices.Kabel_30= load_font("polices/kabelMono_30.pcx", NULL, NULL);
    banquePolices.Pixel_11 = load_font("polices/PPixel_11.pcx", NULL, NULL);
    banquePolices.System_14 = load_font("polices/system_14.pcx", NULL, NULL);

    return banquePolices; //on renvoie la structure remplie
}

//SSPG chargé de sauvegarder l'intégralité des informations de la partie en cour dans un fichier texte
void saveGame(t_cityData partie,int eau,int elec,int eau_used,int elec_used,int pop)
{

    int i,j,k;
    //Création du fichier
    FILE* inputFile = NULL;

    //Ouverture du fichier
    inputFile=fopen("sauvegarde.txt","w+");

    //Blindage de l'ouverture du fichier de sauvegarde
    if(inputFile==NULL)
    {
        printf("La sauvegarde a échoué");
        exit(0);
    }



    ///Sauvegarde des données de la partie (t_city_data)


    fprintf(inputFile,"%d\n",partie.gold);
    fprintf(inputFile,"%d\n",pop);

    fprintf(inputFile,"%d\n",eau);//Otot
    fprintf(inputFile,"%d\n",eau_used);//Oused
    fprintf(inputFile,"%d\n",elec);//Etot
    fprintf(inputFile,"%d\n",elec_used);//Eused


    fprintf(inputFile,"%d\n",partie.year);
    fprintf(inputFile,"%d\n",partie.month);
    fprintf(inputFile,"%d\n",partie.sec);
    fprintf(inputFile,"%d\n",partie.minu);
    fprintf(inputFile,"%d\n",partie.gameMode);
    fprintf(inputFile,"%d\n",partie.ID_cursorHab);
    fprintf(inputFile,"%d\n",partie.ID_cursorRes);
    fprintf(inputFile,"%d\n",partie.actionJoueur);




    ///-Sauvegarde des données du graphe(Graphe)

    fprintf(inputFile,"%d\n",partie.gGraphe.ordre);
    fprintf(inputFile,"%d\n",partie.gGraphe.taille);



    ///Sauvagarde des données de batiments(t_ressources/t_habitation)



    //Habitations



    for ( k=0;k<partie.ID_cursorHab;k++)
    {

        for(i=0; i<9; i++)
        {
            for (j=0; j<2; j++)
            {
                fprintf(inputFile,"%d ",partie.tabHabitations[k].tabCoord[i][j]);
            }

            fprintf(inputFile,"\n");
        }
     }



    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fprintf(inputFile,"%d\n",partie.tabHabitations[i].ID);
    }

    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fprintf(inputFile,"%d\n",partie.tabHabitations[i].population);
    }

    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fprintf(inputFile,"%d\n",partie.tabHabitations[i].niveau);
    }

    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fprintf(inputFile,"%d\n",partie.tabHabitations[i].in_eau);
    }

    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fprintf(inputFile,"%d\n",partie.tabHabitations[i].in_elec);
    }

    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fprintf(inputFile,"%d\n",partie.tabHabitations[i].tickUpdate);
    }

    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fprintf(inputFile,"%d\n",partie.tabHabitations[i].evoFlag);
    }





    //Ressources

    for ( k=0;k<partie.ID_cursorRes;k++)
    {

        for(i=0; i<24; i++)
        {
            for (j=0; j<2; j++)
            {
                fprintf(inputFile,"%d ",partie.tabBatRessources[k].tabCoord[i][j]);
            }

            fprintf(inputFile,"\n");
        }
     }


    for(i=0; i<partie.ID_cursorRes; i++)
    {
        fprintf(inputFile,"%d\n",partie.tabBatRessources[i].connections);
    }


    for(int i=0; i<partie.ID_cursorRes; i++)
    {

        for(int j=0; j< partie.tabBatRessources[i].connections; j++)
        {
            fprintf(inputFile,"%d\n ",partie.tabBatRessources[i].tabIDSorted[j]);
        }


    }


    for(int i=0; i<partie.ID_cursorRes; i++)
    {

        for(int j=0; j< partie.tabBatRessources[i].connections; j++)
        {
            fprintf(inputFile,"%d\n ",partie.tabBatRessources[i].tabDistances[j]);
        }


    }



    for(i=0; i<partie.ID_cursorRes; i++)
    {
        fprintf(inputFile,"%d\n",partie.tabBatRessources[i].ID);
    }

     for(i=0; i<partie.ID_cursorRes; i++)
    {
        fprintf(inputFile,"%d\n",partie.tabBatRessources[i].resType);

    }

    for(i=0; i<partie.ID_cursorRes; i++)
    {
        fprintf(inputFile,"%d\n",partie.tabBatRessources[i].exportTot);
    }





    ///-Sauvegarde des données du plateau(t_kase)

    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fprintf(inputFile,"%d ",partie.plato[i][j].tileType);
        }

        fprintf(inputFile,"\n");
    }

    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fprintf(inputFile,"%d ",partie.plato[i][j].roadType);
        }

        fprintf(inputFile,"\n");
    }


    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fprintf(inputFile,"%d ",partie.plato[i][j].bat_ID);
        }

        fprintf(inputFile,"\n");
    }






    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fprintf(inputFile,"%d ",partie.plato[i][j].tileToDisplay);
        }

        fprintf(inputFile,"\n");
    }



    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fprintf(inputFile,"%d ",partie.plato[i][j].anchorBat);
        }

        fprintf(inputFile,"\n");
    }



    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fprintf(inputFile,"%d ",partie.plato[i][j].sommet);
        }

        fprintf(inputFile,"\n");
    }



    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fprintf(inputFile,"%d ",partie.plato[i][j].isCoElec);
        }

        fprintf(inputFile,"\n");
    }


    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fprintf(inputFile,"%d ",partie.plato[i][j].isCoWater);
        }

        fprintf(inputFile,"\n");
    }



    //fermeture du fichier
    fclose(inputFile);



}


//SSPG chargé de charger une partie sauvegardée depuis un fichier texte
void loadSavedGame()
{
    t_cityData partie;
    t_image banqueImages;
    t_polices banquePolices;
    t_music banqueSon;
    int i,j,k;
    int tileSize=24; //taille en pixel des cases
    int offset_X = -tileSize/2+SCREEN_W/2+60; //offset de l'affichage par rapport à là ou il est censé etre
    int offset_Y = 50;

    banqueImages=loadBanqueImage();
    banquePolices=loadBanquePolices();
    banqueSon= loadBanqueSon();

    partie.matAdj=NULL;





    //Création du fichier
    FILE* readFile = NULL;

    //Ouvertuer du fichier
    readFile=fopen("sauvegarde.txt","r");

    //Blindage de l'ouverture du fichier de sauvegarde
    if(readFile==NULL)
    {
        printf("Erreur, l'ouverture du fichier de sauvegarde a echouee");
        exit(0);
    }


    ///Sauvegarde des données de la partie (t_city_data)


    fscanf(readFile,"%d",&partie.gold);
    fscanf(readFile,"%d\n",&partie.pop);

    fscanf(readFile,"%d",&partie.waterTot);//Otot
    fscanf(readFile,"%d",&partie.waterUsed);//Oused
    fscanf(readFile,"%d",&partie.elecTot);//Etot
    fscanf(readFile,"%d",&partie.elecUsed);//Eused


    fscanf(readFile,"%d",&partie.year);
    fscanf(readFile,"%d",&partie.month);
    fscanf(readFile,"%d",&partie.sec);
    fscanf(readFile,"%d",&partie.minu);
    fscanf(readFile,"%d",&partie.gameMode);
    fscanf(readFile,"%d",&partie.ID_cursorHab);
    fscanf(readFile,"%d",&partie.ID_cursorRes);
    fscanf(readFile,"%d",&partie.actionJoueur);


    partie.tabBatRessources = (t_ressources*)malloc(sizeof(t_ressources)*(partie.ID_cursorRes+1));


    partie.tabHabitations = (t_habitation*)malloc(sizeof(t_habitation)*(partie.ID_cursorHab+1));

    for(i=0;i<partie.ID_cursorRes;i++)
    {
        partie.tabBatRessources[i].tabIDSorted = (int*) malloc(sizeof(int)*(partie.ID_cursorHab+1));
        partie.tabBatRessources[i].tabDistances= (int*) malloc(sizeof(int)*(partie.ID_cursorHab+1));
    }






    ///-Chargement des données du graphe(Graphe)

    fscanf(readFile,"%d",&partie.gGraphe.ordre);
    fscanf(readFile,"%d",&partie.gGraphe.taille);

    partie.matAdj=creermMatrAdj(partie.matAdj,partie.gGraphe);
/*

    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            printf("%d ",partie.matAdj[i][j]);
        }
        printf("\n");
    }*/





    ///Chargements des données de batiments(t_ressources/t_habitation)



    //Habitations



    for ( k=0;k<partie.ID_cursorHab;k++)
    {

        for(i=0; i<9; i++)
        {
            for (j=0; j<2; j++)
            {
                fscanf(readFile,"%d",&partie.tabHabitations[k].tabCoord[i][j]);
            }

        }
     }


    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fscanf(readFile,"%d",&partie.tabHabitations[i].ID);
    }

    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fscanf(readFile,"%d",&partie.tabHabitations[i].population);
    }

    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fscanf(readFile,"%d",&partie.tabHabitations[i].niveau);
    }

    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fscanf(readFile,"%d",&partie.tabHabitations[i].in_eau);
    }

    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fscanf(readFile,"%d",&partie.tabHabitations[i].in_elec);
    }

    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fscanf(readFile,"%d",&partie.tabHabitations[i].tickUpdate);
    }

    for(i=0; i<partie.ID_cursorHab; i++)
    {
        fscanf(readFile,"%d",&partie.tabHabitations[i].evoFlag);
    }







    //Ressources

     for ( k=0;k<partie.ID_cursorRes;k++)
    {

        for(i=0; i<24; i++)
        {
            for (j=0; j<2; j++)
            {
                fscanf(readFile,"%d",&partie.tabBatRessources[k].tabCoord[i][j]);
            }

        }
     }




    for(i=0; i<partie.ID_cursorRes; i++)
    {
        fscanf(readFile,"%d",&partie.tabBatRessources[i].connections);
    }




    for( i=0; i<partie.ID_cursorRes; i++)
    {

        for(j=0; j< partie.tabBatRessources[i].connections; j++)
        {
            fscanf(readFile,"%d",&partie.tabBatRessources[i].tabIDSorted[j]);
        }


    }



    for( i=0; i<partie.ID_cursorRes; i++)
    {

        for(int j=0; j< partie.tabBatRessources[i].connections; j++)
        {
            fscanf(readFile,"%d",&partie.tabBatRessources[i].tabDistances[j]);
        }

    }



    for(i=0; i<partie.ID_cursorRes; i++)
    {
        fscanf(readFile,"%d",&partie.tabBatRessources[i].ID);
    }

     for(i=0; i<partie.ID_cursorRes; i++)
    {
        fscanf(readFile,"%d",&partie.tabBatRessources[i].resType);

    }

    for(i=0; i<partie.ID_cursorRes; i++)
    {
        fscanf(readFile,"%d",&partie.tabBatRessources[i].exportTot);
    }



    ///Chargement des données du plateau


    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fscanf(readFile,"%d",&partie.plato[i][j].tileType);
        }

    }

    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fscanf(readFile,"%d",&partie.plato[i][j].roadType);
        }

    }


    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fscanf(readFile,"%d",&partie.plato[i][j].bat_ID);
        }

    }



    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fscanf(readFile,"%d",&partie.plato[i][j].tileToDisplay);
        }

    }



    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fscanf(readFile,"%d",&partie.plato[i][j].anchorBat);
        }

    }



    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fscanf(readFile,"%d",&partie.plato[i][j].sommet);
        }

    }



    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fscanf(readFile,"%d",&partie.plato[i][j].isCoElec);
        }

    }


    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            fscanf(readFile,"%d",&partie.plato[i][j].isCoWater);
        }

    }



    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            partie.plato[i][j].x1 = 1*i*tileSize/2 + (-1)*j*tileSize/2 +offset_X;
            partie.plato[i][j].y1 = 0.5*i*tileSize/2 + 0.5*j*tileSize/2+ offset_Y;
            partie.plato[i][j].building_x= partie.plato[i][j].x1;
            partie.plato[i][j].building_y= partie.plato[i][j].y1 -  (0.5*tileSize/2 + 0.5*tileSize/2);
            partie.plato[i][j].hiddenColor= makecol(i,j,0);

        }
    }





    //fermeture du fichier
    fclose(readFile);


    boucleDeJeu(partie,banqueImages,banquePolices, banqueSon );
}
