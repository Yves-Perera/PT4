

#include <unistd.h>
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Model.h"
#include "Control.h"
#include "View.h"

using namespace sf;





int main(int argc, char**argv)
{
    VideoMode VMode((NB_MATRICE-2)*multiple, (NB_MATRICE-2)*multiple, 32);
    RenderWindow App(VMode, "Jeu de la Vie");
	
	int cpt = 0;

  pthread_t tid[NB_THREADS];
  int i,j;
	init();

  
	tore();

App.Clear();

  while(App.IsOpened()){
	    
         sf::Event Event;
        while (App.GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                App.Close();
        }
	  tore();
	  //affichageShell(cpt);
	  affichage(App);
	  
	  for(i=0;i<NB_THREADS;i++)
	  {
		int ret = pthread_create(&tid[i], NULL, f_thread, (void*)i);
		if(ret) {
		  fprintf(stderr, "Impossible de créer le thread %d\n", i);
		  return 1;
		}
	  }
	  void *ret_val;

	  for(i=0;i<NB_THREADS;i++)
	  {
		int ret = pthread_join(tid[i], &ret_val);
		if(ret) {
		  fprintf(stderr, "Erreur lors de l'attente du thread %d\n", i);
		  return 1;
		}
	  }

	  cpt++;

	  if(first == 0){
		first = 1;
		next = 0;
	}
	  else{
		first = 0 ;
		next = 1;
	}
	sleep(vitesse);
  }
  
    for(i=0;i<NB_MATRICE;i++){
        for(j=0;j<NB_MATRICE;j++){
            delete[] matrice[i][j];
        }
        delete[] matrice[i];
    }
    delete matrice;
    
  return 0;
}
