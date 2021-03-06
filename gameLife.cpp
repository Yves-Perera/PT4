#include <unistd.h>
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <semaphore.h>
#include "Constantes.h"
#include "Model.h"
#include "Control.h"
#include "View.h"

using namespace sf;

void usage(int argc, char**argv)
{
	if(argc == 2){
		if(atoi(argv[1]) < 4 && atoi(argv[1]) > -1)
		{
			printf("Patern %d choisi\n",atoi(argv[1]));
			switch(atoi(argv[1])){
				case 0:printf("Matrice par defaut\n");
					NB_MATRICE = 250;
					NB_THREADS = 4;
					LOOP = 1000;
					NB_SEC = 2;
					break;
				case 1:printf("Matrice vide\n");
					NB_MATRICE = 250;
					NB_THREADS = 4;
					LOOP = 1000;
					NB_SEC = 2;
					break;
				case 2:printf("Matrice de test avec 1 Section\n");
					NB_MATRICE = 250;
					NB_THREADS = 4;
					LOOP = 10000;
					NB_SEC = 1;
					break;
				case 3:printf("Matrice de test avec 4 Sections\n");
					NB_MATRICE = 250;
					NB_THREADS = 4;
					LOOP = 10000;
					NB_SEC = 4;
					break;
			}
			patern = atoi(argv[1]);
		}
		else
			exit(-1);
	}
	else if(argc == 1){
		printf("Matrice par defaut\n");
		NB_MATRICE = 250;
		NB_THREADS = 4;
		LOOP = 1000;
		NB_SEC = 2;
	}
	else if(argc != 5){
		printf("Erreur nombre d'arguments\n");
		printf("Utilisation: App <Taille Matric> <Nombre de threads> <Nombre Iterations> <nombre de sections>\n");
		exit(-1);
	}
	else{
		NB_MATRICE = atoi(argv[1]);
		NB_THREADS = atoi(argv[2]);
		LOOP = atoi(argv[3]);
		if(atoi(argv[4]) < 1)
		{
			printf("Nombre de section incorrect, une section minimum\n");
			NB_SEC = 1;
		}
		else if(atoi(argv[4]) < 254)
			NB_SEC = atoi(argv[4]);
		else{
			NB_SEC = 254;
			printf("Le nombre maximum de section est 254\n");
		}
	}
}

void inversionMatrice()
{
	first = !first;
	next = !next;
}

int main(int argc, char**argv)
{
	//---Lecture des arguments---------------------------
	usage(argc,argv);
	//---------------------------------------------------
	#ifdef GRAPHIQUE
		VideoMode VMode((NB_MATRICE-2)*multiple, (NB_MATRICE-2)*multiple, 32);
		RenderWindow App(VMode, "Jeu de la Vie");
		App.Clear();
	#endif
	
	timeval tv;//chrono pour mesurer le temps d execution
	double deb,final;
	pthread_t tid[NB_THREADS];
	
	init();//g�n�ration al�atoire de la matrice
	tore();//application du tore
    gettimeofday(&tv,NULL);
	deb = (double)((double)tv.tv_sec*1000000 + (double)(tv.tv_usec));//top du chrono
	
	
	// Creation des threads
	int ret;
	for(int i=0;i<NB_THREADS;i++)
	{
		ret = pthread_create(&tid[i], NULL, f_threadStagnation, (void*)i);
		if(ret) {
			fprintf(stderr, "Impossible de cr�er le thread %d\n", i);
			return 1;
		}
	}
	int rc;
	
	//Boucle principale
	for(int i =0 ; i < LOOP;i++)
	{
		tore();
		rc = pthread_barrier_wait(&barrier);
		if(rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD){printf("Impossible d'attendre la barrier\n");exit(-1);}
		
		inversionMatrice();
		#ifdef GRAPHIQUE
			sf::Event Event;
			while (App.GetEvent(Event))
			{
				if (Event.Type == sf::Event::Closed)
					App.Close();
			}
			affichage(App);
			sleep(vitesse);
		#endif
	}
	
	gettimeofday(&tv,NULL);
	final =(double)((double)tv.tv_sec*1000000 + (double)(tv.tv_usec) - deb);//recupere le chrono
	
	//Arrete les threads
	void *ret_val;
	for(int i=0;i<NB_THREADS;i++)
	{
		int ret = pthread_join(tid[i], &ret_val);
		if(ret){fprintf(stderr, "Erreur lors de l'attente du thread %d\n", i);return 1;}
	}
	
	deleteMatrice(); //suppression de la matrice dynamiques
	
	//-------Information sur l execution-------------------------
	printf("--------------------------\n");
	printf("Effectue avec %d threads\n",NB_THREADS);
	printf("Matrice de %d sur %d\n",NB_MATRICE,NB_MATRICE);
	printf("Pour %d iterations\n", LOOP);
	printf("Avec %d sections par thread detectant la stagnation\n", NB_SEC);
	printf("temps execution = %.6f\n",((double)final/1000000));
	printf("--------------------------\n");
	//------------------------------------------------------------
	return EXIT_SUCCESS;
}
