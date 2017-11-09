#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>
#include "funciones.h"
#include "funciones.c"




int main(int argc, char **argv)
{

	char *nameArchivo = NULL;
	int N = 0;
	int numeroHebras=0;
	int t=0;
	int T=0;
	int d;


	opterr = 0;
	//N: tamaño grilla, T: numero de pasos, H: numero de hebras,
	//f: archivo salida, t:iteraciones 
	while ((d = getopt (argc, argv, "N:T:H:f:t:")) != -1){

		switch (d){
			case 'N':
				N = atoi(optarg);
				break;
			case 't':
				t = atoi(optarg);
				break;
			case 'T':
				T = atoi(optarg);
				break;
			case 'H':
				numeroHebras = atoi(optarg);
				break;
			case 'f':
				nameArchivo = optarg;
				break;
			case '?':
				if (optopt == 'd')
					printf ( "Opción -%c requiere un argumento.\n", optopt);
				else if (isprint (optopt))
				  	printf ( "Opción desconocida `-%c'.\n", optopt);
				else
				  	printf ("Opción con caracter desconocido `\\x%x'.\n",optopt);
					return 1;
				default:
				abort ();
		}

	}

	srand((unsigned)time(NULL));		//seed para usar rand aleatorio

	H = generateMatrix(N);	// Matriz en instante t

	H_t_2 = generateMatrix(N);
	H_t_1 = generateMatrix(N);

	pthread_t threads[numeroHebras];


	printMatrix(H, N);
	int tActual = 0;
	H_t_1 = H;
	H_t_2 = H;
	while(tActual<T)
	{

		crearHebras(threads, numeroHebras, H, N, tActual);

		//El hilo main espera que terminen las hebras
		waitHebras(threads, numeroHebras);

		H_t_2 = H_t_1;
		H_t_1 = H;

		if(tActual==t)	fprintMatrix(H, "out.raw", N);

		printMatrix(H, N);
	
		tActual++;
	}

	//printMatrix(H, N);

	return 0;

}