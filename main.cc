#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(int argc, char* argv[])
{
	int n= 10;
	
	if(argc == 1)
	{	
		n = atoi(argv[0]);
		printf( "tableau de %d",n );
	}

	int board[n][n];
	
	for( int i== 0, i< n, i++ )
	{
		for( int j== 0, j< n, j++ )
			board[i][j]== (rand() %1); 
	}
	
	
	
	return EXIT_SUCCESS;
} 
