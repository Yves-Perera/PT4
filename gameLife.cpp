#include <unistd.h>
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <semaphore.h>
#include "Constantes.h"
#include "Model.h"
#include "Control.h"
#include "View.h"
using namespace sf;

int main(int argc, char**argv)
{
	timeval tv;
	double deb, final;
	
	#ifdef affichage
		VideoMode VMode((NB_MATRICE-2)*multiple, (NB_MATRICE-2)*multiple, 32);
		RenderWindow App(VMode, "Jeu de la Vie");
		App.Clear();
	#endif

  pthread_t tid[NB_THREADS];
  int i,j;
  
    gettimeofday(&tv,NULL);
	deb = (double)((double)tv.tv_sec*1000000 + (double)(tv.tv_usec));
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
	
	for(int i =0 ; i < LOOP;i++)//while(cpt < 1000) //App.IsOpened())
	{
		rc = pthread_barrier_wait(&barrier);
		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
		{
			printf("Impossible d'attendre la barrier\n");
			exit(-1);
		}
		
		
		tore();

		if(first == 0){
			first = 1;
			next = 0;
		}
		else{
			first = 0 ;
			next = 1;
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
	}
	gettimeofday(&tv,NULL);
	final =(double)((double)tv.tv_sec*1000000 + (double)(tv.tv_usec) - deb);
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
	  printf("Matrice de %d sur %d\n",NB_MATRICE,NB_MATRICE);
	  printf("Pour %d iterations\n", LOOP);
	  printf("temps execution = %.6f\n",((double)final/1000000));
	return EXIT_SUCCESS;
}
