
//DESCRIPCION: Función que se encarga de la creación y la inicializacion de las hebras asignando las casillas en las que realizan la funcion
//ENTRADA: una lista con las hebras que inicializar, el numero de hebras, la matriz en la que se trabajará, el tamano de la matriz y la cantidad de iteraciones de corte
//SALIDA: no posee retorno
void crearHebras(pthread_t threads[], int numeroHebras, float **H, int N, int t)
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
	int columna=0;
	//ASIGNAR LAS CASILLAS A LAS HEBRAS
	while(i < numeroHebras)
	{
		hebra *thread_data;
		thread_data = malloc(sizeof(hebra));
		thread_data->tid = i;
		if(impar==1 && i==(numeroHebras-1)) elementosPorHebra = elementosUltimaHebra;
		thread_data->elementosPorHebra = elementosPorHebra;
		thread_data->coordenadas=(coordenada*)malloc(sizeof(int)*elementosPorHebra*2);
		thread_data->matrixSize = N;
		thread_data->tActual = t;

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
				j--;
				fila++;
			}
		}
		threads_data[i]=thread_data;
		i++;
	}
	getNextMatrix(threads, threads_data,H, numeroHebras, N);
}

//DESCRIPCION: funcion que genera la matriz con las condiciones de borde iniciales
//ENTRADA: EL tamano de que debe poseer la matriz cuadrada
//SALIDA: una matriz de flotantes con las condiciones iniciales
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

			if(i==0 || i==N-1 || j==0 || j==N-1) Hnew[i][j]=0;
		}
	}
	return Hnew;
}


//DESCRIPCION: funcion que se encarga de encontrar el valor a una casilla dentro de la matriz para cualquier iteracion
//ENTRADA: la pos x de la casilla, la posy de la casilla , el tamano de la matriz, la iteracion que se esta realizando
//SALIDA: retorna un flotante con el valor posicionado en la casilla
float schrodEq(int x, int y, int N, int t)
{
	
	float lower = 0;
	float upper = 0;
	float left = 0;
	float right = 0;
	float value = 0;

	if(x!=0)	upper = H_t_1[x-1][y];
	if(x!=N-1)	lower = H_t_1[x+1][y];
	if(y!=0)	left = H_t_1[x][y-1];
	if(y!=N-1)	right = H_t_1[x][y+1];



	if (t==0)
	{
		if(x==0 || x== N-1 || y==0 || y==N-1)
		{
			value = 0;

		}
		else
		{
			float aux = ((c*c)/2)*(dt/dd)*(dt/dd);
			value = H_t_1[x][y]+aux*(right+lower+upper+left-4*H_t_1[x][y]);
		}
	}
	else
	{
		if(x==0 || x== N-1 || y==0 || y==N-1)
		{
			value = 0;
		}
		else
		{
			float aux = (c)*(dt/dd);
			aux = aux*aux;
			value = 2*H_t_1[x][y]-H_t_2[x][y]+aux*(right+lower+upper+left-4*H_t_1[x][y]);
		}
	}
	
	//float value = 5;


	return value;
}

//DESCRIPCION: funcion que permite entrar a la seccion critica bloqueando las casillas que son necesarias para realizar los calculos
//ENTRADA: la pos x de la casilla central a bloquear, la posy de la casilla central a bloquear y el tamano de la matriz a considerar
//SALIDA: no posee retorno
void enterSC(int x, int y, int N)
{

	if(x!=0)	while (pthread_mutex_trylock(&mutex[x-1][y])!=0);
	if(y!=0)	while (pthread_mutex_trylock(&mutex[x][y-1])!=0);
				while (pthread_mutex_trylock(&mutex[x][y])!=0);
	if(y!=N-1)	while (pthread_mutex_trylock(&mutex[x][y+1])!=0);
	if(x!=N-1)	while (pthread_mutex_trylock(&mutex[x+1][y])!=0);
}

//DESCRIPCION: funcion que libera las casillas bloqueadas de la seccion critica
//ENTRADA: la pos x de la casilla central a desbloquear, la posy de la casilla central a desbloquear y el tamano de la matriz a considerar
//SALIDA: no posee retorno
void exitSC(int x, int y, int N)
{
	if(x!=0)	while (pthread_mutex_unlock(&mutex[x-1][y])!=0);
	if(y!=0)	while (pthread_mutex_unlock(&mutex[x][y-1])!=0);
				while (pthread_mutex_unlock(&mutex[x][y])!=0);
	if(y!=N-1)	while (pthread_mutex_unlock(&mutex[x][y+1])!=0);
	if(x!=N-1)	while (pthread_mutex_unlock(&mutex[x+1][y])!=0);
}

//DESCRIPCION: funcion principal que aplica el algoritmo a las hebras
//ENTRADA: los argumentos provenientes de la creacion de hebras
//SALIDA: no posee retorno

void *applySchrod(void *arg1)
{
	
	hebra *thread_data = (hebra *) arg1;

//	printf("Soy la hebra %i", (int) thread_data->tid);
//	printf(", tengo %i coordenadas\n", thread_data->elementosPorHebra);

	int i = 0;
	while(i<thread_data->elementosPorHebra)
	{
		int x = thread_data->coordenadas[i].posX;
		int y = thread_data->coordenadas[i].posY;
		int N = thread_data->matrixSize;
		int t = thread_data->tActual;
//		printf("hebra: %i x: %i, y: %i\n",(int) thread_data->tid,x,y);

		enterSC(x, y, N);
		//SC
		float value = schrodEq(x, y, N, t);
		H[x][y] = value;
		//FIN SC
		exitSC(x, y, N);
		i++;
	}

	
}

//DESCRIPCION: funcion que genera la siguiente matriz calculando los valores asignados a las casillas para cada hebra
//ENTRADA: las hebras a ejecutar, la matriz previa , la cantidad de herbas y el tamano de la matriz
//SALIDA: no posee retorno
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

//DESCRIPCION: funcion que muestra por consola una matriz
//ENTRADA: la matriz a mostrar por consola y la dimension que esta posee
//SALIDA: no posee retorno
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

//DESCRIPCION: funcion que escribe en un archivo de texto una matriz
//ENTRADA: la amtriz a escribir, el nombre del archivo de salida y la dimension que esta posee
//SALIDA: no posee retorno
void fprintMatrix(float **H,char *salida, int N){
	int i,j;
	if (salida !=NULL){
		printf("voy a escribir archivo de salida\n");
		FILE *archivoSalida;
		archivoSalida=fopen(salida,"wb");
		printf("voy a escribir archivo de salida2\n");
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