/*
	Erick Sánchez
	Prueba Tecnica
	29 de Septiembre de 2016
	
	Debian GNU/Linux 8 (jessie) 64-bit
	
	Ejecutar archivo con el archivo de referencia como Argumento. Ejemplo:
	./a.out apocalipsis.txt
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int n = 63878, P=0,q=0;

int comparaCad(char* cad, char* indice[P]);
void print(char* indice[P]);
int getIndice(char* cad, char* indice[P]);
void indexPalabra(char* cad, char* indice[P]);
int ** createMatrix();
void destroyMatrix(int **grafo);
int llenarMatriz(int **grafo, char* cad, char* indice[P]);
int mayor(int a, int b);
void predict(int **grafo, int num, int j, char* indice[P]);

int main(int argc, char *argv[]){
	int i = -1,j;
	char cad[n];
	char* indice[n];
	int **grafo;
	
	FILE * apocalipsis;
	apocalipsis = fopen(argv[1],"r");
	
	while(feof(apocalipsis) == 0){
		i++;
		cad[i] = fgetc(apocalipsis);
	}
	fclose(apocalipsis);
	
	indexPalabra(cad,indice);
	if((grafo = (int **) malloc (q * sizeof(int*))) == NULL)
		return -1;
	for ( i = 0; i < q; i++ ) {
		if ( ( grafo[i] = (int *) malloc ( q * sizeof(int) ) ) == NULL )
			return -1;
	}
	if(( grafo = createMatrix()) == NULL){
		return -1;
	}
	
	for ( i = 0; i < q; i++ ) {
		for ( j = 0; j < q; j++ ) {
			grafo[i][j] = 0;
		}
	}
	//print(indice);
	j = llenarMatriz(grafo,cad,indice);
	//printf("%s\n",indice[j]);
	predict(grafo,10,j,indice);
	printf("\n");
	destroyMatrix(grafo);
	return 0;
}

int comparaCad(char* cad, char* indice[P]){//compara la cadena con cada uno de los elementos del arreglo
	int i,j=0;
	for(i=0;i<q;i++){
		if(strcmp(cad,indice[i]) == 0){
			j=1;
			break;
		}
		else j=0;
	}
	return j;
}

void print(char* indice[P]){//imprime los elementos del arreglo
	int i;
	for(i=0;i<q;i++){
		printf("%d: %s\n",i,indice[i]);
	}
}

int getIndice(char* cad, char* indice[P]){//devuelve el valor de esa cadena dentro del arreglo
	int i;
	for(i=0;i<q;i++){
		if(strcmp(cad,indice[i]) == 0){
			break;
		}
	}
	return i;
}

void indexPalabra(char* cad, char* indice[P]){//Indexamiento de palabras
	int i,a=0;
	char aux[50];
	memset(aux, '\0',50);
	for(i=0; i < strlen(cad) ; i++){
		aux[a] = cad[i];
		if(cad[i]==32 || cad[i] == 10){
			a=-1;
			memset(aux, '\0',50);
		}
		else if(cad[i+1]==32 || cad[i+1] == 10){
			if(comparaCad(aux,indice) == 0){
				indice[q] = strdup(aux);
				q++;
			}
			a=-1;
			memset(aux, '\0',50);
			P++;
		}
		a++;
	}
}

int ** createMatrix(){//crea la matriz para el grafo
	int i,j;
	int **A;
	if(q < 0)	return NULL;
	if ( ( A = (int **) malloc ( q * sizeof(int *) ) ) == NULL )
   		return NULL;
   	
   	for ( i = 0; i < q; i++ ) {
		if ( ( A[i] = (int *) malloc ( q * sizeof(int) ) ) == NULL )
			return NULL;
		for ( j = 0; j < q; j++)
			A[i][j] = 0;
	}
	return A;
}

void destroyMatrix(int **grafo){//libera memoria asignada a el grafo
	int i;
	for ( i = 0; i < q; i++ ) {
		free( grafo[i] );
		grafo[i] = NULL;
	}
	free( grafo );
	grafo = NULL;
}

int llenarMatriz(int **grafo, char* cad, char* indice[P]){
	int i,j,m,x,y,a=0;
	char aux[50];
	memset(aux, '\0',50);
	
	for ( x = 0; x < q; x++ ) {
		for ( y = 0; y < q; y++ ) {
			grafo[x][y] = 0;
		}
	}
	
	for(i=0; i < strlen(cad) ; i++){
		aux[a] = cad[i];
		if(cad[i+1]==32 || cad[i+1] == 10){
			m = i;
			m++;
			i = getIndice(aux,indice);
			a=-1;
			memset(aux, '\0',50);
			break;
		}
		a++;
	}
	while(cad[m] == 32 || cad[m] == 10){
		a = 0;
		memset(aux, '\0',50);
		m++;
	} 
	while(strlen(cad) > m){
		aux[a] = cad[m];
		if(cad[m]==32 || cad[m] == 10){
			a=-1;
			memset(aux, '\0',50);
		}
		else if(cad[m+1]==32 || cad[m+1] == 10){
			j = getIndice(aux,indice);
			grafo[i][j]++;
			i=j;
			a=-1;
			memset(aux, '\0',50);
		}
		a++;
		m++;
	}
	return j;
}

void predict(int **grafo, int num, int j, char* indice[P]){
	int i=0,x=0,vj[10000];
	srand(time(NULL));
	vj[0] = i;
	for(i = 1; i < q; i++){
		if(grafo[j][vj[0]] < grafo[j][i]){
			memset(vj, '\0',10000);
			x=0;
			vj[0] = i;
		}
		else if(grafo[j][vj[0]] == grafo[j][i]){
			vj[x] = i;
			x++;
		}
	}
	if(num > 0){
		if(x>0){
			x = rand()%(x);
			printf("%s ",indice[vj[x]]);
			grafo[j][vj[x]]++;
			predict(grafo,num-1,vj[x],indice);
		}
		else{
			printf("%s ",indice[vj[x]]);
			grafo[j][vj[x]]++;
			predict(grafo,num-1,vj[x],indice);
		}
	}
	else{
		if(x>0){
			x = rand()%(x);
			printf("%s ",indice[vj[x]]);
			grafo[j][vj[x]]++;
		}
		else
			printf("%s ",indice[vj[x]]);
			grafo[j][vj[x]]++;
	}
}
