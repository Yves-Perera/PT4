

#include <unistd.h>
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <semaphore.h>
#include "Constantes.h"
#include "Model.h"
#include "Control.h"
#include "View.h"

//commenter pour désactiver l affichage
#define affichage 

using namespace sf;

int main(int argc, char**argv)
{
	#ifdef affichage
    VideoMode VMode((NB_MATRICE-2)*multiple, (NB_MATRICE-2)*multiple, 32);
    RenderWindow App(VMode, "Jeu de la Vie");
    App.Clear();
	#endif
	
	int cpt = 0;

  pthread_t tid[NB_THREADS];
  int i,j;
	init();
	tore();
	
	// Creation des threads
	for(i=0;i<NB_THREADS;i++)
	  {
		int ret = pthread_create(&tid[i], NULL, f_thread, (void*)i);
		if(ret) {
		  fprintf(stderr, "Impossible de créer le thread %d\n", i);
		  return 1;
		}
	  }
	int rc;
	
	while(cpt < 1000) //App.IsOpened())
	{
		rc = pthread_barrier_wait(&barrier);
		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
		{
			printf("Impossible d'attendre la barrier\n");
			exit(-1);
		}
		
		#ifdef affichage
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                App.Close();
        }
		affichage(App);
		#endif
		
		tore();
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

	open=false;
	rc = pthread_barrier_wait(&barrier);
		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
		{
			printf("Impossible d'attendre la barrier\n");
			exit(-1);
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
	  
	  deleteMatrice();
	 
	return EXIT_SUCCESS;
}
