#include <unistd.h>
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Constantes.h"
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

void affichage(RenderWindow &App)
{
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

void *f_affichage(void *arg, RenderWindow &App)
{
	
  int id = (int&)arg;
  
  int x_min,x_max,i,j;
	x_min = (NB_MATRICE/NB_THREADS)*id;
	x_max = (NB_MATRICE/NB_THREADS)*(id+1);
	if(id == 0)
		x_min +=1;
	if(id == NB_THREADS-1)
		x_max -=1;
		
	sf::Shape pix  = sf::Shape::Rectangle(0,0,multiple,multiple,sf::Color(0, 0, 255));
	
	for(i=x_min; i<x_max; i++)
	{
		for(j=1; j<NB_MATRICE-1; j++)
		{
            if(matrice[i][j][first] == 1)
				App.Draw(pix);
			pix.Move(multiple,0);
		}
		pix.Move(-(NB_MATRICE-2)*multiple,multiple);
	}

}
/*
void affichageThread(RenderWindow &App)
{
	pthread_t tid[NB_THREADS];
	sf::Shape fond = Shape::Rectangle(0,0,(NB_MATRICE-2)*multiple, (NB_MATRICE-2)*multiple, Color(0,0,0,10));
	App.Draw(fond);
	for(int i=0;i<NB_THREADS;i++)
	  {
		int ret = pthread_create(&tid[i], NULL, f_affichage, (void*)i, App);
		if(ret) {
		  fprintf(stderr, "Impossible de créer le thread %d\n", i);
		  return 1;
		}
	  }
	  void *ret_val;

	  for(int i=0;i<NB_THREADS;i++)
	  {
		int ret = pthread_join(tid[i], &ret_val);
		if(ret) {
		  fprintf(stderr, "Erreur lors de l'attente du thread %d\n", i);
		  return 1;
		}
	  }
	App.Display();
	
}
*/
