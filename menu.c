#include "header.h"

//SSPG chargé de l'affichage du menu principal ainsi que de lancer les autres SSPG en fonction du choix du joueur
void mainMenu(t_cityData partie, t_image banqueImages, t_polices banquePolices, t_music banqueSon )
{
    int choixMenu=0;

    int colorBlack = makecol(0,0,0);
    int colorWhite = makecol(255,255,255);
    int colorClicked = makecol(4, 45, 148);


    ///----------------CHARGEMENT DES RESSOURCES--------------------///
    //images
    BITMAP * buffer2x;
    buffer2x=create_bitmap(SCREEN_W,SCREEN_H);


    play_sample(banqueSon.menu, 255,128,1000,1);

    ///#################################################     BOUCLE D'AFFICHAGE      ##############################################################///
    while(choixMenu==0)
    {
        clear_bitmap(buffer2x);

        blit(banqueImages.Fond_Menu, buffer2x, 0,0,0,0,SCREEN_W,SCREEN_H);


        ///-------------------------------------AFFICHAGE TITRE DU JEU----------------------------------------------///

        textout_centre_ex(buffer2x, banquePolices.Kabel_28, "ECE", SCREEN_W/6-25+6, SCREEN_H/10+2, colorBlack, -1 );
        textout_centre_ex(buffer2x, banquePolices.Kabel_28, "City", SCREEN_W/6+25+6, SCREEN_H/10+75+2, colorBlack, -1 );

        textout_centre_ex(buffer2x, banquePolices.Kabel_28, "ECE", SCREEN_W/6-25, SCREEN_H/10, colorWhite, -1 );
        textout_centre_ex(buffer2x, banquePolices.Kabel_28, "City", SCREEN_W/6+25, SCREEN_H/10+75, makecol(250,218,94), -1 );



        ///-------------------------------------AFFICHAGE CASES CHOIX MENU----------------------------------------------///

        for(int i=0; i<4; i++) //on boucle pour chaque case du tableau
        {
            rectfill(buffer2x, 50+50*i, 235+100*i, 400+50*i, 300+100*i, makecol(171, 224, 215));//on dessine une case bleu ciel

            //bords noir des cases
            rectfill(buffer2x, 45+50*i,230+100*i, 405+50*i, 235+100*i, colorBlack); //haut de la case
            rectfill(buffer2x, 45+50*i,230+100*i, 50+50*i, 305+100*i, colorBlack); //gauche de la case
            rectfill(buffer2x, 400+50*i, 230+100*i, 405+50*i, 305+100*i, colorBlack); //droit de la case
            rectfill(buffer2x, 45+50*i, 300+100*i, 405+50*i, 305+100*i, colorBlack); //bas de la case


            if((mouse_x > 50+50*i)&&(mouse_x<400+50*i)&&(mouse_y>235+100*i)&&(mouse_y<300+100*i)) //si le joueur passe son curseur sur une case
            {
                rectfill(buffer2x, 50+50*i, 235+100*i, 400+50*i, 300+100*i, makecol(34, 101, 140));//on affiche la case ensurbrillance bleu gris

                if(mouse_b==1)//si il clique sur la case
                {
                    choixMenu=i+1; //on set le choix correspondant
                    rectfill(buffer2x, 50+50*i, 235+100*i, 400+50*i, 300+100*i, colorClicked); //on réaffiche la case en bleu foncé

                }

            }

        }

        ///----------------------------------AFFICHAGE OPTIONS MENU----------------------------------///


        textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Nouvelle Partie", 224-1, 234+2, colorBlack, -1);
        textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Charger Partie", 274-1, 334+2, colorBlack, -1);
        textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Aide", 324-1, 434+2, colorBlack, -1);
        textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Quitter", 374-1, 535+2, colorBlack, -1);

        //on affiche les textes des différentes options du menu
        textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Nouvelle Partie", 225, 235, makecol(95, 137, 140), -1);
        textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Charger Partie", 275, 335, makecol(95, 137, 140), -1);
        textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Aide", 325, 435, makecol(95, 137, 140), -1);
        textout_centre_ex(buffer2x, banquePolices.Kabel_30, "Quitter", 375, 535, makecol(95, 137, 140), -1);







        blit(buffer2x, screen, 0,0,0,0, SCREEN_W,SCREEN_H);
    }

    destroy_bitmap(buffer2x);

    switch (choixMenu)
    {
    case 1:
        //on lance une nouvelle partie
        stop_sample(banqueSon.menu);
        newGame(partie, banqueImages, banquePolices, banqueSon);
        break;

    case 2:
        //charger la partie
        stop_sample(banqueSon.menu);
        loadSavedGame();
        break;
    case 3:
        //lancer l'aide
        stop_sample(banqueSon.menu);
        aide(banqueImages,banquePolices, banqueSon);
        break;

    case 4:
        stop_sample(banqueSon.menu);
        //Quitter le jeu
/*
        if(partie.ID_cursorRes!=0)
        {

            for(int i=0; i< partie.ID_cursorRes; i++)
            {
                for(int j=0; j<partie.tabBatRessources[i].connections; j++)
                {
                    free(partie.tabBatRessources[i].tabIDSorted[j]);
                }
                for(int j=0; j<partie.ID_cursorHab; j++)
                {
                    free(partie.tabBatRessources[i].tabDistances[j]);
                }


            }
        }

        for(int i=0; i<partie.ID_cursorHab; i++)
        {
            // free(partie.tabHabitations[i]);
        }

        if(partie.tabBatRessources!=NULL) free(partie.tabBatRessources);

        if(partie.tabHabitations!=NULL) free(partie.tabHabitations);*/
        exit(0);
        break;
    }

}




//SSPG chargé d'afficher l'aide au joueur
void aide (t_image banqueImages, t_polices banquePolices, t_music banqueSon)
{
    BITMAP * buffer2x;
    buffer2x=create_bitmap(SCREEN_W,SCREEN_H);
    int choixAide=0;

    t_cityData partie;

    int colorBlack = makecol(0,0,0);
    int colorClicked = makecol(4, 45, 148);





    while(choixAide==0)
    {
        clear_bitmap(buffer2x);

        blit(banqueImages.Fond_Menu, buffer2x, 0,0,0,0,SCREEN_W,SCREEN_H);




        ///-------------------------------------Affichage de la case d'aide----------------------------------------------///
        rectfill(buffer2x, 200, 100, 850, 450, makecol(171, 224, 215));//on dessine une case bleu ciel

        rectfill(buffer2x, 195,95, 855, 100, colorBlack);
        rectfill(buffer2x, 195,95, 200, 450, colorBlack);
        rectfill(buffer2x, 850,95, 855, 455, colorBlack);
        rectfill(buffer2x, 195,450, 855, 455, colorBlack);

        textout_ex(buffer2x, banquePolices.Kabel_28, "AIDE", 450-6, 35+2, colorBlack, -1 ); //Affichage du titre de la page (AIDE)
        textout_ex(buffer2x, banquePolices.Kabel_28, "AIDE", 450, 35, makecol(250,218,94), -1 ); //Affichage du titre de la page (AIDE)

        //Affichage du texte d'aide
        textout_centre_ex(buffer2x, banquePolices.System_14, "ECE City est une reprise du celebre SimCity a la sauce ECE. Vous etes le maire d'une", 520, 160, colorBlack, -1 );

        textout_centre_ex(buffer2x, banquePolices.System_14, "ville que vous devez diriger: construction d'habitations, de batiments de production, ", 520, 180, colorBlack, -1 );

        textout_centre_ex(buffer2x, banquePolices.System_14, "de routes, avec la connexion de vos systemes d'eau et d'electricite a gerer. Il y a 2", 520, 200, colorBlack, -1 );

        textout_centre_ex(buffer2x, banquePolices.System_14, "modes de jeux: communisme et capitalisme, chacun avec leur differences que nous ", 520, 220, colorBlack, -1 );

        textout_centre_ex(buffer2x, banquePolices.System_14, "vous laisserons decouvrir lors de vos parties. Le jeu se fait au clavier et a la souris,", 520, 240, colorBlack, -1 );

        textout_centre_ex(buffer2x, banquePolices.System_14, "et ne se joue qu'a un joueur.Le cycle de temps est de 15 secondes,c'est a dire que", 520,260, colorBlack, -1 );

        textout_centre_ex(buffer2x, banquePolices.System_14, "toutes les 15 secondes un mois sera passe dans le jeu. Vous avez a present toutes", 520, 280, colorBlack, -1 );

        textout_centre_ex(buffer2x, banquePolices.System_14, " les cartes en main pour reussir votre premiere partie, amusez vous bien!!", 520, 300, colorBlack, -1 );

        ///-------------------------------------AFFICHAGE CASES CHOIX MENU----------------------------------------------///

        for(int i=0; i<2; i++) //on boucle pour chaque case du tableau
        {
            rectfill(buffer2x, 650, 500+95*i, 1000, 565+95*i, makecol(171, 224, 215));//on dessine une case bleu ciel

            rectfill(buffer2x, 645,495+95*i, 1005, 500+95*i, colorBlack); //haut de la case
            rectfill(buffer2x, 645,495+95*i, 650, 565+95*i, colorBlack); //gauche de la case
            rectfill(buffer2x, 1000,495+95*i, 1005, 570+95*i, colorBlack); //droit de la case
            rectfill(buffer2x, 645,565+95*i, 1005, 570+95*i, colorBlack); //gauche de la case






            if((mouse_x > 650)&&(mouse_x<1000)&&(mouse_y>500+95*i)&&(mouse_y<565+95*i)) //si le joueur passe son curseur sur une case
            {
                rectfill(buffer2x, 650, 500+95*i, 1000, 565+95*i, makecol(34, 101, 140));//on affiche la case en surbrillance bleu gris

                if(mouse_b==1)//si il clique sur la case
                {
                    choixAide=i+1; //on set le choix correspondant
                    rectfill(buffer2x, 650, 500+95*i, 1000, 565+95*i, colorClicked); //on réaffiche la case en bleu foncé

                }

            }

        }

        ///----------------------------------AFFICHAGE OPTIONS MENU----------------------------------///


        textout_ex(buffer2x, banquePolices.Kabel_30, "Retour accueil",660-1, 515+2, colorBlack, -1);
        textout_ex(buffer2x, banquePolices.Kabel_30, "Quitter", 660-1, 610+2, colorBlack, -1);

        textout_ex(buffer2x, banquePolices.Kabel_30, "Retour accueil",660, 515, makecol(95, 137, 140), -1);
        textout_ex(buffer2x, banquePolices.Kabel_30, "Quitter", 660, 610, makecol(95, 137, 140), -1);




        blit(buffer2x, screen, 0,0,0,0, SCREEN_W,SCREEN_H);
    }

    destroy_bitmap(buffer2x);

    switch(choixAide)
    {
    case 1:
        mainMenu(partie, banqueImages, banquePolices, banqueSon );
        break;

    case 2:
        exit(0);
        break;

    }


}
