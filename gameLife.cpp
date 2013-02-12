// compilation: g++ gameLife.cpp -lpthread -lsfml-graphics

#include <unistd.h>
#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace sf;

#define NB_MATRICE 400
#define NB_THREADS 4
#define multiple 2
#define vitesse 0.2f

short int first =0;
short int next =1;


bool matrice[NB_MATRICE][NB_MATRICE][2];
void calculeCellule(int i,int j);
void nextStep(int quartier){
	int x_min,x_max,i,j;
	x_min = (NB_MATRICE/NB_THREADS)*quartier;
	x_max = (NB_MATRICE/NB_THREADS)*(quartier+1);

	for(i=x_min; i<x_max; i++)
		for(j=1; j<NB_MATRICE-1; j++)
			calculeCellule(i,j);
}

void *f_thread(void *arg)
{
  int quartier = (int&)arg;
  //printf("Bonjour, je suis le thread %d\n", quartier);
  nextStep(quartier);
  //return (void*)quartier;
}
void tore(){
	int i;
	
	for(i=1;i<NB_MATRICE-1;i++){
		matrice[i][0][first] = matrice[i][NB_MATRICE-1][first];
		matrice[i][NB_MATRICE][first] = matrice[i][1][first];

		matrice[0][i][first] = matrice[NB_MATRICE-1][i][first];
		matrice[NB_MATRICE][i][first] = matrice[1][i][first];
	}
	
}


int nbCelluleVoisine(int x, int y){
	int cpt=0,i,j;
	for(i=x-1; i<=x+1; i++)
		for(j=y-1; j<=y+1; j++)
			if(!(i==x && j==y))
				if(matrice[i][j][first] == 1)
					cpt++;
	return cpt;
}

void calculeCellule(int i,int j){
	int nbVoisins = nbCelluleVoisine(i,j);
	if( nbVoisins == 3)
		matrice[i][j][next] = 1;
	else if( nbVoisins == 2)
		matrice[i][j][next] = matrice[i][j][first];
	else if( nbVoisins < 2 || nbVoisins > 3)
		matrice[i][j][next] = 0;
}

void affichage(int cpt){
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

void affichageVoisin(RenderWindow &App){
	int i,j;
	/*printf("------Step %d ------\n",cpt);
	for(i=1;i<NB_MATRICE-1;i++){
		for(j=1;j<NB_MATRICE-1;j++){
			printf("%d ",nbCelluleVoisine(i,j));
			}
		printf("\n");
	}*/
	sf::Shape fond = Shape::Rectangle(0,0,(NB_MATRICE-2)*multiple, (NB_MATRICE-2)*multiple, Color(0,0,0,10));
	sf::Shape pix  = sf::Shape::Rectangle(0,0,multiple,multiple,sf::Color(0, 0, 255));
	//sf::Shape pixD  = sf::Shape::Rectangle(0,0,multiple,multiple,sf::Color(0, 0, 255));
    App.Draw(fond);
	for(i=1;i<NB_MATRICE-1;i++){

            for(j=1;j<NB_MATRICE-1;j++){
               // if(matrice[i][j][first] == 1 && matrice[i][j][next] == 0)
                 //   App.Draw(pixD);
                if(matrice[i][j][first] == 1)
                    App.Draw(pix);
                pix.Move(multiple,0);
                //pixD.Move(multiple,0);
			}
			pix.Move(-(NB_MATRICE-2)*multiple,multiple);
			//pixD.Move(-(NB_MATRICE-2)*multiple,multiple);

	}
	    App.Display();
}

int main(int argc, char**argv)
{
    VideoMode VMode((NB_MATRICE-2)*multiple, (NB_MATRICE-2)*multiple, 32);
    RenderWindow App(VMode, "Jeu de la Vie");
	
	int cpt = 0;

  pthread_t tid[NB_THREADS];
  int i,j,k;


  for(i=0;i<NB_MATRICE;i++)
	for(j=0;j<NB_MATRICE;j++)
		for(k=0;k<2;k++)
			matrice[i][j][k] = rand()%2;
	tore();

App.Clear();

  while(App.IsOpened()){
	    
         sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
        }
	  tore();
	  //affichage(cpt);

	  affichageVoisin(App);



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
		//printf("Le thread %d s'est terminé avec le code %d\n", i, (int)ret_val);
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
  return 0;
}
