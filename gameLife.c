#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>

#define NB_MATRICE 22
#define NB_THREADS 4

short int first =0;
short int next =1;

short int matrice[NB_MATRICE][NB_MATRICE][2];
short int matrice[NB_MATRICE][NB_MATRICE][2];
  
void *f_thread(void *arg)
{
  int quartier=(int)arg;
  //printf("Bonjour, je suis le thread %d\n", quartier);
  nextStep(quartier);
  return (void*)quartier;
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
void nextStep(int quartier){
	int x_min,x_max,i,j;
	x_min = (NB_MATRICE/NB_THREADS)*quartier;
	x_max = (NB_MATRICE/NB_THREADS)*(quartier+1);
	
	for(i=x_min; i<x_max; i++)
		for(j=1; j<NB_MATRICE-1; j++)
			calculeCellule(i,j);
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

void affichageVoisin(int cpt){
	int i,j;
	printf("------Step %d ------\n",cpt);
	for(i=1;i<NB_MATRICE-1;i++){
		for(j=1;j<NB_MATRICE-1;j++){
			printf("%d ",nbCelluleVoisine(i,j));
			}
		printf("\n");
	}
}
int main(int argc, char**argv)
{
	int cpt = 0;
  //int pthread_mutex_init( pthread_mutex_t *mutex,const pthread_mutexattr_t *mutexattr);

  pthread_t tid[NB_THREADS];
  int i,j,k;
  
  
  for(i=0;i<NB_MATRICE;i++) 
	for(j=0;j<NB_MATRICE;j++)
		for(k=0;k<2;k++)
			matrice[i][j][k] = 0;
			
	matrice[2][2][0]=1;
	matrice[2][3][0]=1;
	matrice[2][4][0]=1;
		
  while(cpt < 10){
	  tore();
	  affichage(cpt);
	  //affichageVoisin(cpt);
	  
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
  }
  
 
  
  
  //int pthread_mutex_destroy(pthread_mutex_t *mutex);
  
  return 0;
}
