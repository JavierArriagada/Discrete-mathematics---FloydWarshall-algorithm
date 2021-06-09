#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define Inf 1000
#define LEN 50
int n;
int ady[LEN][LEN];//matriz de adyacencia con las distancias
int d[LEN][LEN][LEN];//matriz para las operaciones de Floyd
int rec[LEN][LEN];//matriz con las referencias de las ciudades y 'ciudades auxiliares'
int sum_rec=0;
int ady2[LEN][LEN];

typedef enum {
	Ciudad_C, 
	Interseccion_I, 
	Ciudad_Interseccion,  
	Interseccion_Ciudad,
}tipoDeCarretera;

typedef struct{
	char ciudad[LEN];
	int x;
	int y;
	int num;
}ciudades;

typedef struct {
	char name[LEN];
	char source[LEN];
	char destiny[LEN];
	int kmSource;
	int kmDestiny;
	int length;
	tipoDeCarretera tipo;
} caminimumo;



FILE *file;
ciudades city[LEN];
caminimumo carreteras[200];
char* tokens[6];
int tokensCount = 0;
int highwayCount;
int cityCount;
char line[LEN];
int isNumber();


int isNumber(char* str){
	int i;
	for(i = 0; i < strlen(str); i++){
		if(isdigit(str[i]) == 0)
			return 0;
	}
	return 1;
}

int minimum(int x, int y){
	if( x<y ) return x;
	else return y;
}

void initialize(){
	cityCount=0;
	highwayCount = 0;
	tokensCount = 0;
	int i;
	for(i = 0; i < 6; i++)
		tokens[i] = NULL;
}

void saveCity(FILE *file,int n){
	rewind(file);
	char line[100];
	char tmp[] = "Ciudades";
	char *token;
	int i=0;
	fgets(line,100,file);
	int j;
	if(strcmp(tmp,line) != 0){
		if(n<LEN){
			for(j=0; j<n; j++){
				fgets(line,100,file);
				token = strtok(line," ");			
				while(token != NULL){
					strcpy(city[i].ciudad,token);
					token = strtok( NULL, " " );
					city[i].x = atoi(token);			
					token = strtok( NULL, " " );
					city[i].y = atoi(token);
					token = strtok( NULL, "\n" );
					//printf("%s %d %d\n",city[i].ciudad,city[i].x,city[i].y);
					i++;
				}	
			}
		}
		else printf("Archivo no Valido\n");
	}
	else printf("Archivo no valido\n");
}

void loadHighwayCityCity(){
	strcpy(carreteras[highwayCount].name, tokens[0]);
	strcpy(carreteras[highwayCount].source, tokens[1]);
	strcpy(carreteras[highwayCount].destiny, tokens[2]);
	carreteras[highwayCount].length = atoi(tokens[3]);
	carreteras[highwayCount].tipo = Ciudad_C;
}

void loadHighwayCityInter(){
	//interseccion -> Ciudad
	if(isNumber(tokens[2]) == 1){
		strcpy(carreteras[highwayCount].name, tokens[0]);
		strcpy(carreteras[highwayCount].source, tokens[1]);
		carreteras[highwayCount].kmSource = atoi(tokens[2]);
		strcpy(carreteras[highwayCount].destiny, tokens[3]);
		carreteras[highwayCount].length = atoi(tokens[4]);
		carreteras[highwayCount].tipo = Interseccion_Ciudad;
	}
	//ciudadad -> interseccion
	else{
		strcpy(carreteras[highwayCount].name, tokens[0]);
		strcpy(carreteras[highwayCount].source, tokens[1]);
		strcpy(carreteras[highwayCount].destiny, tokens[2]);
		carreteras[highwayCount].kmDestiny = atoi(tokens[3]);
		carreteras[highwayCount].length = atoi(tokens[4]);
		carreteras[highwayCount].tipo = Ciudad_Interseccion;
	}
	
}

void loadHighwayInterInter(){
	strcpy(carreteras[highwayCount].name, tokens[0]);
	strcpy(carreteras[highwayCount].source, tokens[1]);
	carreteras[highwayCount].kmSource = atoi(tokens[2]);
	strcpy(carreteras[highwayCount].destiny, tokens[3]);
	carreteras[highwayCount].kmDestiny = atoi(tokens[4]);
	carreteras[highwayCount].length = atoi(tokens[5]);
	carreteras[highwayCount].tipo = Interseccion_I;
}

void guardarCaminimumo(){
	while(!feof(file)){
		fgets(line, LEN, file);
		int i;
		for(i = 0; i < 6; i++){
			tokens[i] = strtok(i == 0 ? line : NULL, " ");
			if(tokens[i] == NULL)
				break;
		}
		tokensCount = i;
		if(tokensCount == 4)
			loadHighwayCityCity();
		else if(tokensCount == 5)
			loadHighwayCityInter();
		else if(tokensCount == 6)
			loadHighwayInterInter();
		else
			continue;
		highwayCount++;
	}
}

int countCity(FILE *file){
	int i;
	char tmp[]="Carreteras";
	char line[100];
	char *token;
	for(i=0;i<50;i++){
		fgets(line,100,file);	
		token = strtok(line,"\n");
		while(token != NULL){
			token = strtok(NULL,"\n");
		}
		if(strcmp(tmp,line) == 0){			
			return i-1;
		}
	}
	return i;	
}
void adyM(){

	int i,j,k;
	
	for(i=1;i<=n;i++){
		for(j=1;j<=n;j++){
			if(i==j){
				ady[i][j] = 0;
			}
			else{
				ady[i][j] = Inf;
			}
		}
	}
	for(i=1;i<=n;i++){
		for(j=1;j<=n;j++){
			for(k=1;k<=n;k++){
				if(strcmp(carreteras[i-1].source,city[j-1].ciudad)==0){
					if(strcmp(carreteras[i-1].destiny,city[k-1].ciudad)==0){
						ady[j][k] = carreteras[i-1].length;
						ady[k][j] = carreteras[i-1].length;
					}
				}
			}
		}
	}

}

void floyd(int v){
	int k,i,j;
	k=0;

	for(i=1;i<=v;i++){
		for(j=1;j<=v;j++){
			d[k][i][j]=ady[i][j];
		}
	}

	for(k=1;k<=v;k++){
		for(i=1;i<=v;i++){
			for(j=1;j<=v;j++){
		
				d[k][i][j]=minimum(d[k-1][i][j], d[k-1][i][k] + d[k-1][k][j]);
				
				if(d[k-1][i][k] + d[k-1][k][j] < d[k-1][i][j]){
				
					rec[i][j] = k;
				
				}
   	   }
   	}   	
	}
	/*for(k=0;k<=v;k++){
		printf(" k=%d \n",k);
			for(i=1;i<=v;i++){
				printf("\n");
			for(j=1;j<=v;j++)
				printf("\t %d",d[k][i][j]);
			}
	    printf("\n \n ");
	}*/ 
}

int distancia(int a,int b){
	int dist =(int)sqrt(pow((city[b].x - city[a].x),2) + pow((city[b].y - city[a].y),2)); 
	return dist;
}

void CaminimumoMasCorto(int vis[]){
	int i,j,dist_minimum=Inf,inicio,terminimumo;
	/*
	for(i=1;i<=n;i++){
	printf("%d ",vis[i]);
	}
	printf("\n");
	*/
	for(i=1;i<=n;i++){
		if(vis[i]==1){
			for(j=1;j<=n;j++){
				if(vis[j]==0){
					 //printf("i:%d j:%d dist:%d\n",i,j,distancia(i-1,j-1));
					if(distancia(i-1,j-1) < dist_minimum){
					
						dist_minimum = distancia(i-1,j-1);
						//printf("   %d\n",dist_minimum);
						inicio = i;
						terminimumo = j;
					}
				}
			}
		}
	}
	ady[inicio][terminimumo]=dist_minimum;
	ady[terminimumo][inicio]=dist_minimum;
}
int q[20],visited[20],f=0,r=-1;
void bfs(int v){
	int i;
	for(i=1;i<=n;i++)
		if(ady2[v][i] && !visited[i])
			q[++r]=i;
 		if(f<=r){
			visited[q[f]]=1;
			bfs(q[f++]);
		}
}


int d_rec(int x, int y){
	int i;
	if( rec[x][y] == y || rec[x][y] == x){ 
		
		sum_rec += d[n][x][y];		
	}
	else{
		for(i=1;i<=n;i++){
			if( rec[x][y] == i ){
				d_rec(i,y);
				
				d_rec(x,i);	
				break;
			}
		}
	}
	return sum_rec;
}


int main(){

char archivo[100];

	while(1){
		
		printf("Ingrese name del archivo que desea leer:\n");  
		scanf("%s", archivo);
		file = fopen(archivo,"r");
		if(file != NULL){
			if(archivo != NULL){				
				
				initialize();
				n=countCity(file);
				saveCity(file,n);
				guardarCaminimumo();
				adyM();
				
	int i,j,k,l;
	
	for(i=1;i<=n;i++){
		f=0;r=-1;
		for(j=1;j<=n;j++){
  			q[j]=0;
			visited[j]=0;
		}		
		bfs(i);
		CaminimumoMasCorto(visited);
		for(k=1;k<=n;k++){
			for(l=1;l<=n;l++){
				if(ady[k][l] == Inf){
					ady2[k][l] = 0;
				}
				else{
					ady2[k][l]=ady[k][l];
				}			
			}
		}
	}
	
	for(i=1;i<=n;i++){
		for(j=1;j<=n;j++){
			rec[j][i]=i;
		}
	}
	floyd(n);
	/*
	for(i=1;i<=n;i++){
		for(j=1;j<=n;j++){
			printf("%d\t", rec[i][j]);
		}
		puts("\n");		
	}*/
	
	int sum = 0, suma = Inf, i_save;
	
	for(i=1;i<=n;i++){
		for(j=1;j<=n;j++){
			sum_rec=0;
			d_rec(i,j);
			sum += sum_rec;
			//printf("%d->%d:%d\n", i,j,sum_rec);
		}
	
		if(sum < suma){	
			suma = sum;
			i_save=i;
		}
		sum = 0;
	}

		printf("%s %s\n", "La planta de distribución debe estar en la ciudad ",city[i_save-1].ciudad);
			}
			else{
			printf("Archivo vacio\n");
			continue;
			}
		}
		else{ 
		printf("Archivo no existe\n");
		continue;
		}
		char str_aux;
		char respuesta[50];
		volver:
		printf("¿Desea leer otro archivo? S/N: ");
		scanf("%c",&str_aux);
		scanf("%s", respuesta);

		if((respuesta[0] == 'N' && respuesta[1] == '\0')||(respuesta[0] == 'n' && respuesta[1] == '\0')){
			break;
		}
		else if((respuesta[0] == 'S' &&respuesta[1] == '\0') || (respuesta[0] == 's' && respuesta[1] == '\0')){
			continue;		
		}
		else{
			printf("Respuesta inválida\n");
			goto volver;
		}
	}
return 0;		
}





