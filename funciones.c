
//DESCRIPCION:
//ENTRADA:
//SALIDA:

void crearHebras(pthread_t threads[], int numeroHebras, float **H, int N)
{
	
	hebra **threads_data;
	threads_data = malloc(sizeof(hebra)*numeroHebras);
	int j;
	int i = 0;
	int impar=0;
	int elementosPorHebra=0;
	int elementosUltimaHebra;
	int cantidadElementos=N*N;
	
	//Asignación memoria mutex global
	mutex=(pthread_mutex_t **)malloc(sizeof(pthread_mutex_t*)*N);
	for (int i = 0; i <N; ++i){
		mutex[i]=(pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*N);
		for (int j = 0; j < N; ++j){

			pthread_mutex_init(&mutex[i][j], NULL);			
		}
	}


	//Todas las hebras trabajan la misma cantidad de elementos
	if (cantidadElementos%numeroHebras==0){
		elementosPorHebra=cantidadElementos/numeroHebras;
	}

	//Las hebras trabajan cantidades distintas
	else{
		impar=1;
		elementosPorHebra=cantidadElementos/numeroHebras;
		elementosUltimaHebra = elementosPorHebra + cantidadElementos%numeroHebras;
	}
	int fila=0;

	//ASIGNAR LAS CASILLAS A LAS HEBRAS
	while(i < numeroHebras)
	{
		int columna=0;
		hebra *thread_data;
		thread_data = malloc(sizeof(hebra));
		thread_data->tid = i;
		if(impar==1 && i==(numeroHebras-1)) elementosPorHebra = elementosUltimaHebra;
		thread_data->elementosPorHebra = elementosPorHebra;
		thread_data->coordenadas=(coordenada*)malloc(sizeof(char)*elementosPorHebra);
		thread_data->mutexHilo = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t)*elementosPorHebra);

		//printf("Hebra %d \n",(int)thread_data->tid );
		//printf("elementosPorHebra dentro del while %d\n", elementosPorHebra);
		//Asginar las posiciones de las casillas de la matriz a cada hebra (EJemplo parte desde la posicion inicial hasta el final)
		for (j = 0; j < elementosPorHebra; ++j)
		{
			if (columna<N)
			{
				thread_data->coordenadas[j].posX=fila;
				thread_data->coordenadas[j].posY=columna;
				//printf("%d) posX: %d posY: %d\n",j,thread_data->coordenadas[j].posX,thread_data->coordenadas[j].posY);
				columna++;
			}
			else{
				columna=0;
				fila++;
			}
		}
		thread_data->mutexHilo = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t)*elementosPorHebra);
		threads_data[i]=thread_data;
		i++;
	}

	i = 0;
	while(i<numeroHebras)
	{
		//pthread_create(&threads[i], NULL, applyWave, (void *) threads_data[i]);
		i++;
	}

	getNextMatrix(threads, threads_data,H, numeroHebras, N);
}

//DESCRIPCION:
//ENTRADA:
//SALIDA:
float **generateMatrix(int N)
{

	int i, j;
	float **Hnew;
	Hnew = (float **)malloc(sizeof(float*)*N);
	for (i = 0; i < N; ++i){
		Hnew[i]=(float*)malloc(sizeof(float)*N);
		for (j = 0; j < N; ++j){
			Hnew[i][j] = 0;
			if(0.4*N<i && i<0.6*N)
			{
				Hnew[i][j]=20;
			}
			if(0.4*N<j && j<0.6*N)
			{
				Hnew[i][j]=20;
			}
		}
	}
	return Hnew;
}

//DESCRIPCION:
//ENTRADA:
//SALIDA:
float schrodEqOLD(float **H, int x, int y,int N)
{
	//float **Hnew = generateMatrix();
	float aux = (c)/(dt/dd);
	aux = aux*aux;
	//printf("%f\n", aux);
	float value = 2*H[x][y]-H[x][y]+aux*(H[x+1][y]+H[x-1][y]+H[x][y-1]-4*H[x][y+1]);
	//printf("%f\n", a);
	
	//Hnew[x][y] = a;

	return value;
}

//DESCRIPCION:
//ENTRADA:
//SALIDA:
float schrodEq(float **H, int x, int y, int N)
{
	float aux = (c)/(dt/dd);
	aux = aux*aux;
	
	float lower = 0;
	float upper = 0;
	float left = 0;
	float right = 0;

	if(x!=0)	upper = H[x-1][y];
	if(x!=N)	lower = H[x+1][y];
	if(y!=0)	left = H[x][y-1];
	if(y!=N)	right = H[x][y+1];

	float value = 2*H[x][y]-H[x][y]+aux*(lower+upper+left-4*right);


	return value;
}

//DESCRIPCION:
//ENTRADA:
//SALIDA:
//void applySchrod(int x, int y, float **Hold, float **Hnew, int N)
void *applySchrod(void *arg1)
{
	hebra *thread_data = (hebra *) arg1;
	//float value = schrodEq(Hold, x, y, N);
	float value = 5;
	//Hnew[x][y] = value;

	printf("Soy la hebra %i\n", (int) thread_data->tid);
}

//DESCRIPCION:
//ENTRADA:
//SALIDA:
void getNextMatrix(pthread_t threads[], hebra **threads_data, float **Hprev, int numeroHebras, int N)
{
	float **Hnext;
	int i = 0;
	while(i<numeroHebras)
	{
		pthread_create(&threads[i], NULL, applySchrod, (void *) threads_data[i]);
		i++;
	}


}


//DESCRIPCION:
//ENTRADA:
//SALIDA:
float **advance(float **H, int x, int y, int n, int N)
{

	int i=x;
	int j=y;
	float **Hnew = generateMatrix(N);
	int count = 0; // contador
	for (i; i < N; ++i){
		for (j; j < N; ++j){
			float value = schrodEq(H, x, y, N);
			value = 5;
			//if (i==0 || j==0 || i==N-1 || j==N-1) value = 0;
        	Hnew[i][j] = value;
        	printf("Estoy en (%i,%i) y ",i,j);
        	printf("count: %i\n",count);
        	count++;
        	if(count==n) break;
		}
		if(count==n) break;
	}
	return Hnew;
}

//DESCRIPCION:
//ENTRADA:
//SALIDA:
//Funcion que dado H(0) entrega H(t)
float **applyWave(float **H, int t,int N)
{
	float**H_t = generateMatrix(N);
	float**Haux = generateMatrix(N);
	int tActual = 0;
	while(tActual < t)
	{
		printf("Estoy en t=%i\n",tActual);
		Haux = advance(H, 0, 0, N, N);
		H = Haux;

		tActual++;
	}
	H_t = Haux;
	return H_t;
}

//DESCRIPCION:
//ENTRADA:
//SALIDA:
void printMatrix(float **H,int N)
{
	int i,j;
	for (i = 0; i < N; ++i){
		for (j = 0; j < N; ++j){
			printf("%f", H[i][j]);	
		}
		printf("\n");
	}
}

//DESCRIPCION:
//ENTRADA:
//SALIDA:
void fprintMatrix(float **H,char *salida, int N){
	int i,j;
	if (salida !=NULL){
		FILE *archivoSalida;
		archivoSalida=fopen(salida,"wb");
		for (i = 0; i < N; ++i){
			fwrite(H[i],N,sizeof(float),archivoSalida);
		}
		fclose(archivoSalida);
	}
}

//Descripcion: función que permite que se espere la ejecucion de un conjunto de hebras
//Entrada: una lista de las hebras que se desean esperar y el numero de hebras totales
//Salida: no posee retorno
void waitHebras(pthread_t threads[], int numeroHebras)
{
	int i = 0;
	while(i < numeroHebras)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}