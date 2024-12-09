#include "header.h"


int main()
{
    initAllegro(); //on initialise tout le nécessaire en rapport avec allegro

    t_cityData partie; //on initialise la structure qui stocke les informations de jeu

    t_image banqueImages; //on intialise la structure contenant les images

    banqueImages = loadBanqueImage();//on la remplie

    t_polices banquePolices; //on initialse la structure contenant les polices

    //on la remplie
    banquePolices= loadBanquePolices();


    t_music banqueSon; //on initialse la structure contenant les sons

    banqueSon= loadBanqueSon(); //on la remplie

     while(!key[KEY_ESC])
    {
         //Sous prog permettant de lancer le menu
         mainMenu(partie, banqueImages, banquePolices, banqueSon);
    }


    return 0;
}
END_OF_MAIN()
