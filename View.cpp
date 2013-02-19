#include <unistd.h>
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Model.h"

extern short int first;
extern short int next;
extern bool ***matrice;

using namespace sf;

void affichageShell(int cpt){
	int i,j;
	printf("------Step %d ------\n",cpt);
	for(i=1;i<NB_MATRICE-1;i++){
		for(j=1;j<NB_MATRICE-1;j++){
			if(matrice[i][j][first] == 1)
				printf("x ");
			else
				printf("o ");
			}
		printf("\n");
	}
}

void affichage(RenderWindow &App){
	int i,j;
	sf::Shape fond = Shape::Rectangle(0,0,(NB_MATRICE-2)*multiple, (NB_MATRICE-2)*multiple, Color(0,0,0,10));
	sf::Shape pix  = sf::Shape::Rectangle(0,0,multiple,multiple,sf::Color(0, 0, 255));
    App.Draw(fond);
	for(i=1;i<NB_MATRICE-1;i++){

            for(j=1;j<NB_MATRICE-1;j++){
                if(matrice[i][j][first] == 1)
                    App.Draw(pix);
                pix.Move(multiple,0);
			}
			pix.Move(-(NB_MATRICE-2)*multiple,multiple);

	}
	    App.Display();
}
