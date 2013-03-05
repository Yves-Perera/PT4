#include "Constantes.h"
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include "Model.h"

short int first =0;
short int next =1;
bool *** matrice;
short int *** voisins;
pthread_barrier_t barrier;

void init(){
	matrice = new bool**[NB_MATRICE];
	voisins = new short int **[NB_MATRICE];
	
    for(int i=0;i<NB_MATRICE;i++){
        matrice[i] = new bool*[NB_MATRICE];
        voisins[i] = new short int *[NB_MATRICE];
        
        for(int j=0;j<NB_MATRICE;j++){
            matrice[i][j] = new bool[2];
            voisins[i][j] = new short int[2];
        }
    }
    
    for(int i=0;i<NB_MATRICE;i++)
		for(int j=0;j<NB_MATRICE;j++)
			for(int k=0;k<2;k++)
				matrice[i][j][k] = rand()%2;
		
	if(pthread_barrier_init(&barrier, NULL, NB_THREADS+1))
        printf("Impossible de creer la barrier\n");


}

void deleteMatrice()
{	
    for(int i=0;i<NB_MATRICE;i++)
    {
		for(int j=0;j<NB_MATRICE;j++)
		{
             delete[] matrice[i][j];
             delete[] voisins[i][j];
        }
        delete[] matrice[i];
        delete[] voisins[i];
    }
    
    delete[] matrice;
	delete[] voisins;
}
