#include<stdio.h>
#include<stdlib.h>
#include<string.h>

  char string[30];
  FILE* fp;
char *cities[50], *pathBegin[200], *pathEnd[200];
  int cityID[50]={0};
  
  int i, nodes, edges;

int esCiudad(char word[30], char *ciudades[50]){
  int i,j;
  for(i=0, j=0; i<50, j<30;i++, j++){
    word[j]=string[j];
    ciudades[i]=cities[i];
  }
  for(i = 0; i < 50; i++){
    if(!strcmp(string, ciudades[i])) return 1;
  }
  return 0;
}

int main(){
  int *pathID[200]={0};
  int *pathLen[200]={0};
  int coordX[100], coordY[100], coordI[100], coordF[100];
    while(1){
    printf("Ingrese nombre del archivo\n");
    scanf("%s", string);
    fp = fopen(string, "r");
    if(fp == NULL){
      printf("El archivo especificado no existe\n");
      return -1;
    }
    while(!feof(fp)){
      printf("Leyendo...\n");
      fscanf(fp, "%s", string);
      for(i = 0; i < 50; i++){
	nodes = i;
	fscanf(fp, "%s", string);
	if(strcmp(string, "Carreteras") == 0) break;
	else{
	  strcpy(cities[i], string);
	  cityID[i]=i+1;
	  fscanf(fp, "%d %d", &coordX[i], &coordY[i]);
	}
      }
      for(i = 0; i < 200; i++){
	edges = i;
	fscanf(fp, "%s", path[i]);
	fscanf(fp, "%s", string);
	if(feof(fp)) break;
	else if(esCiudad(string, cities)){
	  strcpy(pathBegin[i], string);
	  fscanf(fp, "%s", string);
	  if(esCiudad(string, cities)){
	    strcpy(pathEnd[i], string);
	    fscanf(fp, "%d", pathLen[i]);
	  }
	  else {
	    strcpy(pathEnd[i], string);
	    fscanf(fp, "%d %d", coordX[i], coordY[i]);
	  }
	}
	else {
	  strcpy(pathBegin[i], string);
	  fscanf(fp, "%d", coordI[i]);
	  fscanf(fp, "%s", string);
	  if(esCuidad(string, cities)){
	    strcpy(pathEnd[i], string);
	    fscanf(fp, "%d", pathLen[i]);
	  }
	  else {
	    strcpy(pathEnd[i], string);
	    fscanf(fp, "%d %d", coordF[i], pathLen[i]);
	  }
	}
      }
    }
  }
}
