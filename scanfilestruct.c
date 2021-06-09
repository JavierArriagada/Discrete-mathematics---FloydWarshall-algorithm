#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Ciudad
{
	char nombre[30];
	int coordenada_x;
	int coordenada_y;
} Ciudad;
typedef struct Carreteras
{
	char nombre[30];
	char inicio[30];
	char termino[30];
	int coor_inicio;
	int coor_termino;
	int largo;
} Carretera;


// Funcion que retorna 1 si "nombre" es el nombre de una ciudad. Retorna cero en otro caso
int es_nombre_de_ciudad(char* nombre, Ciudad* ciudades)
{
	int i;
	for(i=0; i<50; i++)
	{
		if(!strcmp(nombre, ciudades[i].nombre))
		{
			return 1;
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	char buffer[30];
	char c;
	FILE* fp;

	Ciudad ciudades[50];
	Carretera carreteras[300];
	int i,totalciudades=0,totalcarreteras=0;
	while(1)
	{
		printf("Ingrese el nombre del archivo\n");
		// Se lee el nombre del archivo
		scanf("%s", buffer);
		fp = fopen(buffer, "r");
		if (fp==NULL)
		{
			printf("El archivo '%s' no existe\n", buffer);
			return -1;
		}
		while(!feof(fp) ){
			printf("El archivo esta abierto\n");
			fscanf(fp, "%s", buffer); // En esta linea se lee la palabra "Ciudades"
			for(i = 0;i<50;i++){
				totalciudades=i;
				fscanf(fp, "%s", buffer);
				// strcmp retorna cero si las strings son iguales
				if (strcmp(buffer, "Carreteras")==0)
					break; // entonces si son iguales, salimos del ciclo y comenzamos a leer carreteras
				else
				{
					// Si no, se copia la palabra leida al nombre de la i-esima ciudad
					strcpy(ciudades[i].nombre, buffer);
					fscanf(fp, "%d %d", &ciudades[i].coordenada_x, &ciudades[i].coordenada_y);
				}
			}
			// Este es el scaneo de las carreteras
			i = 0;
			for(i=0;i<300;i++){
				totalcarreteras=i;
				fscanf(fp,"%s",carreteras[i].nombre);
				fscanf(fp,"%s", buffer);
				if(feof(fp))break;
				// Acá hay que hacer distinciones, mi idea acá es ver si el segundo string de la linea es el nombre de una ciudad.
				// Si coincide, entonces la carreteras empieza en una ciudad.
				else if (es_nombre_de_ciudad(buffer, ciudades))
				{
					strcpy(carreteras[i].inicio, buffer);
					fscanf(fp, "%s", buffer);
					// Luego analizamos el string sgte, si este es el nombre de una ciudad, entonces copiamos ese nombre al string de termino
					// de la carretera y luego scaneamos el largo de esta.
					if(es_nombre_de_ciudad(buffer, ciudades)){
						strcpy(carreteras[i].termino, buffer);
						fscanf(fp, "%d", &carreteras[i].largo);
					}
					// En caso que sea el nombre de una carretera, copiamos ese nombre al string de termino, luego scaneamos la coordenada
					// de termino y finalmente el largo de la carretera.
					else {
						strcpy(carreteras[i].termino, buffer);
						fscanf(fp, "%d %d", &carreteras[i].coor_termino, &carreteras[i].largo);
					}
				}
				// Ahora analizamos el caso de que si el primer string es el nombre de una carretera. copiamos el nombre de esta ruta al string
				// de inicio y posteriormente scaneamos la coordenada de inicio
				else {
					strcpy(carreteras[i].inicio , buffer);
					fscanf(fp, "%d", &carreteras[i].coor_inicio);
					// Luego analizamos el termino de la carretera e imitamos lo que hicimos anteriormente, si termina en una ruta, copiamos
					// el nombre y la coordenada de termino, si acaba en ciudad, copiamos el nombre y el largo de la carretera.
					fscanf(fp, "%s", buffer);
					if(es_nombre_de_ciudad(buffer, ciudades)) {
						strcpy(carreteras[i].termino, buffer);
						fscanf(fp, "%d", &carreteras[i].largo);
					}
					else{
						strcpy(carreteras[i].termino,buffer);
						fscanf(fp, "%d %d", &carreteras[i].coor_termino, &carreteras[i].largo);
					}
				}
			}
		}
		printf("Desea leer otro archivo? (S/N): ");
		while(1)
		{
			scanf("%c", &c);
			if (c=='S' || c=='N')
				break;
			else
				printf("Opcion no valida. Intentelo de nuevo: ");
		}
		if (c=='S')
			continue;
		else if (c=='N')
			break;
	}
	// Este ciclo imprime la info de todas las ciudades
	for(i = 0; i < totalciudades; i++){
		printf("%s %d %d\n", ciudades[i].nombre, ciudades[i].coordenada_x, ciudades[i].coordenada_y);
	}
	// Este ciclo imprime la info de todas las carreteras
	for(i = 0; i < totalcarreteras; i++){
		printf("%s %s %s %d %d %d\n", carreteras[i].nombre, carreteras[i].inicio, carreteras[i].termino, carreteras[i].coor_inicio, carreteras[i].coor_termino, carreteras[i].largo);
	}
	return 0;
}
