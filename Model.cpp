#include "Constantes.h"
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include "Model.h"

bool first =false;
bool next =true;
bool *** matrice;
pthread_barrier_t barrier;

void init(){
	matrice = new bool**[NB_MATRICE];
	
    for(int i=0;i<NB_MATRICE;i++){
        matrice[i] = new bool*[NB_MATRICE];
        
        for(int j=0;j<NB_MATRICE;j++)
            matrice[i][j] = new bool[2];
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
             delete[] matrice[i][j];
        delete[] matrice[i];
    }
    
    delete[] matrice;
}
