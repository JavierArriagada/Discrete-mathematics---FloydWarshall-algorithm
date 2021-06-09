#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#define INFINITO 999

typedef enum {
	CiudadCiudad, 
	InterseccionInterseccion, 
	CiudadInterseccion,  
	InterseccionCiudad,
}tipoDeCarretera;

typedef struct{
	char ciudad[50];
	int x;
	int y;
	int num;
}ciudades;

typedef struct {
	char nombre[50];
	char origen[50];
	char destino[50];
	int kmOrigen;
	int kmDestino;
	int longitud;
	tipoDeCarretera tipo;
} camino;


FILE *file;
ciudades city[50];
camino carreteras[200];
int matrizAdyacencia[50][50];
int newPeso[50][50];
char* tokens[6];
int tokensContador = 0;
int carreterasContador;
int ciudadesCnt;
char linea[50];
int isNum();


void matriz(int n){
	int i,j=0,k,l,m,q;

	for(l=0;l<n;l++){
		for(m=0;m<n;m++){
			matrizAdyacencia[l][m]=INFINITO;
		}
	}
	for(q=0;q<n;q++){
		matrizAdyacencia[q][q]=0;
	}
	
	for(i=0;i<n;i++){
		if(strcmp(city[i].ciudad,carreteras[j].origen)==0){
			for(k=0;k<n;k++){
				if(strcmp(city[k].ciudad,carreteras[j].destino)==0){
					matrizAdyacencia[i][k]=carreteras[j].longitud;
					matrizAdyacencia[k][i]=carreteras[j].longitud;
					j++;
				}
			}					
		}
	}

	int t,w,r,y,z;
	for(t=0;t<carreterasContador;t++){
		if(carreteras[t].tipo == CiudadInterseccion){
			for(r=0;r<n;r++){
				if(strcmp(city[r].ciudad,carreteras[t].origen)==0){					
					for(w=0;w<n;w++){
						if(matrizAdyacencia[r][w] == INFINITO){
							for(y=0;y<carreterasContador;y++){
								if(strcmp(carreteras[y].nombre,carreteras[t].nombre)==0){
									for(z=0;z<carreterasContador;z++){
									if(strcmp(city[w].ciudad,carreteras[z].origen)==0){
									matrizAdyacencia[r][w]=(carreteras[t].longitud+carreteras[t].kmDestino);
									matrizAdyacencia[w][r]=(carreteras[t].longitud+carreteras[t].kmDestino);
									}else if(strcmp(city[w].ciudad,carreteras[z].destino)==0){
									matrizAdyacencia[r][w]=(carreteras[t].longitud+carreteras[t].kmOrigen);
									matrizAdyacencia[w][r]=(carreteras[t].longitud+carreteras[t].kmDestino);
									}
									}
								}
							}
						}
						/*else{
							for(y=0;y<carreterasContador;y++){
								if(strcmp(carreteras[y].nombre,carreteras[t].nombre)==0){
									for(z=0;z<carreterasContador;z++){
									if(strcmp(city[w].ciudad,carreteras[z].origen)==0){
										if(matrizAdyacencia[r][w]>(carreteras[t].longitud+carreteras[t].kmDestino)){
									matrizAdyacencia[r][w]=(carreteras[t].longitud+carreteras[t].kmDestino);
									matrizAdyacencia[w][r]=(carreteras[t].longitud+carreteras[t].kmDestino);
										}
									}
									else if(strcmp(city[w].ciudad,carreteras[z].destino)==0){
										if(matrizAdyacencia[r][w]>(carreteras[t].longitud+carreteras[t].kmOrigen)){
									matrizAdyacencia[r][w]=(carreteras[t].longitud+carreteras[t].kmOrigen);
									matrizAdyacencia[w][r]=(carreteras[t].longitud+carreteras[t].kmOrigen);
										}
									}
									}
								}
							}						
						}*/						
					}
				}
			}
		}
	}
		/*if(carreteras[t].tipo == InterseccionInterseccion){
				if(matrizAdyacencia[w][r] > (carreteras[t].longitud + carreteras[t].kmDestino + carreteras[t].kmOrigen)){
					matrizAdyacencia[w][r] = (carreteras[t].longitud + carreteras[t].kmDestino + carreteras[t].kmOrigen);
					
				}			
			}
							
		
	}
	}*/
	int g,h;

	for(g=0;g<n;g++){
		for(h=0;h<n;h++){
			printf("%d ",matrizAdyacencia[g][h]);
		}
		puts("\n");
	}
}

void newCamino(int n){
	int i,j,k,l,m,b;
	int x1,x2,y1,y2;
	
	int tmp,tmp2;

	for(b=0;b<n;b++){
		for(m=0;m<n;m++){
			newPeso[b][m]=0;
		}
	}

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(matrizAdyacencia[i][j] == INFINITO){
				for(k=0;k<n;k++){
					for(l=0;l<n;l++){
						if(city[k].num == i && city[l].num ==j){
							x1=city[k].x;
							y1=city[k].y;
							x2=city[l].x;
							y2=city[l].y;
							tmp = x2-x1;
							printf("x1 %d y1 %d tmp %d\n",x1,y1,tmp);
							tmp2 = y2-y1;
							printf("x2 %d y2 %d tmp2 %d\n",x2,y2,tmp2);
							newPeso[i][j]=sqrt(pow(tmp,2)+pow(tmp2,2));	
						}
					}
				}
			}
		}
	}
	int g,h;

	for(g=0;g<n;g++){
		for(h=0;h<n;h++){
			printf("%d ",newPeso[g][h]);
		}
		puts("\n");
	}
}


void floyd(int n){

	int k,i,j;              /*Algoritmo de Floyd Warshall*/          
	for(k=0; k<n; k++){ /*Sirve para encontrar la menor distancia entre 2 puntos, en este caso de la matriz matrizAdyacencia[n][n]*/
		for(i=0; i<n; i++){
   			for(j=0; j<n; j++){
    				if (matrizAdyacencia[i][k]+matrizAdyacencia[k][j]<matrizAdyacencia[i][j])
					matrizAdyacencia[i][j]=matrizAdyacencia[i][k]+matrizAdyacencia[k][j];
   			}
		}
	}


	int g,h;
	puts("\n");
	for(g=0;g<n;g++){
		for(h=0;h<n;h++){
			printf("%d ",matrizAdyacencia[g][h]);
		}
		puts("\n");
	}

}


int isNum(char* str){
	int i;
	for(i = 0; i < strlen(str); i++){
		if(isdigit(str[i]) == 0)
			return 0;
	}
	return 1;
}

void inicializar(){
	ciudadesCnt = 0;
	carreterasContador = 0;
	tokensContador = 0;
	int i;
	for(i = 0; i < 6; i++)
		tokens[i] = NULL;
}

void guardarCiudad(FILE *file,int n){
	rewind(file);
	char linea[100];
	char tmp[] = "Ciudades";
	char *token;
	int i=0;
	fgets(linea,100,file);
	int j;	
	printf("%d\n",n);
	if(strcmp(tmp,linea) != 0){
		if(n<50){
			for(j=0; j<n; j++){
				fgets(linea,100,file);
				token = strtok(linea," ");			
				while(token != NULL){
					strcpy(city[i].ciudad,token);
					token = strtok( NULL, " " );
					city[i].x = atoi(token);			
					token = strtok( NULL, " " );
					city[i].y = atoi(token);
					token = strtok( NULL, "\n" );
					printf("%s %d %d\n",city[i].ciudad,city[i].x,city[i].y);
					i++;
				}	
			}
		}
		else printf("Archivo no Valido\n");
	}
	else printf("Archivo no valido\n");
}

void cargarCarreteraCiudadCiudad(){
	strcpy(carreteras[carreterasContador].nombre, tokens[0]);
	strcpy(carreteras[carreterasContador].origen, tokens[1]);
	strcpy(carreteras[carreterasContador].destino, tokens[2]);
	carreteras[carreterasContador].longitud = atoi(tokens[3]);
	carreteras[carreterasContador].tipo = CiudadCiudad;
}

void cargarCarreteraCiudadInterseccion(){
	//interseccion -> Ciudad
	if(isNum(tokens[2]) == 1){
		strcpy(carreteras[carreterasContador].nombre, tokens[0]);
		strcpy(carreteras[carreterasContador].origen, tokens[1]);
		carreteras[carreterasContador].kmOrigen = atoi(tokens[2]);
		strcpy(carreteras[carreterasContador].destino, tokens[3]);
		carreteras[carreterasContador].longitud = atoi(tokens[4]);
		carreteras[carreterasContador].tipo = InterseccionCiudad;
	}
	//ciudadad -> interseccion
	else{
		strcpy(carreteras[carreterasContador].nombre, tokens[0]);
		strcpy(carreteras[carreterasContador].origen, tokens[1]);
		strcpy(carreteras[carreterasContador].destino, tokens[2]);
		carreteras[carreterasContador].kmDestino = atoi(tokens[3]);
		carreteras[carreterasContador].longitud = atoi(tokens[4]);
		carreteras[carreterasContador].tipo = CiudadInterseccion;
	}
}

void cargarCarreteraInterseccionInterseccion(){
	strcpy(carreteras[carreterasContador].nombre, tokens[0]);
	strcpy(carreteras[carreterasContador].origen, tokens[1]);
	carreteras[carreterasContador].kmOrigen = atoi(tokens[2]);
	strcpy(carreteras[carreterasContador].destino, tokens[3]);
	carreteras[carreterasContador].kmDestino = atoi(tokens[4]);
	carreteras[carreterasContador].longitud = atoi(tokens[5]);
	carreteras[carreterasContador].tipo = InterseccionInterseccion;
}

void guardarCamino(){
	while(!feof(file)){
		fgets(linea,50, file);
		int i;
		for(i = 0; i < 6; i++){
			tokens[i] = strtok(i == 0 ? linea : NULL, " ");
			if(tokens[i] == NULL)
				break;
		}
		tokensContador = i;
		if(tokensContador == 4)
			cargarCarreteraCiudadCiudad();
		else if(tokensContador == 5)
			cargarCarreteraCiudadInterseccion();
		else if(tokensContador == 6)
			cargarCarreteraInterseccionInterseccion();
		else
			continue;	
		carreterasContador++;
	}
}

int contarCiudad(FILE *file){
	int i;
	char tmp[]="Carreteras";
	char linea[100];
	char *token;
	for(i=0;i<50;i++){
		fgets(linea,100,file);	
		token = strtok(linea,"\n");
		while(token != NULL){
			token = strtok(NULL,"\n");
		}
		if(strcmp(tmp,linea) == 0){			
			return i-1;
		}
	}
	return i;	
}
/*
void imprimir(){
	puts("Ciudades");
	int j;
	for(j = 0; j < ciudadesCnt; j++){
		printf("%d %s %d %d\n",city[j].num, city[j].ciudad, city[j].x, city[j].y);
	}
	puts("Carreteras");
	int i;
	for(i = 0; i < carreterasContador; i++){
		switch(carreteras[i].tipo){
			case CiudadCiudad:{
				printf("%s %s %s %d\n", carreteras[i].nombre, carreteras[i].origen, carreteras[i].destino, carreteras[i].longitud);
				break;
			}
			case CiudadInterseccion:{
				printf("%s %s %s %d %d\n", carreteras[i].nombre, carreteras[i].origen, carreteras[i].destino, carreteras[i].kmDestino, carreteras[i].longitud);
				break;
			}
			case InterseccionCiudad:{
				printf("%s %s %d %s %d\n", carreteras[i].nombre, carreteras[i].origen, carreteras[i].kmOrigen, carreteras[i].destino, carreteras[i].longitud);
				break;
			}
			case InterseccionInterseccion:{
				printf("%s %s %d %s %d %d\n", carreteras[i].nombre, carreteras[i].origen, carreteras[i].kmOrigen, carreteras[i].destino, carreteras[i].kmDestino, carreteras[i].longitud);
				break;
			}
		}
	}
}
*/
int main(){
	int n;
	char archivo[100];
	char exit[]="salir";
	while(1){
		printf("Ingrese nombre del archivo que desea leer o 'salir' si desea cerrar el programa \n");  
		scanf("%s", archivo);
		if(strncmp(exit,archivo,5)!=0){
			file = fopen(archivo,"r");
			if(file == NULL){
				printf("Fichero no existe o no se puede abrir\n");
			}
			else{
				fseek( file, 0, SEEK_END );
				if(ftell(file) !=0){
					rewind(file);
					inicializar();
					n=contarCiudad(file);
					guardarCiudad(file,n);					
					guardarCamino();
					matriz(n);
					newCamino(n);
					floyd(n);
					//imprimir();	
				}
				else printf("Archivo vacio\n");
			}
		}
		else break;
	}
return 0;
}
