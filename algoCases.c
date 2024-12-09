#include "header.h"

//SSPG chargé de la répartition des ressources entre les habitations en fonction des distances
t_cityData gestionRes(t_cityData partie)
{

    int tempEau;
    int tabSeuilHaut[6]; //tableau stockant les valeur seuil de population maximal des différents niveau d'habitations
    tabSeuilHaut[0] = 1; //terrain vague
    tabSeuilHaut[1] = 1;
    tabSeuilHaut[2] = 10; //cabane
    tabSeuilHaut[3] = 50; //maison
    tabSeuilHaut[4] = 100; //immeuble
    tabSeuilHaut[5] = 1000; //gratte ciel

    /*int tabSeuilBas[6]; //tableau stockant les valeur seuil de population minimal des différents niveau d'habitations
    //tabSeuilBas[0] = 0; //terrain vague
    tabSeuilBas[2] = 1; //cabane
    tabSeuilBas[3] = 11; //maison
    tabSeuilBas[4] = 51; //immeuble
    tabSeuilBas[5] = 101; //gratte ciel

    */


    ///FAIRE UNE BOUCLE DE REMISE A ZERO DE TOUT POUR REFAIRE LE CALCUL

    for(int i=0; i<partie.ID_cursorRes; i++)
    {
        partie.tabBatRessources[i].exportTot=0;
    }
    for(int i=0; i<partie.ID_cursorHab; i++)
    {
        partie.tabHabitations[i].in_eau=0;
        partie.tabHabitations[i].in_elec=0;
    }





    for(int i=0; i< partie.ID_cursorRes; i++)//on boucle sur tout les batiments
    {
        if(partie.tabBatRessources[i].resType==1)//si c'est un chateau d'eau
        {
            //on boucle pour toutes les habitations
            for(int j=0; j<partie.tabBatRessources[i].connections; j++)
            {
                //si on a pas atteint le seuil max d'apport de l'habitation
                if(partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].in_eau<tabSeuilHaut[partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].niveau+1])
                {
                    //si il reste de l'eau à exporter
                    if(partie.tabBatRessources[i].exportTot<5000)
                    {
                        //on calcule la différence entre l'apport actuel de l'hab et son seuil max
                        tempEau= tabSeuilHaut[partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].niveau+1] - partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].in_eau;

                        //si il nous reste assez pour tout approvisionner
                        if(tempEau < (5000-partie.tabBatRessources[i].exportTot))
                        {
                            //on envoie le max jusqu'au seuil de l'hab
                            partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].in_eau = partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].in_eau +tempEau;
                            partie.tabBatRessources[i].exportTot = partie.tabBatRessources[i].exportTot + tempEau;
                        }
                        //si il ne nous reste pas assez pour tout approvisionner
                        else
                        {
                            //on envoie ce qu'il reste
                            partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].in_eau = partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].in_eau + (5000-partie.tabBatRessources[i].exportTot);
                            partie.tabBatRessources[i].exportTot =5000;
                        }
                    }
                }
            }
        }
    }
    for(int i=0; i< partie.ID_cursorRes; i++)//on boucle sur tout les batiments
    {
        if(partie.tabBatRessources[i].resType==2)//si c'est une centrale
        {
            //on boucle pour toutes les habitations
            for(int j=0; j<partie.tabBatRessources[i].connections; j++)
            {
                //si l'hab est pas déjà alimentée et si on peut l'alimenter en entier
                if(partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].in_elec==0 && partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].in_eau < (5000-partie.tabBatRessources[i].exportTot)+1 &&partie.tabBatRessources[i].exportTot <5000 )
                {
                    //on lui apporte assez pour être alimenté
                    partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].in_elec = partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].in_eau;
                    partie.tabBatRessources[i].exportTot = partie.tabBatRessources[i].exportTot + partie.tabHabitations[partie.tabBatRessources[i].tabIDSorted[j]].in_elec;
                }
            }

        }
    }


    //si on a un apport en eau et en élec, on recalcule la population
    for(int i=0; i<partie.ID_cursorHab; i++)
    {
        if(partie.tabHabitations[i].in_elec!=0 && partie.tabHabitations[i].in_eau!=0) partie.tabHabitations[i].population=partie.tabHabitations[i].in_eau;
    }


    return partie;
}


//SSPG chargé de déterminer l'évolution d'un batiment
t_cityData updateHab(t_cityData partie, int gameMode, int updateID)
{

    int tabSeuilHaut[6];
    tabSeuilHaut[0] = 1; //terrain vague
    tabSeuilHaut[1] = 1;
    tabSeuilHaut[2] = 10; //cabane
    tabSeuilHaut[3] = 50; //maison
    tabSeuilHaut[4] = 100; //immeuble
    tabSeuilHaut[5] = 1000; //gratte ciel

    int tabSeuilBas[6]; //tableau stockant les valeur seuil de population minimal des différents niveau d'habitations
    tabSeuilBas[0] = 0; //terrain vague
    tabSeuilBas[2] = 1; //cabane
    tabSeuilBas[3] = 11; //maison
    tabSeuilBas[4] = 51; //immeuble
    tabSeuilBas[5] = 101; //gratte ciel

    ///REFAIRE CALCUL DE POPULATION MAX EN CAS DE REGRESSION
    if(gameMode==1) //si on est en mode de jeu capitaliste
    {



        //si eau sous le seuil de pop ou pas d'élec
        if(partie.tabHabitations[updateID].in_eau<tabSeuilBas[partie.tabHabitations[updateID].niveau+1]||partie.tabHabitations[updateID].in_elec==0)
        {


            //on régresse
            if(partie.tabHabitations[updateID].niveau==1) partie.tabHabitations[updateID].niveau=-1; //si on régresse depuis une cabane, on va direct aux ruines
            else if(partie.tabHabitations[updateID].niveau>0) partie.tabHabitations[updateID].niveau--; //si non on baisse d'un niveau

            //on réajuste la population
            if(partie.tabHabitations[updateID].population>tabSeuilHaut[partie.tabHabitations[updateID].niveau]) partie.tabHabitations[updateID].population=tabSeuilHaut[partie.tabHabitations[updateID].niveau];
        }
        else if(partie.tabHabitations[updateID].niveau==(-1))
        {

            partie.tabHabitations[updateID].niveau=1; //si on évolue depuis les ruines, on va direct en maison
        }
        else if(partie.tabHabitations[updateID].niveau<4)
        {

            partie.tabHabitations[updateID].niveau++; //sinon on monte d'un niveau
        }




    }
    else //si on est en mode de jeu communiste
    {

        //si eau supérieur au seuil
        if(partie.tabHabitations[updateID].in_eau >=tabSeuilHaut[partie.tabHabitations[updateID].niveau+1]&&partie.tabHabitations[updateID].niveau<4&&partie.tabHabitations[updateID].in_elec!=0)
        {


            //on évolue
            if(partie.tabHabitations[updateID].niveau==(-1)) partie.tabHabitations[updateID].niveau=1; //si on évolue depuis les ruines, on va direct en maison
            else if(partie.tabHabitations[updateID].niveau<4) partie.tabHabitations[updateID].niveau++; //sinon on monte d'un niveau
        }

        else if(partie.tabHabitations[updateID].niveau>0)
        {


            //si eau sous le seuil de pop ou pas d'élec
            if(partie.tabHabitations[updateID].in_eau<tabSeuilBas[partie.tabHabitations[updateID].niveau+1]||partie.tabHabitations[updateID].in_elec==0)
            {

                //on réajuste la population
                if(partie.tabHabitations[updateID].population>tabSeuilHaut[partie.tabHabitations[updateID].niveau]) partie.tabHabitations[updateID].population=tabSeuilHaut[partie.tabHabitations[updateID].niveau];

                if(partie.tabHabitations[updateID].niveau==1) partie.tabHabitations[updateID].niveau=-1; //si on régresse depuis une cabane, on va direct aux ruines
                else if(partie.tabHabitations[updateID].niveau>0) partie.tabHabitations[updateID].niveau--; //si non on baisse d'un niveau

                //on réajuste la population
                if(partie.tabHabitations[updateID].population>tabSeuilHaut[partie.tabHabitations[updateID].niveau]) partie.tabHabitations[updateID].population=tabSeuilHaut[partie.tabHabitations[updateID].niveau];
            }
        }


        //sinon on fait rien
    }
    return partie;
}


/*
//SSPG chargé de trier le tableau de distances entre un batiment de ressources
et toutes les habitations du jeu en un tableau d'ID d'habitation trié par distance
croissante avec le bat IDbat
*/
t_cityData triTabDist(t_cityData partie, int IDbat)
{
    //on déclare un tableau dynamique à 2 dimmensions
    int **tabDistTemp=NULL;
    //[0] = distance
    //[1] = ID
    int temp1=0, temp2=0; //on déclare des variables temporaires pour le tri à bulle
    int counter=0;

    tabDistTemp=(int**) malloc ((2)*sizeof(int*)); //on alloue dynamiquement les 2 lignes du tableau

    //on alloue ensuite pour chaque ligne du tableau autant de colones qu'il y a d'habitations
    for (int i=0; i<2; i++)
    {
        tabDistTemp[i]=(int*) malloc((partie.ID_cursorHab+1)*sizeof(int));
    }


    //remplissage
    for(int j=0; j<partie.ID_cursorHab; j++)
    {
        tabDistTemp[0][j]= partie.tabBatRessources[IDbat].tabDistances[j]; //on met la distance jusqu'à l'habitation j
        tabDistTemp[1][j]=j; // on met l'ID de l'habitation stockée
    }


    //  printf("\nprint avant tri \n");
    //printf("\nBat n %d\n", IDbat);

    /*
        for(int j=0; j<partie.ID_cursorHab; j++)
        {

            printf("dist hab %d : %d\n", tabDistTemp[1][j], tabDistTemp[0][j]);
            printf("real: %d\n", partie.tabBatRessources[IDbat].tabDistances[j]);
        }*/


    //on fait un tri à bulle croissant
    for(int i=0; i<partie.ID_cursorHab; i++)
    {
        for(int j=0; j<partie.ID_cursorHab-1; j++)
        {
            if(tabDistTemp[0][j]>tabDistTemp[0][j+1]) //si l'hab j a une distance + grande jusqu'au bat que j+1
            {
                //on échange les cases
                temp1 = tabDistTemp[0][j];
                temp2 =tabDistTemp[1][j];
                tabDistTemp[0][j] = tabDistTemp[0][j+1];
                tabDistTemp[1][j] = tabDistTemp[1][j+1];
                tabDistTemp[0][j+1] = temp1;
                tabDistTemp[1][j+1] = temp2;

            }

        }
    }


    partie.tabBatRessources[IDbat].tabIDSorted =  realloc(partie.tabBatRessources[IDbat].tabIDSorted, sizeof(int)*(partie.ID_cursorHab+1));

    //on stocke ensuite les ID des batiments trié par distance croissante
    for(int i=0; i<partie.ID_cursorHab; i++)
    {
        if(tabDistTemp[0][i]>0)
        {

            partie.tabBatRessources[IDbat].tabIDSorted[counter] = tabDistTemp[1][i];
            counter++;
        }
    }
    partie.tabBatRessources[IDbat].connections=counter;
    partie.tabBatRessources[IDbat].tabIDSorted =  realloc(partie.tabBatRessources[IDbat].tabIDSorted, sizeof(int)*(counter+1));

    //on libère la mémoire allouée pour le tableau à 2 dimmensions
    for(int i=0; i<2 ; i++)
    {
        free(tabDistTemp[i]);
    }
    free(tabDistTemp);


//    printf("\n");


    return partie; //on renvoie la structure de partie entière
}



/*
//SSPG chargé de calculer la plus petite distance accessible par route
entre toutes les cases du bat IDbat et de l'habitation IDhab
*/
int calcSmallestDist( t_cityData partie, int IDbat, int IDhab)
{
    int distTemp1=1000, distTemp2=1000; //on set 2 variables temporaire pour stovker les longueurs calculées


    for(int r=0; r<24; r++) //on boucle pour chaque case du bat de ressources
    {
        if(partie.plato[partie.tabBatRessources[IDbat].tabCoord[r][0]+1][partie.tabBatRessources[IDbat].tabCoord[r][1]].tileType==3) //on teste si on a une route en x+1
        {
            for(int h=0; h<9; h++)//on boucle pour toutes les cases de l'habitation
            {


                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]+1][partie.tabHabitations[IDhab].tabCoord[h][1]].tileType==3) //on teste si on a une route en x+1
                {
                    //on calcule la distance entre les deux cases
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0]+1,partie.tabBatRessources[IDbat].tabCoord[r][1], partie.tabHabitations[IDhab].tabCoord[h][0]+1,partie.tabHabitations[IDhab].tabCoord[h][1],partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1; //si on obtient une distance plus courte que celle stockée en mémoire, on la remplace
                }
                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]-1][partie.tabHabitations[IDhab].tabCoord[h][1]].tileType==3) //on teste si on a une route en x-1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0]+1,partie.tabBatRessources[IDbat].tabCoord[r][1], partie.tabHabitations[IDhab].tabCoord[h][0]-1,partie.tabHabitations[IDhab].tabCoord[h][1],partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }
                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]][partie.tabHabitations[IDhab].tabCoord[h][1]-1].tileType==3) //on teste si on a une route en y-1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0]+1,partie.tabBatRessources[IDbat].tabCoord[r][1], partie.tabHabitations[IDhab].tabCoord[h][0],partie.tabHabitations[IDhab].tabCoord[h][1]-1,partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }
                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]][partie.tabHabitations[IDhab].tabCoord[h][1]+1].tileType==3) //on teste si on a une route en x+1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0]+1,partie.tabBatRessources[IDbat].tabCoord[r][1], partie.tabHabitations[IDhab].tabCoord[h][0],partie.tabHabitations[IDhab].tabCoord[h][1]+1,partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }

            }
        }
        if(partie.plato[partie.tabBatRessources[IDbat].tabCoord[r][0]-1][partie.tabBatRessources[IDbat].tabCoord[r][1]].tileType==3) // x-1
        {
            for(int h=0; h<9; h++)
            {

                //on recalcu le toutes les distances entre tout les batiments


                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]+1][partie.tabHabitations[IDhab].tabCoord[h][1]].tileType==3) //on teste si on a une route en x+1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0]-1,partie.tabBatRessources[IDbat].tabCoord[r][1], partie.tabHabitations[IDhab].tabCoord[h][0]+1,partie.tabHabitations[IDhab].tabCoord[h][1],partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }
                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]-1][partie.tabHabitations[IDhab].tabCoord[h][1]].tileType==3) //on teste si on a une route en x-1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0]-1,partie.tabBatRessources[IDbat].tabCoord[r][1], partie.tabHabitations[IDhab].tabCoord[h][0]-1,partie.tabHabitations[IDhab].tabCoord[h][1],partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }
                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]][partie.tabHabitations[IDhab].tabCoord[h][1]-1].tileType==3) //on teste si on a une route en y-1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0]-1,partie.tabBatRessources[IDbat].tabCoord[r][1], partie.tabHabitations[IDhab].tabCoord[h][0],partie.tabHabitations[IDhab].tabCoord[h][1]-1,partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }
                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]][partie.tabHabitations[IDhab].tabCoord[h][1]+1].tileType==3) //on teste si on a une route en x+1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0]-1,partie.tabBatRessources[IDbat].tabCoord[r][1], partie.tabHabitations[IDhab].tabCoord[h][0],partie.tabHabitations[IDhab].tabCoord[h][1]+1,partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }

            }
        }
        if(partie.plato[partie.tabBatRessources[IDbat].tabCoord[r][0]][partie.tabBatRessources[IDbat].tabCoord[r][1]-1].tileType==3) //y-1
        {
            for(int h=0; h<9; h++)
            {

                //on recalcu le toutes les distances entre tout les batiments

                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]+1][partie.tabHabitations[IDhab].tabCoord[h][1]].tileType==3) //on teste si on a une route en x+1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0],partie.tabBatRessources[IDbat].tabCoord[r][1]-1, partie.tabHabitations[IDhab].tabCoord[h][0]+1,partie.tabHabitations[IDhab].tabCoord[h][1],partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }
                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]-1][partie.tabHabitations[IDhab].tabCoord[h][1]].tileType==3) //on teste si on a une route en x-1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0],partie.tabBatRessources[IDbat].tabCoord[r][1]-1, partie.tabHabitations[IDhab].tabCoord[h][0]-1,partie.tabHabitations[IDhab].tabCoord[h][1],partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }
                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]][partie.tabHabitations[IDhab].tabCoord[h][1]-1].tileType==3) //on teste si on a une route en y-1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0],partie.tabBatRessources[IDbat].tabCoord[r][1]-1, partie.tabHabitations[IDhab].tabCoord[h][0],partie.tabHabitations[IDhab].tabCoord[h][1]-1,partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }
                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]][partie.tabHabitations[IDhab].tabCoord[h][1]+1].tileType==3) //on teste si on a une route en x+1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0],partie.tabBatRessources[IDbat].tabCoord[r][1]-1, partie.tabHabitations[IDhab].tabCoord[h][0],partie.tabHabitations[IDhab].tabCoord[h][1]+1,partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }


            }
        }
        if(partie.plato[partie.tabBatRessources[IDbat].tabCoord[r][0]][partie.tabBatRessources[IDbat].tabCoord[r][1]+1].tileType==3) //y+1
        {
            for(int h=0; h<9; h++)
            {

                //on recalcu le toutes les distances entre tout les batiments

                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]+1][partie.tabHabitations[IDhab].tabCoord[h][1]].tileType==3) //on teste si on a une route en x+1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0],partie.tabBatRessources[IDbat].tabCoord[r][1]+1, partie.tabHabitations[IDhab].tabCoord[h][0]+1,partie.tabHabitations[IDhab].tabCoord[h][1],partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }
                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]-1][partie.tabHabitations[IDhab].tabCoord[h][1]].tileType==3) //on teste si on a une route en x-1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0],partie.tabBatRessources[IDbat].tabCoord[r][1]+1, partie.tabHabitations[IDhab].tabCoord[h][0]-1,partie.tabHabitations[IDhab].tabCoord[h][1],partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }
                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]][partie.tabHabitations[IDhab].tabCoord[h][1]-1].tileType==3) //on teste si on a une route en y-1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0],partie.tabBatRessources[IDbat].tabCoord[r][1]+1, partie.tabHabitations[IDhab].tabCoord[h][0],partie.tabHabitations[IDhab].tabCoord[h][1]-1,partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }
                if(partie.plato[partie.tabHabitations[IDhab].tabCoord[h][0]][partie.tabHabitations[IDhab].tabCoord[h][1]+1].tileType==3) //on teste si on a une route en x+1
                {
                    distTemp1= longueurRoute(partie.matAdj, partie.tabBatRessources[IDbat].tabCoord[r][0],partie.tabBatRessources[IDbat].tabCoord[r][1]+1, partie.tabHabitations[IDhab].tabCoord[h][0],partie.tabHabitations[IDhab].tabCoord[h][1]+1,partie.gGraphe, partie.plato);
                    if(distTemp1<distTemp2&& distTemp1>0) distTemp2=distTemp1;
                }

            }
        }


    }

    if(distTemp2==1000) distTemp2=-2;
    return distTemp2+1;
}




Graphe graphe_brut(t_kase plateau[35][45],Graphe g)
{

    int i,j;

    int taille=0;
    int ordre=0;

    //Création du fichier
    FILE* inputFile = NULL;

    //Ouverture du fichier
    inputFile=fopen("graphe.txt","w+");

    //Blindage de l'ouverture du fichier
    if(inputFile==NULL)
    {
        printf("Erreur ouverture fichier");
        exit(0);
    }

    //Calcul de l'odre du graphe et remplissage des n° de sommets (IMPORTANT:le graphe doit donc se mettre a jour continuellement dans le prjet final

    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {
            if(plateau[i][j].tileType!=0)
            {
                plateau[i][j].sommet=ordre;
                ordre++;
            }

            else
            {
                plateau[i][j].sommet=0;

            }

        }
    }

    g.ordre=ordre;



    //On detecte les connexions des routes dans le graphe pour ecrire les aretes en .txt
    for(i=0; i<35; i++)
    {
        for (j=0; j<45; j++)
        {

            if(plateau[i][j].tileType!=0)
            {

                if(plateau[i-1][j].tileType==3 && i!=0)
                {
                    fprintf(inputFile,"%d %d ",plateau[i][j].sommet,plateau[i-1][j].sommet);
                    fprintf(inputFile,"\n");
                    taille++;

                }

                if(plateau[i+1][j].tileType==3 && i!=34)
                {
                    fprintf(inputFile,"%d %d ",plateau[i][j].sommet,plateau[i+1][j].sommet);
                    fprintf(inputFile,"\n");
                    taille++;

                }
                if(plateau[i][j-1].tileType==3&& j!=0)
                {
                    fprintf(inputFile,"%d %d ",plateau[i][j].sommet,plateau[i][j-1].sommet);
                    fprintf(inputFile,"\n");
                    taille++;
                }
                if(plateau[i][j+1].tileType==3 && j!=44)
                {
                    fprintf(inputFile,"%d %d ",plateau[i][j].sommet,plateau[i][j+1].sommet);
                    fprintf(inputFile,"\n");
                    taille++;

                }

            }

        }

    }
    g.taille=taille;
    fclose(inputFile);

    return g;
}



int** creermMatrAdj (int**tab, Graphe g)
{

    int i,j;
    FILE * ifs = fopen("graphe.txt","r");
    int s1=0, s2=0;



    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }




    //Declaration d'une tableau dynamique a double entrée pour la matrice d'adjacence
    tab=NULL;
    tab=(int**) malloc (g.ordre*sizeof(int*));

    //Allocation du tableau a deux dimensions
    for (i=0; i<g.ordre; i++)
    {
        tab[i]=(int*) malloc(g.ordre*sizeof(int));
    }
    if (tab==NULL)
    {
        printf("erreur alloc dyn");
    }

    //On initialise toute les cases du tableau a 0
    for (i=0; i<g.ordre; i++)
    {
        for (j=0; j<g.ordre; j++)
        {
            tab[i][j]=0;
        }

    }

    // créer les arêtes du graphe
    for ( i=0; i<g.taille; i++)
    {

        fscanf(ifs,"%d %d",&s1,&s2);


        //Si il existe ne arrete entre 2 sommets dans le .txt, on affecte le poids de cet arrete aux cases des sommets reliés par cet arrete dans le tableau
        tab[s1][s2]=1;




    }

    fclose(ifs);
    //On retourne notre matrice d'adjacence
    return tab;




}


int isConnected (int**matriceAdj, int x_or,int y_or,int x_dest, int y_dest, Graphe gr,t_kase plateau[35][45])
{
    int infi =9999;
    int sommetD=plateau[x_or][y_or].sommet;
    int fin=plateau[x_dest][y_dest].sommet;

    // printf("Sommet debut:%d\n",sommetD);
    //printf("Sommet fin:%d\n",fin);


    //On declare nos variables: un tableau dynamique a double entrée et 3 tableaux dynamique simple
    int tot,mindistance,suivant,i,j;

    //Initialisation des tableaux et allocations
    int**coutA=NULL;
    int* distance=NULL;
    int* pred=NULL;
    int* visite=NULL;

    visite=(int*) malloc(gr.ordre*sizeof(int));

    pred=(int*) malloc(gr.ordre*sizeof(int));

    distance=(int*) malloc(gr.ordre*sizeof(int));

    coutA = (int**) malloc(gr.ordre*sizeof(int*));

    for(i = 0; i < gr.ordre; i++)
    {
        coutA[i] = (int*) malloc(gr.ordre*sizeof(int));
    }

    //pred contient les predecesseurs des sommets du graphe
    //Tot donne le nombre total de sommets visités
    //Distance  stock le poids pour chaque arrete par laquelle on passe
    //Visite stock les sommets visités au cours de la recherche de chemin
    //On initialise notre matrice d'adjacence avec les poids
    for(i=0; i<gr.ordre; i++)
    {
        for(j=0; j<gr.ordre; j++)
        {
            //Si il n'y pas d'arretes reliants les sommets, on met son poids comme infini
            if(matriceAdj[i][j]==0)
            {
                coutA[i][j]=infi;
            }
            //Sinon on met le poids stocké dans notre matrice pour ces 2 sommets
            else
            {
                coutA[i][j]=matriceAdj[i][j];
            }
        }

    }


    //On initialise notre tableau de ditance et de predecesseurs
    for(i=0; i<gr.ordre; i++)
    {
        distance[i]=coutA[sommetD][i];
        pred[i]=sommetD;
        visite[i]=0;
    }

    distance[sommetD]=0;
    visite[sommetD]=1;
    tot=1;
    //Tant que l'on a pas visité tout les sommets du graphe
    while(tot<gr.ordre-1)
    {
        mindistance=infi;

        //suivant donne le sommet a la plus courte distance du sommet en cours d'exploration
        for(i=0; i<gr.ordre; i++)
        {

            if(distance[i]<mindistance &&!visite[i])
            {
                mindistance=distance[i];
                suivant=i;
            }
        }
        //On verifie si il y a un meilleur chemin qui existe au sommet suivant
        visite[suivant]=1;
        for(i=0; i<gr.ordre; i++)
            if(!visite[i])
            {

                if(mindistance+coutA[suivant][i]<distance[i])
                {
                    distance[i]=mindistance+coutA[suivant][i];
                    pred[i]=suivant;
                }
            }

        tot++;
    }



    //On retourne 1 ou 0 en fonction de si les cases sont co

    for(i=0; i<gr.ordre; i++)
    {
        if(i!=sommetD && i==fin && distance[i]!=infi)
        {
            //On libere l'espace memoire alloué pour nos tableaux dynamiques

            for (j=0; j<gr.ordre; j++)
            {
                free(coutA[j]);
            }


            free(coutA);
            free(distance);
            free(pred);
            free(visite);
            return 1;

        }

        else if (i!=sommetD && i==fin && distance[i]==infi)
        {
            //On libere l'espace memoire alloué pour nos tableaux dynamiques

            for (j=0; j<gr.ordre; j++)
            {
                free(coutA[j]);
            }


            free(coutA);
            free(distance);
            free(pred);
            free(visite);

            return 0;

        }

    }

    //Erreur du Dijkistra
    return -1;





}

//SSPG chargé de calculer la plus petite distance accessible par route entre 2 cases données
int longueurRoute (int**matriceAdj, int x_or,int y_or,int x_dest, int y_dest, Graphe gr,t_kase plateau[35][45])
{
    int infi =9999;
    int sommetD=plateau[x_or][y_or].sommet;
    int fin=plateau[x_dest][y_dest].sommet;
    // printf("Sommet debut:%d\n",sommetD);
    // printf("Sommet fin:%d\n",fin);

    //On declare nos variables: un tableau dynamique a double entrée et 3 tableaux dynamique simple
    int tot,mindistance,suivant,i,j;

    //Initialisation des tableaux et allocations
    int**coutA=NULL;
    int* distance=NULL;
    int* pred=NULL;
    int* visite=NULL;

    visite=(int*) malloc(gr.ordre*sizeof(int));

    pred=(int*) malloc(gr.ordre*sizeof(int));

    distance=(int*) malloc(gr.ordre*sizeof(int));

    coutA = (int**) malloc(gr.ordre*sizeof(int*));

    for(i = 0; i < gr.ordre; i++)
    {
        coutA[i] = (int*) malloc(gr.ordre*sizeof(int));
    }

    //pred contient les predecesseurs des sommets du graphe
    //Tot donne le nombre total de sommets visités
    //Distance  stock le poids pour chaque arrete par laquelle on passe
    //Visite stock les sommets visités au cours de la recherche de chemin
    //On initialise notre matrice d'adjacence avec les poids
    for(i=0; i<gr.ordre; i++)
    {
        for(j=0; j<gr.ordre; j++)
        {
            //Si il n'y pas d'arretes reliants les sommets, on met son poids comme infini
            if(matriceAdj[i][j]==0)
            {
                coutA[i][j]=infi;
            }
            //Sinon on met le poids stocké dans notre matrice pour ces 2 sommets
            else
            {
                coutA[i][j]=matriceAdj[i][j];
            }
        }

    }


    //On initialise notre tableau de ditance et de predecesseurs
    for(i=0; i<gr.ordre; i++)
    {
        distance[i]=coutA[sommetD][i];
        pred[i]=sommetD;
        visite[i]=0;
    }

    distance[sommetD]=0;
    visite[sommetD]=1;
    tot=1;
    //Tant que l'on a pas visité tout les sommets du graphe
    while(tot<gr.ordre-1)
    {
        mindistance=infi;

        //suivant donne le sommet a la plus courte distance du sommet en cours d'exploration
        for(i=0; i<gr.ordre; i++)
        {

            if(distance[i]<mindistance &&!visite[i])
            {
                mindistance=distance[i];
                suivant=i;
            }
        }
        //On verifie si il y a un meilleur chemin qui existe au sommet suivant
        visite[suivant]=1;
        for(i=0; i<gr.ordre; i++)
            if(!visite[i])
            {

                if(mindistance+coutA[suivant][i]<distance[i])
                {
                    distance[i]=mindistance+coutA[suivant][i];
                    pred[i]=suivant;
                }
            }

        tot++;
    }



    //On affiche le chemin entre les 2 sommets choisis (debut et fin)

    for(i=0; i<gr.ordre; i++)
    {
        if(i!=sommetD && i==fin && distance[i]!=infi)
        {
            //On libere l'espace memoire alloué pour nos tableaux dynamiques

            for (j=0; j<gr.ordre; j++)
            {
                free(coutA[j]);
            }


            free(coutA);
            free(pred);
            free(visite);
            return distance[i];

        }

        else if (i!=sommetD && i==fin && distance[i]==infi)
        {
            //On libere l'espace memoire alloué pour nos tableaux dynamiques

            for (j=0; j<gr.ordre; j++)
            {
                free(coutA[j]);
            }


            free(coutA);
            free(distance);
            free(pred);
            free(visite);
            return 0;

        }

    }
    //Erreur du dijkistra
    return -1;





}


//SSPG chargé de détecter l'orientation de la route en regardant les cases adjacentes
int detectOrientRoute (int x, int y, t_kase tab[35][45])
{


    if (tab[x-1][y].tileType==3 && tab[x+1][y].tileType==3 && tab[x][y-1].tileType!=3 &&tab[x][y+1].tileType!=3)
    {
        return 2; //Pour une route verticale simple (connectée en bas et en haut )
    }

    if (tab[x][y-1].tileType==3 && tab[x][y+1].tileType==3 && tab[x-1][y].tileType!=3 &&tab[x+1][y].tileType!=3)
    {
        return 1; //Route horizontale simple (connectée a droite et a gauche )

    }

    if (tab[x+1][y].tileType==3 && tab[x][y+1].tileType==3 && tab[x-1][y].tileType!=3 && tab[x][y-1].tileType!=3)
    {
        return 3; //Virage a droite (connectée en  bas et a droite)

    }

    if (tab[x-1][y].tileType==3 && tab[x][y+1].tileType==3 && tab[x+1][y].tileType!=3 &&tab[x][y-1].tileType!=3)
    {
        return 6; //Virage a droite (connectée en  haut et a droite)

    }


    if (tab[x-1][y].tileType==3 && tab[x][y-1].tileType==3 && tab[x+1][y].tileType!=3 &&tab[x][y+1].tileType!=3)
    {
        return 5; //Virage a gauche (connectée en  haut et a gauche)

    }

    if (tab[x+1][y].tileType==3 && tab[x][y-1].tileType==3 &&tab[x-1][y].tileType!=3 &&tab[x][y+1].tileType!=3)
    {
        return 4; //Virage a gauche (connectée en  bas et a gauche)

    }

    if (tab[x-1][y].tileType==3 && tab[x][y+1].tileType==3 && tab[x][y-1].tileType==3 && tab[x+1][y].tileType!=3)
    {
        return 9; //carefour triple (connectée en  haut  a droite et a gauche)

    }

    if (tab[x+1][y].tileType==3 && tab[x][y+1].tileType==3 && tab[x][y-1].tileType==3 && tab[x-1][y].tileType!=3)
    {
        return 10; //carefour triple (connectée en  bas  a droite et a gauche)

    }

    if (tab[x-1][y].tileType==3 && tab[x][y+1].tileType==3 && tab[x+1][y].tileType==3 && tab[x][y-1].tileType!=3)
    {
        return 8; //carefour triple (connectée en  haut  en bas et a droite)

    }

    if (tab[x-1][y].tileType==3 && tab[x][y-1].tileType==3 && tab[x+1][y].tileType==3 && tab[x][y+1].tileType!=3)
    {
        return 7; //carefour triple (connectée en  haut  en bas et a gauche)

    }


    if (tab[x-1][y].tileType==3 && tab[x+1][y].tileType==3 && tab[x][y-1].tileType==3 && tab[x][y+1].tileType==3)
    {
        return 11; //carefour quadruple (connectée en  haut  a droite et a gauche)

    }

    if((tab[x-1][y].tileType==3 && tab[x+1][y].tileType!=3 && tab[x][y-1].tileType!=3 && tab[x][y+1].tileType!=3)||(tab[x-1][y].tileType!=3 && tab[x+1][y].tileType==3 && tab[x][y-1].tileType!=3 && tab[x][y+1].tileType!=3))
    {
        return 2;
    }
    if((tab[x-1][y].tileType!=3 && tab[x+1][y].tileType!=3 && tab[x][y-1].tileType==3 && tab[x][y+1].tileType!=3)||(tab[x-1][y].tileType!=3 && tab[x+1][y].tileType!=3 && tab[x][y-1].tileType!=3 && tab[x][y+1].tileType==3))
    {
        return 1;
    }

    else return 1;
}


