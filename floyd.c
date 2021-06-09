#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define Inf 1000
#define LEN 50
int n;
void floyd(int);
int d_rec();
int ady[LEN][LEN];//matriz de adyacencia con las distancias
int d[LEN][LEN][LEN];//matriz para las operaciones de Floyd
int rec[LEN][LEN];//matriz con las referencias de las ciudades y 'ciudades auxiliares'
int sum_rec=0;
int ady2[LEN][LEN];

typedef enum {
	CiudadCiudad, 
	InterseccionInterseccion, 
	CiudadInterseccion,  
	InterseccionCiudad,
}tipoDeCarretera;

typedef struct{
	char ciudad[LEN];
	int x;
	int y;
	int num;
}ciudades;

typedef struct {
	char nombre[LEN];
	char origen[LEN];
	char destino[LEN];
	int kmOrigen;
	int kmDestino;
	int longitud;
	tipoDeCarretera tipo;
} camino;



FILE *file;
ciudades city[LEN];
camino carreteras[200];
char* tokens[6];
int tokensContador = 0;
int carreterasContador;
int ciudadesCnt;
char linea[LEN];
int isNum();


int isNum(char* str){
	int i;
	for(i = 0; i < strlen(str); i++){
		if(isdigit(str[i]) == 0)
			return 0;
	}
	return 1;
}

void inicializar(){
	ciudadesCnt=0;
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
	if(strcmp(tmp,linea) != 0){
		if(n<LEN){
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
		fgets(linea, LEN, file);
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
int newPeso[LEN][LEN];
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
			if(ady[i][j] == Inf){
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
				if(strcmp(carreteras[i-1].origen,city[j-1].ciudad)==0){
					if(strcmp(carreteras[i-1].destino,city[k-1].ciudad)==0){
						ady[j][k] = carreteras[i-1].longitud;
						ady[k][j] = carreteras[i-1].longitud;
					}
				}
			}
		}
	}

}

int distancia(int a,int b){
	int dist =(int)sqrt(pow((city[b].x - city[a].x),2) + pow((city[b].y - city[a].y),2)); 
	return dist;
}

void CaminoMasCorto(int vis[]){
	int i,j,dist_min=Inf,inicio,termino;
	
	for(i=1;i<=n;i++){
	printf("%d ",vis[i]);
	}printf("\n");
	for(i=1;i<=n;i++){
		if(vis[i]==1){
			for(j=1;j<=n;j++){
				if(vis[j]==0){
					 //printf("i:%d j:%d dist:%d\n",i,j,distancia(i-1,j-1));
					if(distancia(i-1,j-1) < dist_min){
					
						dist_min = distancia(i-1,j-1);
						printf("   %d\n",dist_min);
						inicio = i;
						termino = j;
					}
				}
			}
		}
	}
	ady[inicio][termino]=dist_min;
	ady[termino][inicio]=dist_min;
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

int main(){

char archivo[100],respuesta;

	while(1){
		
		printf("Ingrese nombre del archivo que desea leer:\n");  
		scanf("%s", archivo);
		file = fopen(archivo,"r");
		if(file != NULL){
			if(archivo != NULL){				
				
				inicializar();
				n=contarCiudad(file);
				guardarCiudad(file,n);
				guardarCamino();
				newCamino(n);
				adyM();
				
	int i,j,k,l;
	
	for(i=1;i<=n;i++){
		f=0;r=-1;
		for(j=1;j<=n;j++){
  			q[j]=0;
			visited[j]=0;
		}		
		bfs(i);
		CaminoMasCorto(visited);
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
	for(i=1;i<=n;i++){ //para imprimir las matrices
		for(j=1;j<=n;j++){
			printf("%d\t", rec[i][j]);
		}
		puts("\n");		
	}
	
int sum = 0, suma = Inf, i_save;
for(i=1;i<=n;i++){
	for(j=1;j<=n;j++){
		sum_rec=0;
		d_rec(i,j);
		sum += sum_rec;
		printf("%d->%d:%d\n", i,j,sum_rec);
	}
	
	if(sum < suma){	
		suma = sum;
		i_save=i;
	}
	sum = 0;
}

		printf("%s %s\n", "El punto de reparto debe estar en la ciudad :",city[i_save-1].ciudad);
			}
			else printf("Archivo vacio\n");
		}
		else printf("Archivo no existe\n");
		
		volver:
		
		printf("¿Desea leer otro archivo? S/N: ");
		scanf("%c",&respuesta);
		
		if(respuesta == 'N'){
			break;
		}
		else if(respuesta != 'S'){
			printf("Respuesta inválida\n");
			goto volver;
		}
	}
	

return 0;		
}
///////////////////////////MINIMO//////////////////////////
int min(int x, int y){
	if( x<y ) return x;
	else return y;
}

////////////////////FLOYD//////////////////////////////////
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
		
				d[k][i][j]=min(d[k-1][i][j], d[k-1][i][k] + d[k-1][k][j]);
				
				if(d[k-1][i][k] + d[k-1][k][j] < d[k-1][i][j]){
				
					rec[i][j] = k;
				
				}
   	   }
   	}   	
	}
	for(k=0;k<=v;k++){
		printf(" k=%d \n",k);
			for(i=1;i<=v;i++){
				printf("\n");
			for(j=1;j<=v;j++)
				printf("\t %d",d[k][i][j]);
			}
	    printf("\n \n ");
	  } 
	}
/////////////////////////DISTANCIA MINIMA AL VERTICE///////

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

