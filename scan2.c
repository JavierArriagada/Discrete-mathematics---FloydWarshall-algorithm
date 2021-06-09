#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define V 50
#define E 200

char ciudad[V][30], ruta[E][30], string[30];
int cityID[V], coordX[V], coordY[V];
int alturaI[100], alturaF[100];
int adyM[V][V];
FILE* fp;

int id(char* cadena){
	int ID, i;
	for(i=0;i<V;i++){
		if(strcmp(cadena, ciudad[i])) ID = i;
	}
	return ID;
}

int esCiudad(char* cadena){
	int i;	
	for(i=0; i<V; i++){ 
		strcmp(cadena, ciudad[i]);
		return 1;
	}	
	return 0;
}

int main(){

	int nodos = 0, rutas = 0;
	int i;
	while(1){
		printf("Ingrese nombre del archivo\n");
		scanf("%s", string);
		fp = fopen(string, "r");
		if(fp == NULL){ 
			printf("El archivo especificado no existe\n");
			return -1;
		}
		while(!feof(fp)){
			puts("Procesando...");
			//escaneo de ciudades
			fscanf(fp, "%s", string);
			for(i=0;i<V;i++){
				fscanf(fp, "%s", ciudad[i]);
				if(strcmp(string, "Carreteras") == 0) break;
				else {
					strcpy(ciudad[i], string);
					cityID[i] = i;
					fscanf(fp, "%d %d", &coordX[i], &coordY[i]);
				}
				nodos = i + 1;
			}
			//escaneo de carreteras
			for(i=0;i<E;i++){
				int m, n;
				//primer string de la linea
				fscanf(fp, "%s", ruta[i]);
				//segundo string de la linea
				fscanf(fp, "%s", string);
				if(feof(fp)) break;
				//si el 2do string es ciudad, se obtiene su id
				else if(esCiudad(string)){
					m = id(string);
					//tercer string...
					fscanf(fp, "%s", string);
					//si el 3ro es ciudad, se obtiene su id y se guarda el costo en la matriz de adyacencia
					if(esCiudad(string)){
						n = id(string);
						fscanf(fp, "%d", &adyM[m][n]);
					}
					//si es ruta....
					else {
						strcpy(ciudad[nodos-1+i], string);   //se crea nodo de interseccion
						cityID[nodos-1+i] = nodos-1+i;   //se asigna una id
						n = id(string);  //se obtiene su id
						fscanf(fp, "%d %d", &alturaI[i], &adyM[m][n]); //se guarda costo en la matriz
						adyM[n][m]=adyM[m][n]; //el costo de ida es igual al de vuelta
					}
			   	}
				//si el 2do string es una carretera.....AQUI CAGO XDDDDD......
				else {
					strcpy(ciudad[nodos-1+i], string);   //se crea nodo de interseccion
					cityID[nodos-1+i] = nodos-1+i;   //se asigna una id
					m = id(string);  //se obtiene su id
					strcpy(ciudad[nodos+i], string);   //se crea nodo de interseccion
					cityID[nodos+i] = nodos+i;   //se asigna una id
					n = id(string);  //se obtiene su id
					fscanf(fp, "%d", &adyM[m][n])
				}
				
			}
			
		}
	}

return 0;
}
