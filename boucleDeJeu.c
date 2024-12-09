#include "header.h"


/*
    SSPG chargé du déroulement de la boucle de jeu
*/
void boucleDeJeu(t_cityData partie, t_image banqueImages, t_polices banquePolices, t_music banqueSon)
{


    BITMAP * buffer2x; //Déclaration du buffer
    buffer2x=create_bitmap(SCREEN_W,SCREEN_H);//creation du bitmap buffer double

    srand(time(NULL));
    int togglePause=0; //var bool pour mettre en pause le timer

    int playerInMenu=0; //var bool pour savoir si le joueur est dans le menu
    int choixMenu=0; //var stockant le

//     choix du joueur dans le menu de jeu
    int niveauVisu=0; //niveau de visualisation du terrain (0- route | 1- eau | 2- elec)
    t_cityData newPartie;//structure vide utilisée comme placeholder dans le cas d'un retour au menu principal


    int pauseTimer=0;
    int*pt_save=NULL;
    pt_save=(int*) malloc(5*sizeof(int));

    for(int i=0;i<4;i++)
    {
        pt_save[i]=0;
    }

    int eau_total_save=0,eau_used_save=0,elec_tot_save=0,elec_used_save=0,pop_save;

    int prixBat[4]; //tab des prix des batiments
    prixBat[0]= 10;//prix d'une route
    prixBat[1]= 1000; //prix d'un terrain vaque
    prixBat[2]= 100000; //prix d'une centrale
    prixBat[3]= 100000; //prix d'une water tower





    //timer pour animation
    //on définit deux "clocks", une pour mesurer le temps de départ, et une autre qu'on utilise pour calculer la différence
    clock_t beforeSwitchAnim = clock(); //temps de départ
    clock_t differenceSwitchAnim; //différence de temps

    //on définit msec pour établir en tant qu'entier le nombre de milisecondes écoulés, et trigger pour le seuil de déclenchement, lui aussi en milisecondes
    int msecSwitchAnim = 0, triggerSwitchAnim = 150; /* 20ms */
    int current_anim=0;





    //timer pour le décompte du temps en secondes
    clock_t beforeNextSec= clock(); //temps de départ
    clock_t differenceNextSec; //différence de temps

    int msecNextSec=0, triggerNextSec= 1000;

    /// BOUCLE DE JEU

    play_sample(banqueSon.jeu, 255, 128, 1000, 1);
    while(!key[KEY_ESC])
    {
        //on clear le buffer
        clear_bitmap(buffer2x);

        //affichage du decor du menu sur le buffer
        blit(banqueImages.Fond_Menu,buffer2x,0,0,0,0,SCREEN_W,SCREEN_H);




        ///------------------------------------------------AFFICHAGE------------------------------------------------
        //affichage du terrain de jeu
        affMapIso(buffer2x, partie.plato, banqueImages, partie.actionJoueur, niveauVisu, current_anim, partie);


        //affichage de l'interface de jeu
        affUIplateau(buffer2x, partie, banqueImages, banquePolices, playerInMenu, partie.minu, partie.sec, niveauVisu,pt_save, pauseTimer  );




        ///---------------------------------------------TIMER DE TEMPS-------------------------------------------------


        //on calcule la différence en ms entre le dernier reset
        differenceSwitchAnim = clock() - beforeSwitchAnim;
        msecSwitchAnim = differenceSwitchAnim * 1000 / CLOCKS_PER_SEC;

        //changement de frame d'animation
        if((msecSwitchAnim > triggerSwitchAnim))
        {
            //on réinisitalise la valeur du before
            beforeSwitchAnim=clock();

            current_anim++;
            if(current_anim>3)current_anim=0;

        }









        //timer de temps réel
        differenceNextSec = clock() - beforeNextSec;
        msecNextSec = differenceNextSec* 1000 / CLOCKS_PER_SEC;

        if((msecNextSec > triggerNextSec))
        {
            //on réinisitalise la valeur du before
            beforeNextSec=clock();
            differenceNextSec = clock() - beforeNextSec;
            msecNextSec = differenceNextSec* 1000 / CLOCKS_PER_SEC;

            if(!togglePause)
            {


                partie.sec++;

                if(partie.sec%15==14)
                {
                    if(partie.ID_cursorHab!=0)
                    {
                        for(int i=0; i<partie.ID_cursorHab; i++)
                        {
                            partie.tabHabitations[i].evoFlag=1;
                        }
                    }
                    partie.month++;
                    if(partie.month>12)
                    {
                        partie.month=1;
                        partie.year++;

                    }
                }
                if(partie.sec>59)
                {
                    partie.sec=0;
                    partie.minu++;
                }

            }
        }

        ///---------------------------------------------------------MISE A JOUR DES HABITATIONS------------------------------------------------
        //on teste voir si on a des habitations
        if(partie.ID_cursorHab!=0)
        {
            //si oui, on boucle pour chacune
            for(int i=0; i<partie.ID_cursorHab; i++)
            {

                //si son timer d'évolution atteint sa marque et qu'elle n'as pas encore évoluée
                if(partie.tabHabitations[i].tickUpdate==partie.sec%15 && partie.tabHabitations[i].evoFlag)
                {
                    //on set son evoFLag à 0 (il viens d'évoluer)
                    partie.tabHabitations[i].evoFlag=0;
                    //on lance l'update
                    partie = updateHab(partie, partie.gameMode, i);


                    //on calcule les impots lié à l'habitation
                    partie.gold= partie.gold + partie.tabHabitations[i].population*10;
                }
            }
        }

        if(partie.ID_cursorHab!=0 && partie.ID_cursorRes!=0) //si on a des batiment et des habitations
        {
            for(int i=0; i<partie.ID_cursorRes; i++)
            {
                partie = triTabDist(partie, i);
            }

            partie=gestionRes(partie);
        }



        ///---------------------------------------------------------DETECTION D'ACTION DU JOUEUR------------------------------------------------
        //sortir d'une action
        if(mouse_b&2) partie.actionJoueur=0;

        //on détecte si le joueur clique sur une case pour poser un batiment
        for(int i=1; i<9; i++)
        {
            //on regarde sur quelle case il clique et si il a assez d'argent pour construire le batiment en question
            if(isClickInCoord(38+(i-1)*123, 123+(i-1)*123, 570,  655) && !playerInMenu && i!=5)
            {
                //on set son action au chiffre correspondant au batiment sélectionné
                if(i<5&&!niveauVisu &&  partie.gold>=prixBat[i-1]) partie.actionJoueur=i;
                else if(i>5)
                {
                    partie.actionJoueur=0;
                    niveauVisu=i-6;
                }
            }
        }

        if(isClickInCoord(895, 955, 120, 180))
        {
            pauseTimer=0;
        }
        if(isClickInCoord(959, SCREEN_W-5, 120, 180))
        {
            pauseTimer=1;
        }






        if(key[KEY_0]) partie.actionJoueur=0;
        if(key[KEY_1]) partie.actionJoueur=1;
        if(key[KEY_2]) partie.actionJoueur=2;
        if(key[KEY_3]) partie.actionJoueur=3;
        if(key[KEY_4]) partie.actionJoueur=4;


        ///---------------------------------------------POSER UN BATIMENT---------------------------------------------

        if(partie.actionJoueur!=0) partie = poseBatiment(partie, partie.actionJoueur, banqueImages, partie.sec%15);



        ///---------------------------------------------------MENU DE JEU---------------------------------------------
        //on détecte si le joueur veut entrer dans le menu
        if(isClickInCoord(765, SCREEN_W-5, 5, 115)&& !playerInMenu)
        {
            playerInMenu=1;
            partie.actionJoueur=0;
            niveauVisu=0;
        }
        if(playerInMenu || pauseTimer) togglePause=1;
        else togglePause=0;

        //on détecte son choix dans le menu
        for(int i=0; i<4; i++)
        {
            if(isClickInCoord(340,660,175+i*90, 250+i*90 )&&playerInMenu)
            {
                if(i==0)
                {
                    playerInMenu=0;
                }
                else choixMenu=i;
            }
        }


        if(choixMenu!=0)
        {
            switch(choixMenu)
            {
            case 1:
                //sauvegarder
                eau_total_save=pt_save[0];
                eau_used_save=pt_save[1];
                elec_tot_save=pt_save[2];
                elec_used_save=pt_save[3];
                pop_save=pt_save[4];


                saveGame(partie,eau_total_save,elec_tot_save,eau_used_save,elec_used_save,pop_save);
                break;

            case 2:
                //sauvegarder et quitter
                //on sauvegarde
                eau_total_save=pt_save[0];
                eau_used_save=pt_save[1];
                elec_tot_save=pt_save[2];
                elec_used_save=pt_save[3];
                pop_save=pt_save[4];


                saveGame(partie,eau_total_save,elec_tot_save,eau_used_save,elec_used_save,pop_save);
                stop_sample(banqueSon.jeu);
                //on quitte vers le menu
                mainMenu(newPartie, banqueImages, banquePolices, banqueSon);
                break;

            case 3:
                //quitter sans sauvegarder
                stop_sample(banqueSon.jeu);
                //on quitte vers le menu
                mainMenu(newPartie, banqueImages, banquePolices, banqueSon);
                break;
            }
        }



        //on affiche le buffer à l'écran
        blit(buffer2x, screen, 0,0,0,0, SCREEN_W,SCREEN_H);


    }


}


//SSPG chargé de la pose d'un batiment et du remplissage des structures utilisée
t_cityData poseBatiment(t_cityData partie, int actionJoueur, t_image banqueImage, int tickPoseBat )
{

    int caseID=0;


    for(int i=0; i<35; i++)
    {
        for(int j=0; j<45; j++)
        {
            switch(partie.actionJoueur)
            {
            case 1: //poser une route
                if(getpixel(banqueImage.HiddenMap, mouse_x, mouse_y)==makecol(i+1,j+1,0)&& partie.plato[i][j].tileType==0&&mouse_b&1)
                {
                    if(partie.gold<=10)  partie.actionJoueur=0;
                    //on retire le prix
                    partie.gold=partie.gold-10;
                    //on pose la case de route
                    partie.plato[i][j].tileType=3; //route

                    partie.plato[i][j].roadType= detectOrientRoute(i, j, partie.plato);

                    if(partie.plato[i][j+1].tileType==3)
                    {
                        partie.plato[i][j+1].roadType= detectOrientRoute(i, j+1, partie.plato);
                    }
                    if(partie.plato[i][j-1].tileType==3)
                    {
                        partie.plato[i][j-1].roadType= detectOrientRoute(i, j-1, partie.plato);
                    }
                    if(partie.plato[i-1][j].tileType==3)
                    {
                        partie.plato[i-1][j].roadType= detectOrientRoute(i-1, j, partie.plato);
                    }
                    if(partie.plato[i+1][j].tileType==3)
                    {
                        partie.plato[i+1][j].roadType= detectOrientRoute(i+1, j, partie.plato);
                    }
                    partie.plato[i][j].roadType= detectOrientRoute(i, j, partie.plato);
                    //on lance la détection de route
                    //on set l'action du joueur à 0

                    partie.gGraphe = graphe_brut(partie.plato, partie.gGraphe); //réécriture du graphe
                    partie.matAdj=creermMatrAdj(partie.matAdj, partie.gGraphe); //réécriture de la matrice d'adjacence

                    if(partie.ID_cursorHab!=0 && partie.ID_cursorRes!=0)
                    {
                        for(int g=0; g<partie.ID_cursorRes; g++)
                        {
                            for(int h=0; h<partie.ID_cursorHab; h++)
                            {
                                //calcule de toutes les distances
                                partie.tabBatRessources[g].tabDistances[h]= calcSmallestDist(partie, g, h);

                            }
                        }
                    }

                    for(int l=0; l<35; l++)
                    {
                        for(int m=0; m<45; m++)
                        {
                            for(int x=0; x<35; x++)
                            {
                                for(int y=0; y<45; y++)
                                {
                                    if(partie.plato[x][y].tileType==3)
                                    {
                                        if(!partie.plato[x][y].isCoWater) isCoBat(x, y, 1, partie.plato);
                                        if(!partie.plato[x][y].isCoElec) isCoBat(x, y, 2, partie.plato);


                                    }
                                }
                            }
                        }
                    }


                }
                break;

            case 2: //poser une maison
                if(getpixel(banqueImage.HiddenMap, mouse_x, mouse_y)==makecol(i+1,j+1,0)&& kaseFreeInRange(i,j,3,3,partie.plato)&& mouse_b&1)
                {
                    partie.gold=partie.gold-1000;

                    for(int lo=0; lo<3; lo++)
                    {
                        for(int la=0; la<3; la++)
                        {
                            partie.plato[i+lo][j+la].tileType=4; //on dit que le les cases sélectionnées sont des habitations
                            partie.plato[i+lo][j+la].bat_ID= partie.ID_cursorHab;
                            partie.tabHabitations[partie.ID_cursorHab].tabCoord[caseID][0]=i+lo;
                            partie.tabHabitations[partie.ID_cursorHab].tabCoord[caseID][1]=j+la;
                            caseID++;
                        }
                    }
                    caseID=0;

                    partie.tabHabitations[partie.ID_cursorHab].ID= partie.ID_cursorHab;
                    partie.tabHabitations[partie.ID_cursorHab].in_eau=0;
                    partie.tabHabitations[partie.ID_cursorHab].in_elec=0;
                    partie.tabHabitations[partie.ID_cursorHab].population=0;
                    partie.tabHabitations[partie.ID_cursorHab].niveau=0;

                    partie.tabHabitations[partie.ID_cursorHab].tickUpdate=tickPoseBat;

                    partie.tabHabitations[partie.ID_cursorHab].evoFlag=0;




                    partie.gGraphe = graphe_brut(partie.plato, partie.gGraphe); //réécriture du graphe
                    partie.matAdj=creermMatrAdj(partie.matAdj, partie.gGraphe); //réécriture de la matrice d'adjacence






                    //si on a déjà posé des batiments de ressources
                    if(partie.ID_cursorRes!=0)
                    {

                        for(int h=0; h<partie.ID_cursorRes; h++)
                        {
                            //on réalloue les tableaux de distances entre les batiments de ressources et les habtitations
                            partie.tabBatRessources[h].tabDistances =  realloc(partie.tabBatRessources[h].tabDistances, sizeof(int)*(partie.ID_cursorHab+1));

                            //on calcule la ditance minimale entre l'habitation qui vient d'être posée et chaque batiment de ressources
                            partie.tabBatRessources[h].tabDistances[partie.ID_cursorHab] = calcSmallestDist(partie, h, partie.ID_cursorHab);

                        }
                    }
                    partie.plato[i][j].anchorBat=1;
                    partie.ID_cursorHab++;
                    partie.tabHabitations = realloc(partie.tabHabitations,sizeof(t_habitation)*(partie.ID_cursorHab+1)); //on réalloue de la mémoire pour stocker le prochain batiment

                    partie.actionJoueur=0;

                }

                break;



            case 3: //poser un chateau d'eau
                if(getpixel(banqueImage.HiddenMap, mouse_x, mouse_y)==makecol(i+1,j+1,0)&& kaseFreeInRange(i,j, 4, 6, partie.plato)&&mouse_b&1)
                {
                    partie.gold=partie.gold-100000;

                    caseID=0;
                    for(int lo=0; lo<4; lo++)
                    {
                        for(int la=0; la<6; la++)
                        {
                            partie.plato[i+lo][j+la].tileType=5; //on dit que le les cases sélectionnées sont des habitations
                            partie.plato[i+lo][j+la].bat_ID= partie.ID_cursorRes;
                            partie.tabBatRessources[partie.ID_cursorRes].tabCoord[caseID][0]=i+lo;
                            partie.tabBatRessources[partie.ID_cursorRes].tabCoord[caseID][1]=j+la;
                            caseID++;
                        }
                    }
                    caseID=0;

                    partie.tabBatRessources[partie.ID_cursorRes].exportTot=0;
                    partie.tabBatRessources[partie.ID_cursorRes].ID= partie.ID_cursorRes;
                    partie.tabBatRessources[partie.ID_cursorRes].resType=1;

                    //on alloue dynamiquement le tableau des distances par rapport aux habitations
                    partie.tabBatRessources[partie.ID_cursorRes].tabDistances= (int*) malloc(sizeof(int)*(partie.ID_cursorHab+1));
                    partie.tabBatRessources[partie.ID_cursorRes].tabIDSorted = (int*) malloc(sizeof(int)*(partie.ID_cursorHab+1));

                    partie.gGraphe = graphe_brut(partie.plato, partie.gGraphe); //réécriture du graphe
                    partie.matAdj=creermMatrAdj(partie.matAdj, partie.gGraphe); //réécriture de la matrice d'adjacence
                    partie.tabBatRessources[partie.ID_cursorRes].connections=0;

                    for(int h=0; h< partie.ID_cursorHab; h++)
                    {
                        partie.tabBatRessources[partie.ID_cursorRes].tabDistances[h]= calcSmallestDist(partie, partie.ID_cursorRes, h);
                        //on calcule la plus courte distance entre le batiment de ressources qui vient d'être posé et chaque habitations
                    }


                    for(int l=0; l<35; l++)
                    {
                        for(int m=0; m<45; m++)
                        {
                            for(int x=0; x<35; x++)
                            {
                                for(int y=0; y<45; y++)
                                {
                                    if(partie.plato[x][y].tileType==3)
                                    {


                                       if(!partie.plato[x][y].isCoWater) isCoBat(x, y, 1, partie.plato);
                                    }
                                }
                            }
                        }
                    }


                    partie.ID_cursorRes++;
                    //on réalloue de la mémoire pour stocker le prochain batiment
                    partie.tabBatRessources = realloc(partie.tabBatRessources, sizeof(t_ressources)*(partie.ID_cursorRes+1));
                    partie.plato[i][j].anchorBat=1;

                    partie.actionJoueur=0;
                }
                break;

            case 4: //poser une centrale
                if(getpixel(banqueImage.HiddenMap, mouse_x, mouse_y)==makecol(i+1,j+1,0)&& kaseFreeInRange(i,j, 4, 6, partie.plato)&&mouse_b&1)
                {
                    partie.gold=partie.gold-100000;

                    for(int lo=0; lo<4; lo++)
                    {
                        for(int la=0; la<6; la++)
                        {
                            partie.plato[i+lo][j+la].tileType=6; //on dit que le les cases sélectionnées sont des habitations
                            partie.plato[i+lo][j+la].bat_ID= partie.ID_cursorRes;
                            partie.tabBatRessources[partie.ID_cursorRes].tabCoord[caseID][0]=i+lo;
                            partie.tabBatRessources[partie.ID_cursorRes].tabCoord[caseID][1]=j+la;
                            caseID++;
                        }
                    }

                    caseID=0;
                    partie.tabBatRessources[partie.ID_cursorRes].exportTot=0;
                    partie.tabBatRessources[partie.ID_cursorRes].ID= partie.ID_cursorRes;
                    partie.tabBatRessources[partie.ID_cursorRes].resType=2;

                    partie.tabBatRessources[partie.ID_cursorRes].tabDistances = (int*) malloc(sizeof(int)*(partie.ID_cursorHab+1));
                    partie.tabBatRessources[partie.ID_cursorRes].tabIDSorted = (int*) malloc(sizeof(int)*(partie.ID_cursorHab+1));

                    partie.gGraphe = graphe_brut(partie.plato, partie.gGraphe); //réécriture du graphe
                    partie.matAdj=creermMatrAdj(partie.matAdj, partie.gGraphe); //réécriture de la matrice d'adjacence
                    partie.tabBatRessources[partie.ID_cursorRes].connections=0;

                    for(int h=0; h< partie.ID_cursorHab; h++)
                    {
                        partie.tabBatRessources[partie.ID_cursorRes].tabDistances[h]= calcSmallestDist(partie, partie.ID_cursorRes, h);
                        //on calcule la plus courte distance entre le batiment de ressources qui vient d'être posé et chaque habitations
                    }
                    for(int l=0; l<35; l++)
                    {
                        for(int m=0; m<45; m++)
                        {
                            for(int x=0; x<35; x++)
                            {
                                for(int y=0; y<45; y++)
                                {
                                    if(partie.plato[x][y].tileType==3)
                                    {
                                        if(!partie.plato[x][y].isCoElec) isCoBat(x, y, 2, partie.plato);

                                    }
                                }
                            }
                        }
                    }

                    partie.ID_cursorRes++;
                    partie.tabBatRessources = realloc(partie.tabBatRessources, sizeof(t_ressources)*(partie.ID_cursorRes+1));
                    partie.plato[i][j].anchorBat=1;
                    partie.actionJoueur=0;
                }
                break;
            }

        }
    }
    return partie;
}


/*
    SSPG chargé de l'affichage de l'interface du plateau de jeu
*/
void affUIplateau(BITMAP * buffer2x, t_cityData partie, t_image banqueImages, t_polices banquePolices, int InMenu, int timerM, int timerS, int nivVisu,int*pt_save, int togglePause)
{


    ///INFO PARTIE
    rectfill(buffer2x, 0, 0, 290, 164, makecol(0,0,0)); //Fond du carré d'information
    rectfill(buffer2x, 5, 5, 285, 159, makecol(171, 224, 215)); //Fond du carré d'information

    //on calcule la quantité totale de ressources pour l'afficher au joueur
    partie.waterTot=0;
    partie.elecTot=0;
    partie.waterUsed=0;
    partie.elecUsed=0;
    partie.pop=0;

    for(int i=0; i<partie.ID_cursorRes; i++)
    {
        if(partie.tabBatRessources[i].resType==1)
        {
            partie.waterTot= partie.waterTot+5000;
            partie.waterUsed= partie.waterUsed+ partie.tabBatRessources[i].exportTot;
        }
        else
        {
            partie.elecTot= partie.elecTot+5000;
            partie.elecUsed= partie.elecUsed+ partie.tabBatRessources[i].exportTot;
        }
    }

    pt_save[0]=partie.waterTot;
    pt_save[1]=partie.waterUsed;
    pt_save[2]=partie.elecTot;
    pt_save[3]=partie.elecUsed;

    for(int i=0; i< partie.ID_cursorHab; i++)
    {
        partie.pop= partie.pop + partie.tabHabitations[i].population;
    }
    pt_save[4]=partie.pop;




    textprintf_ex(buffer2x, banquePolices.Pixel_11,10, 10, makecol(0,0,0), -1, "Argent: %d$ ", partie.gold);
    textprintf_ex(buffer2x, banquePolices.Pixel_11,10, 30, makecol(0,0,0), -1, "Population: %d ", partie.pop);
    textprintf_ex(buffer2x, banquePolices.Pixel_11,10, 50, makecol(0,0,0), -1, "Eau: %d/%d ", partie.waterUsed, partie.waterTot);
    textprintf_ex(buffer2x, banquePolices.Pixel_11,10, 70, makecol(0,0,0), -1, "Elec.: %d/%d ", partie.elecUsed, partie.elecTot);

    if(timerS<10) textprintf_ex(buffer2x, banquePolices.Pixel_11,180, 120, makecol(0,0,0), -1, "(%d:0%d)",timerM, timerS);
    else textprintf_ex(buffer2x, banquePolices.Pixel_11,180, 120, makecol(0,0,0), -1, "(%d:%d)",timerM,timerS);
    textprintf_ex(buffer2x, banquePolices.Pixel_11,80, 140, makecol(0,0,0), -1, "Date: %d/%d ", partie.month, partie.year);





    rectfill(buffer2x, 0, 545, SCREEN_W, SCREEN_H, makecol(0,0,0)); //Fond toolbar
    rectfill(buffer2x, 5, 550, SCREEN_W-5, SCREEN_H-5, makecol(192,191,196)); //Rectangle Toolbar


    //Case action joueur
    for(int i=1; i<9; i++)
    {
        if(i!=5)
        {

            //case blanche (icone sorts)
            rectfill(buffer2x, 33+(i-1)*123, 565, 128+(i-1)*123, 660, makecol(0,0,0) );
            rectfill(buffer2x, 38+(i-1)*123, 570, 123+(i-1)*123, 655, makecol(255,255,255) );
            switch(i)
            {
            //cases poser un batiment

            case 1://route
                masked_stretch_blit(banqueImages.actionRoute, buffer2x, 0,0, 24,24,38+(i-1)*123,570, 85,85 );
                if(partie.actionJoueur==i)
                {
                    for(int x=38+(i-1)*123; x< 38+(i-1)*123+86; x++)
                    {
                        for(int y= 570; y< 570+86; y++)
                        {
                            if(getpixel(buffer2x, x, y)==makecol(255,255,255)) putpixel(buffer2x, x, y,makecol(189,189,189));
                        }
                    }
                }
                break;

            case 2: //habitation
                masked_blit(banqueImages.actionHabitation, buffer2x, 0,0,38+(i-1)*123,570, 85,85 );
                if(partie.actionJoueur==i)
                {
                    for(int x=38+(i-1)*123; x< 38+(i-1)*123+86; x++)
                    {
                        for(int y= 570; y< 570+86; y++)
                        {
                            if(getpixel(buffer2x, x, y)==makecol(255,255,255)) putpixel(buffer2x, x, y,makecol(189,189,189));
                        }
                    }
                }
                break;

            case 3: //chateau d'eau
                masked_blit(banqueImages.actionWaterTower, buffer2x, 0,0,38+(i-1)*123,570, 85,85 );
                if(partie.actionJoueur==i)
                {
                    for(int x=38+(i-1)*123; x< 38+(i-1)*123+86; x++)
                    {
                        for(int y= 570; y< 570+86; y++)
                        {
                            if(getpixel(buffer2x, x, y)==makecol(255,255,255)) putpixel(buffer2x, x, y,makecol(189,189,189));
                        }
                    }
                }
                break;

            case 4: //centrale
                masked_blit(banqueImages.actionCentrale, buffer2x, 0,0,38+(i-1)*123,570, 85,85 );
                if(partie.actionJoueur==i)
                {
                    for(int x=38+(i-1)*123; x< 38+(i-1)*123+86; x++)
                    {
                        for(int y= 570; y< 570+86; y++)
                        {
                            if(getpixel(buffer2x, x, y)==makecol(255,255,255)) putpixel(buffer2x, x, y,makecol(189,189,189));
                        }
                    }
                }
                break;

            //cases niveau de visualisation

            case 6: //route (niv 0)
                blit(banqueImages.visuRoute, buffer2x, 0,0,38+(i-1)*123,570, 85,85 );
                if(nivVisu==i-6)
                {
                    for(int x=38+(i-1)*123; x< 38+(i-1)*123+86; x++)
                    {
                        for(int y= 570; y< 570+86; y++)
                        {
                            if(getpixel(buffer2x, x, y)==makecol(255,255,255)) putpixel(buffer2x, x, y,makecol(189,189,189));
                        }
                    }
                }
                break;

            case 7: //eau (niv -1)
                blit(banqueImages.visuEau, buffer2x, 0,0,38+(i-1)*123,570, 85,85 );
                if(nivVisu==i-6)
                {
                    for(int x=38+(i-1)*123; x< 38+(i-1)*123+86; x++)
                    {
                        for(int y= 570; y< 570+86; y++)
                        {
                            if(getpixel(buffer2x, x, y)==makecol(255,255,255)) putpixel(buffer2x, x, y,makecol(189,189,189));
                        }
                    }
                }
                break;

            case 8: //elec (niv -2)
                blit(banqueImages.visuElec, buffer2x, 0,0,38+(i-1)*123,570, 85,85 );
                if(nivVisu==i-6)
                {
                    for(int x=38+(i-1)*123; x< 38+(i-1)*123+86; x++)
                    {
                        for(int y= 570; y< 570+86; y++)
                        {
                            if(getpixel(buffer2x, x, y)==makecol(255,255,255)) putpixel(buffer2x, x, y,makecol(189,189,189));
                        }
                    }
                }
                break;

            }


        }
    }

    ///CASE MENU
    rectfill(buffer2x, 760, 0, SCREEN_W, 120, makecol(0,0,0));
    rectfill(buffer2x, 765, 5, SCREEN_W-5, 115, makecol(192,191,196));

    if(isMouseInCoord(765, SCREEN_W-5, 5, 115)&& !InMenu)
    {
        rectfill(buffer2x, 765, 5, SCREEN_W-5, 115,makecol(143, 188, 114) );
    }

    textout_centre_ex(buffer2x, banquePolices.Kabel_28, "Menu", 892-3,20+2, makecol(0,0,0), -1);
    textout_centre_ex(buffer2x, banquePolices.Kabel_28, "Menu", 892,20, makecol(255,255,255), -1);


    ///BOUTON PAUSE
    //fond noir
    rectfill(buffer2x, 890, 120, SCREEN_W, 185, makecol(0,0,0) );
    //case play
    rectfill(buffer2x, 895, 120, 955, 180, makecol(192,191,196));
    if(isMouseInCoord(895, 955, 120, 180)&& !InMenu)
    {
        rectfill(buffer2x, 895, 120, 955, 180,makecol(143, 188, 114) );
    }
    //case pause
    rectfill(buffer2x, 959, 120, SCREEN_W-5, 180, makecol(192,191,196));
    if(isMouseInCoord(959, SCREEN_W-5, 120, 180)&& !InMenu )
    {
        rectfill(buffer2x, 959, 120, SCREEN_W-5, 180,makecol(143, 188, 114) );
    }
    else if( togglePause==1)
    {
        rectfill(buffer2x, 959, 120, SCREEN_W-5, 180,makecol(120,120,120));
    }

    masked_blit(banqueImages.play, buffer2x, 0,0, 895, 120, 60,60);
    masked_blit(banqueImages.pause, buffer2x, 0,0, 959, 120, 60,60);




    /// AFFICHAGE MENU


    if(InMenu)
    {
        rectfill(buffer2x, 205,65,795,535, makecol(192,191,196));
        for(int i=0; i<6; i++)
        {
            rect(buffer2x, 205-i,65-i,795+i,535+i, makecol(66,40,14));
            if(i==5 || i==0)
            {
                rect(buffer2x, 205-i,65-i,795+i,535+i, makecol(0,0,0));
            }

        }

        textout_centre_ex(buffer2x, banquePolices.Kabel_28, "Menu", 205+590/2-3,70+2,  makecol(0,0,0),-1);
        textout_centre_ex(buffer2x, banquePolices.Kabel_28, "Menu",  205+590/2, 70, makecol(255,255,255),-1);

        for(int i=0; i<4; i++)
        {
            for(int j=0; j<6; j++)
            {
                if(isMouseInCoord(340,660,175+i*90, 250+i*90 ))
                {
                    rectfill(buffer2x,340,175+i*90, 660,250+i*90, makecol(143, 188, 114));
                }
                rect(buffer2x, 340-j,175+i*90 -j, 660+j,250+i*90+j,makecol(66,40,14) );
                if(j==5 || j==0)
                {
                    rect(buffer2x, 340-j,175+i*90 -j, 660+j,250+i*90+j,makecol(0,0,0) );
                }

                switch(i)
                {
                case 0:
                    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Reprendre", 205+590/2-3,180+i*90+2, makecol(0,0,0), -1 );
                    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Reprendre", 205+590/2,180+i*90, makecol(255,255,255), -1 );
                    break;

                case 1:
                    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Sauvegarder", 205+590/2-3,180+i*90+2, makecol(0,0,0), -1 );
                    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Sauvegarder", 205+590/2,180+i*90, makecol(255,255,255), -1 );
                    break;

                case 2:
                    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Sauvegarder ", 205+590/2-3,170+i*90+2, makecol(0,0,0), -1 );
                    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Sauvegarder ", 205+590/2,170+i*90, makecol(255,255,255), -1 );

                    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "et Quitter", 205+590/2-3,200+i*90+2, makecol(0,0,0), -1 );
                    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "et Quitter", 205+590/2,200+i*90, makecol(255,255,255), -1 );

                    break;

                case 3:
                    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Quitter sans", 205+590/2-3,170+i*90+2, makecol(0,0,0), -1 );
                    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Quitter sans", 205+590/2,170+i*90, makecol(255,255,255), -1 );

                    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Sauvegarder", 205+590/2-3,200+i*90+2, makecol(0,0,0), -1 );
                    textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Sauvegarder", 205+590/2,200+i*90, makecol(255,255,255), -1 );
                    break;
                }
            }

        }

    }


}




/*
    SSPG chargé de l'affichage de la carte isométrique chargée au préalable
*/
void affMapIso(BITMAP * buffer2x, t_kase plato[35][45], t_image banqueImage, int actionJoueur, int niveauVisu, int current_anim, t_cityData partie)
{

    int tabSeuil[5]; //tableau stockant les valeur seuil de population des différents niveau d'habitations
    tabSeuil[0] = 0; //terrain vague
    tabSeuil[1] = 10; //cabane
    tabSeuil[2] = 50; //maison
    tabSeuil[3] = 100; //immeuble
    tabSeuil[4] = 1000; //gratte ciel


    ///AFFICHAGE DES TILES ET OBSTACLES


    //detection du type de tile a afficher en fonction de la couleur cachée
    for(int i=0; i<35; i++)
    {
        for(int j=0; j<45; j++)
        {
            //on regarde si la couleur du pixel à l'emplacement des coordonées de la souris sur
            //la hiddenMap correpsond à la couleur de la case affichée
            if(getpixel(banqueImage.HiddenMap, mouse_x, mouse_y)==makecol(i+1,j+1,0)&&!plato[i][j].anchorBat)
            {



                ///AFFICHAGE CASE SELECTIONNEE
                //si on est en train de poser un batiment
                switch(actionJoueur)
                {
                case 2://poser une maison

                    if((3+i)<=35 && (3+j)<=45 && kaseFreeInRange(i, j, 3,3, plato)&& i!=35 &&j!=45)//on regarde si il y a assez de cases dans la proximité pour poser le batiment
                    {
                        for(int x=0; x<3; x++)
                        {
                            for(int y=0; y<3; y++)
                            {
                                plato[i+x][j+y].isSelected=1; //on dit aux cases adjacentes qu'elles sont aussi sélectionnées
                            }
                        }
                    }
                    else //si il n'y a pas assez de place pres du bord
                    {
                        for(int x=0; x<3&&x+i<35; x++)
                        {
                            for(int y=0; y<3&&y+j<45; y++)
                            {
                                plato[i+x][j+y].isSelected=2;
                            }
                        }
                    }

                    break;

                case 3:
                    if((4+i)<=35 && (6+j)<=45 && kaseFreeInRange(i, j, 4,6, plato))//on regarde si il y a assez de cases dans la proximité pour poser le batiment
                    {
                        for(int x=0; x<4&&x+i<35; x++)
                        {
                            for(int y=0; y<6&&y+j<45; y++)
                            {
                                plato[i+x][j+y].isSelected=1; //on dit aux cases adjacentes qu'elles sont aussi sélectionnées
                            }
                        }
                    }
                    else //si il n'y a pas assez de place pres du bord
                    {
                        for(int x=0; x<4&&x+i<35; x++)
                        {
                            for(int y=0; y<6&&y+j<45; y++)
                            {
                                plato[i+x][j+y].isSelected=2;
                            }
                        }
                    }
                    break;

                case 4:
                    if((4+i)<=35 && (6+j)<=45 && kaseFreeInRange(i, j, 4,6, plato))//on regarde si il y a assez de cases dans la proximité pour poser le batiment
                    {
                        for(int x=0; x<4&&x+i<35; x++)
                        {
                            for(int y=0; y<6&&y+j<45; y++)
                            {
                                plato[i+x][j+y].isSelected=1; //on dit aux cases adjacentes qu'elles sont aussi sélectionnées
                            }
                        }
                    }
                    else //si il n'y a pas assez de place pres du bord
                    {
                        for(int x=0; x<4&&x+i<35; x++)
                        {
                            for(int y=0; y<6&&y+j<45; y++)
                            {
                                plato[i+x][j+y].isSelected=2;
                            }
                        }
                    }
                    break;

                default:
                    plato[i][j].tileToDisplay=1;
                    break;


                }
                if(plato[i+1][j].isSelected==2 || plato[i][j+1].isSelected==2 || (i==35 && j==45) )
                {
                    plato[i][j].tileToDisplay=2;
                }
                else plato[i][j].tileToDisplay=1;

                //si oui, on dit a cette tile qu'elle doit afficher l'apparence "selectionéee"

            }

            else if(!plato[i][j].isSelected && plato[i][j].tileToDisplay!=1)
            {
                plato[i][j].tileToDisplay =plato[i][j].tileType;

            }
            else if(plato[i][j].isSelected==1)
            {
                plato[i][j].tileToDisplay =1;
                plato[i][j].isSelected=0;
            }
            else
            {
                plato[i][j].tileToDisplay =2;
                plato[i][j].isSelected=0;
            }
        }
    }



    ///----------------------------------------------AFFICHAGE DES TILES-----------------------------///
    for(int i=0; i<35; i++)
    {
        for(int j=0; j<45; j++)
        {
            switch(niveauVisu)
            {


            case 0:
                switch (plato[i][j].tileToDisplay)
                {

                case 3:
                    //affichage case route

                    switch(plato[i][j].roadType)
                    {
                    //roadY
                    case 1:
                        masked_stretch_blit(banqueImage.Tile_Road_Y, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                        break;

                    //roadX
                    case 2:
                        masked_stretch_blit(banqueImage.Tile_Road_X, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                        break;

                    //turndown
                    case 3:
                        masked_stretch_blit(banqueImage.Tile_Road_Turn_Down, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                        break;

                    //turnright
                    case 4:
                        masked_stretch_blit(banqueImage.Tile_Road_Turn_Right, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                        break;

                    //turnup
                    case 5:
                        masked_stretch_blit(banqueImage.Tile_Road_Turn_Up, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                        break;

                    //turnleft
                    case 6:
                        masked_stretch_blit(banqueImage.Tile_Road_Turn_Left, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                        break;

                    //3way3
                    case 7:
                        masked_stretch_blit(banqueImage.Tile_Road_3Way_3, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                        break;

                    //3way1
                    case 8:
                        masked_stretch_blit(banqueImage.Tile_Road_3Way_1, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                        break;

                    //3way4
                    case 9:
                        masked_stretch_blit(banqueImage.Tile_Road_3Way_4, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                        break;

                    //3way2
                    case 10:
                        masked_stretch_blit(banqueImage.Tile_Road_3Way_2, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                        break;

                    //cross
                    case 11:
                        masked_stretch_blit(banqueImage.Tile_Road_Cross, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                        break;

                    }

                    break;

                case 1:
                    //si la case est sélectionée
                    //on l'affiche en bleu
                    masked_stretch_blit(banqueImage.tile_Selected, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                    //on lui dit qu'elle n'est plus sélectionée
                    plato[i][j].tileToDisplay=plato[i][j].tileType;

                    break;

                case 2:
                    masked_stretch_blit(banqueImage.tile_Occupied, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                    //on lui dit qu'elle n'est plus sélectionée
                    plato[i][j].tileToDisplay=plato[i][j].tileType;

                    break;

                default:
                    masked_stretch_blit(banqueImage.tile_Default, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                    break;


                }
                break;


            //visu eau
            case 1:

                switch (plato[i][j].tileToDisplay)
                {
                //case route
                case 3:
                    if(plato[i][j].isCoWater)masked_stretch_blit(banqueImage.tile_Water, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                    else masked_stretch_blit(banqueImage.tile_Default, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                    break;

                case 1:
                    //si la case est sélectionée
                    //on l'affiche en bleu
                    masked_stretch_blit(banqueImage.tile_Selected, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                    //on lui dit qu'elle n'est plus sélectionée
                    plato[i][j].tileToDisplay=plato[i][j].tileType;

                    break;

                default:
                    masked_stretch_blit(banqueImage.tile_Default, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                    break;
                }


                break;

            //visu elec
            case 2:
                switch (plato[i][j].tileToDisplay)
                {
                //case route
                case 3:
                    if(plato[i][j].isCoElec)masked_stretch_blit(banqueImage.tile_Elec, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                    else masked_stretch_blit(banqueImage.tile_Default, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                    break;

                case 1:
                    //si la case est sélectionée
                    //on l'affiche en bleu
                    masked_stretch_blit(banqueImage.tile_Selected, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                    //on lui dit qu'elle n'est plus sélectionée
                    plato[i][j].tileToDisplay=plato[i][j].tileType;

                    break;

                default:
                    masked_stretch_blit(banqueImage.tile_Default, buffer2x, 0,0, 24,24, plato[i][j].x1, plato[i][j].y1, 24,24);
                    break;
                }
                break;
            }


        }
    }


    ///---------------------------------------AFFICHAGE DES BATIMENTS---------------------------------//

    for(int i=0; i<35; i++)
    {
        for(int j=0; j<45; j++)
        {
            switch(niveauVisu)
            {
            case 0: //visu route
                switch (plato[i][j].tileToDisplay)
                {
                case 4: //habitation
                    if(plato[i][j].anchorBat)
                    {
                        switch(partie.tabHabitations[plato[i][j].bat_ID].niveau)
                        {
                        case (-1):
                            masked_stretch_blit(banqueImage.tile_ruine, buffer2x, 0,0, 85,85, plato[i][j].building_x-30, plato[i][j].building_y-17, 85,85);

                            break;

                        case 0:
                            masked_stretch_blit(banqueImage.tile_terrainVague, buffer2x, 0,0, 85,85, plato[i][j].building_x-30, plato[i][j].building_y-17, 85,85);
                            break;

                        case 1:
                            masked_stretch_blit(banqueImage.tile_cabane, buffer2x, 0,0, 85,85, plato[i][j].building_x-30, plato[i][j].building_y-17, 85,85);

                            break;

                        case 2:
                            masked_stretch_blit(banqueImage.tile_maison, buffer2x, 0,0, 85,85, plato[i][j].building_x-30, plato[i][j].building_y-17, 85,85);

                            break;

                        case 3:
                            masked_stretch_blit(banqueImage.tile_immeuble, buffer2x, 0,0, 85,85, plato[i][j].building_x-30, plato[i][j].building_y-17, 85,85);

                            break;

                        case 4:
                            masked_stretch_blit(banqueImage.tile_gratteCiel, buffer2x, 0,0, 85,85, plato[i][j].building_x-30, plato[i][j].building_y-35, 85,85);

                            break;

                        }
                    }
                    break;


                case 5: //chateau d'eau
                    if(plato[i][j].anchorBat) masked_stretch_blit(banqueImage.tabWaterTower[current_anim], buffer2x, 0,0, 120,63, plato[i][j].building_x-60, plato[i][j].building_y+10, 120,63);
                    break;

                case 6: //centrale
                    if(plato[i][j].anchorBat) masked_stretch_blit(banqueImage.tabCentrale[current_anim], buffer2x, 0,0, 120,102, plato[i][j].building_x-60, plato[i][j].building_y-29, 120,102);

                    break;

                }
                break;


            default: //visu eau/elec
                switch (plato[i][j].tileToDisplay)
                {
                case 4: //habitation
                    if(plato[i][j].anchorBat)
                    {
                        masked_stretch_blit(banqueImage.tile_terrainVague, buffer2x, 0,0, 85,85, plato[i][j].building_x-30, plato[i][j].building_y-17, 85,85);

                        textprintf_ex(buffer2x, font,plato[i][j].x1, plato[i][j].y1+20, makecol(255,255,255), -1, "n.%d",  partie.tabHabitations[plato[i][j].bat_ID].ID );
                        if(niveauVisu==1)
                        {

                            if(partie.tabHabitations[plato[i][j].bat_ID].in_eau<10)
                            {
                                if(partie.tabHabitations[plato[i][j].bat_ID].in_eau <tabSeuil[partie.tabHabitations[plato[i][j].bat_ID].niveau]&& current_anim<2 )
                                {
                                    textprintf_ex(buffer2x, font,plato[i][j].x1+8, plato[i][j].y1+12, makecol(255,0,0), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_eau );
                                }
                                else textprintf_ex(buffer2x, font,plato[i][j].x1+8, plato[i][j].y1+12, makecol(255,255,255), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_eau );


                            }
                            else if(partie.tabHabitations[plato[i][j].bat_ID].in_eau<100)
                            {
                                if(partie.tabHabitations[plato[i][j].bat_ID].in_eau <tabSeuil[partie.tabHabitations[plato[i][j].bat_ID].niveau]&& current_anim<2 )
                                {
                                    textprintf_ex(buffer2x, font,plato[i][j].x1+5, plato[i][j].y1+12, makecol(255,0,0), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_eau );
                                }
                                else textprintf_ex(buffer2x, font,plato[i][j].x1+5, plato[i][j].y1+12, makecol(255,255,255), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_eau );
                            }
                            else if(partie.tabHabitations[plato[i][j].bat_ID].in_eau<1000)
                            {
                                if(partie.tabHabitations[plato[i][j].bat_ID].in_eau <tabSeuil[partie.tabHabitations[plato[i][j].bat_ID].niveau]&& current_anim<2 )
                                {
                                    textprintf_ex(buffer2x, font,plato[i][j].x1+2, plato[i][j].y1+12, makecol(255,0,0), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_eau );
                                }
                                else textprintf_ex(buffer2x, font,plato[i][j].x1+2, plato[i][j].y1+12, makecol(255,255,255), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_eau );
                            }
                            else
                            {
                                if(partie.tabHabitations[plato[i][j].bat_ID].in_eau <tabSeuil[partie.tabHabitations[plato[i][j].bat_ID].niveau]&& current_anim<2 )
                                {
                                    textprintf_ex(buffer2x, font,plato[i][j].x1-3, plato[i][j].y1+12, makecol(255,0,0), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_eau );
                                }
                                else textprintf_ex(buffer2x, font,plato[i][j].x1-3, plato[i][j].y1+12, makecol(255,255,255), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_eau );
                            }
                        }
                        else
                        {
                            //mettre la vaeur en clignottant rouge si inférieure au seuil minimal du niveau de l'habitation


                            if(partie.tabHabitations[plato[i][j].bat_ID].in_elec<10)
                            {
                                if(partie.tabHabitations[plato[i][j].bat_ID].in_elec <tabSeuil[partie.tabHabitations[plato[i][j].bat_ID].niveau]&& current_anim<2 )
                                {
                                    textprintf_ex(buffer2x, font,plato[i][j].x1+8, plato[i][j].y1+12, makecol(255,0,0), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_elec );
                                }
                                else textprintf_ex(buffer2x, font,plato[i][j].x1+8, plato[i][j].y1+12, makecol(255,255,255), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_elec );


                            }
                            else if(partie.tabHabitations[plato[i][j].bat_ID].in_elec<100)
                            {
                                if(partie.tabHabitations[plato[i][j].bat_ID].in_elec <tabSeuil[partie.tabHabitations[plato[i][j].bat_ID].niveau]&& current_anim<2 )
                                {
                                    textprintf_ex(buffer2x, font,plato[i][j].x1+5, plato[i][j].y1+12, makecol(255,0,0), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_elec );
                                }
                                else textprintf_ex(buffer2x, font,plato[i][j].x1+5, plato[i][j].y1+12, makecol(255,255,255), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_elec );
                            }
                            else if(partie.tabHabitations[plato[i][j].bat_ID].in_elec<1000)
                            {
                                if(partie.tabHabitations[plato[i][j].bat_ID].in_elec <tabSeuil[partie.tabHabitations[plato[i][j].bat_ID].niveau]&& current_anim<2 )
                                {
                                    textprintf_ex(buffer2x, font,plato[i][j].x1+2, plato[i][j].y1+12, makecol(255,0,0), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_elec );
                                }
                                else textprintf_ex(buffer2x, font,plato[i][j].x1+2, plato[i][j].y1+12, makecol(255,255,255), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_elec );
                            }
                            else
                            {
                                if(partie.tabHabitations[plato[i][j].bat_ID].in_elec <tabSeuil[partie.tabHabitations[plato[i][j].bat_ID].niveau]&& current_anim<2 )
                                {
                                    textprintf_ex(buffer2x, font,plato[i][j].x1-3, plato[i][j].y1+12, makecol(255,0,0), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_elec );
                                }
                                else textprintf_ex(buffer2x, font,plato[i][j].x1-3, plato[i][j].y1+12, makecol(255,255,255), -1, "%d",  partie.tabHabitations[plato[i][j].bat_ID].in_elec );
                            }

                        }
                    }
                    break;

                case 5: //chateau d'eau
                    if(plato[i][j].anchorBat&&niveauVisu==1)
                    {
                        masked_stretch_blit(banqueImage.tile_visuWaterTower, buffer2x, 0,0, 120,63, plato[i][j].building_x-60, plato[i][j].building_y+10, 120,63);
                        textprintf_ex(buffer2x, font,plato[i+1][j+2].x1, plato[i][j+2].y1+20, makecol(255,255,255), -1, "n.%d",  partie.tabBatRessources[plato[i][j].bat_ID].ID);

                        if(partie.tabBatRessources[plato[i][j].bat_ID].exportTot<10) textprintf_ex(buffer2x, font,plato[i+1][j+2].x1+8, plato[i][j+2].y1+12, makecol(255,255,255), -1, "%d",  partie.tabBatRessources[plato[i][j].bat_ID].exportTot );
                        else if(partie.tabBatRessources[plato[i][j].bat_ID].exportTot<100) textprintf_ex(buffer2x, font,plato[i+1][j+2].x1+5, plato[i][j+2].y1+12, makecol(255,255,255), -1, "%d",  partie.tabBatRessources[plato[i][j].bat_ID].exportTot );
                        else if(partie.tabBatRessources[plato[i][j].bat_ID].exportTot<1000) textprintf_ex(buffer2x, font,plato[i+1][j+2].x1+2, plato[i][j+2].y1+12, makecol(255,255,255), -1, "%d",  partie.tabBatRessources[plato[i][j].bat_ID].exportTot );
                        else textprintf_ex(buffer2x, font,plato[i+1][j+2].x1-3, plato[i][j+2].y1+12, makecol(255,255,255), -1, "%d",  partie.tabBatRessources[plato[i][j].bat_ID].exportTot );

                    }
                    break;

                case 6: //centrale
                    if(plato[i][j].anchorBat&&niveauVisu==2)
                    {
                        masked_stretch_blit(banqueImage.tile_visuCentrale, buffer2x, 0,0, 120,63, plato[i][j].building_x-60, plato[i][j].building_y+10, 120,63);
                        textprintf_ex(buffer2x, font,plato[i+1][j+2].x1, plato[i][j+2].y1+20, makecol(0,0,0), -1, "n.%d",  partie.tabBatRessources[plato[i][j].bat_ID].ID);

                        if(partie.tabBatRessources[plato[i][j].bat_ID].exportTot<10) textprintf_ex(buffer2x, font,plato[i+1][j+2].x1+8, plato[i][j+2].y1+12, makecol(0,0,0), -1, "%d",  partie.tabBatRessources[plato[i][j].bat_ID].exportTot );
                        else if(partie.tabBatRessources[plato[i][j].bat_ID].exportTot<100) textprintf_ex(buffer2x, font,plato[i+1][j+2].x1+5, plato[i][j+2].y1+12, makecol(0,0,0), -1, "%d",  partie.tabBatRessources[plato[i][j].bat_ID].exportTot );
                        else if(partie.tabBatRessources[plato[i][j].bat_ID].exportTot<1000) textprintf_ex(buffer2x, font,plato[i+1][j+2].x1+2, plato[i][j+2].y1+12, makecol(0,0,0), -1, "%d",  partie.tabBatRessources[plato[i][j].bat_ID].exportTot );
                        else textprintf_ex(buffer2x, font,plato[i+1][j+2].x1-3, plato[i][j+2].y1+12, makecol(0,0,0), -1, "%d",  partie.tabBatRessources[plato[i][j].bat_ID].exportTot );

                    }
                    break;

                }
                break;
            }

        }
    }
}




