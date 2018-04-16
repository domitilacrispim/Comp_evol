#include<bits/stdc++.h>
#define tour 3
#define tama 32
#define tx_c 90

typedef struct individuo {
	int regra[128];
	double nota;
}individuo;

individuo pop[100];
individuo filhos[90];
int reticulados[100][149];
int reticu[10000][149];

individuo gera_ind ( individuo i, int num_1, int tam){
	memset(i.regra, 0, sizeof(i.regra));
	int aux;	
	while(num_1>0){
		aux=rand()%tam;
		if(i.regra[aux]==0) {
			i.regra[aux]=1;
			num_1--;	
		} 
	}
	return i;
}

individuo avalia_r2 (individuo id){
	int reticulo[149], evol[149], notas=0, aux, zeros;	
	for( int i=0; i<100; i++){
		for ( int ri=0; ri<149; ri++){
				reticulo[ri]=reticulados[i][ri];
		}
		//printf("\n");
		for(int ger=0; ger<300; ger++){
			for ( int j=0; j<149; j++){
				aux=(reticulo[(j+147)%149]*16+reticulo[(j+148)%149]*8+reticulo[j%149]*4+reticulo[(j+1)%149]*2+reticulo[(j+2)%149]*1);	
				evol[j]=id.regra[aux];
			}
			for ( int r=0; r<149; r++){
				reticulo[r]=evol[r];
			}
	
		}	
		zeros=0;
		for ( int r=0; r<149; r++){
			if(evol[r]==0) zeros++;
		}
		if((zeros==149 || zeros==0) && id.regra[31]==0 && id.regra[0]==1 ) {
			notas++;	
		}
	
	}
	id.nota=notas;
	return id;
}

individuo avalia_r2_final (individuo id){
	int reticulo[149], evol[149], notas=0, aux, zeros;	
	for( int i=0; i<10000; i++){
		for ( int ri=0; ri<149; ri++){
				reticulo[ri]=rand()%2;
		}
		for(int ger=0; ger<300; ger++){
			for ( int j=0; j<149; j++){
				aux=(reticulo[(j+147)%149]*16+reticulo[(j+148)%149]*8+reticulo[j%149]*4+reticulo[(j+1)%149]*2+reticulo[(j+2)%149]*1);	
				evol[j]=id.regra[aux];
			}
			for ( int r=0; r<149; r++){
				reticulo[r]=evol[r];
			}
	
		}	
		zeros=0;
		for ( int r=0; r<149; r++){
			if(evol[r]==0) zeros++;
		}
		if((zeros==149 || zeros==0) && id.regra[31]==0 && id.regra[0]==1 ) {
			notas++;	
		}
	
	}
	id.nota=notas;
	return id;
}


void gera_reti (){
	int y, aux1;
	double aux=1.5;
	memset ( reticulados, 0, sizeof(reticulados));
	for(int u=0; u<100; u++){
		y=aux*u;
		y=y%150;
		//printf("%d\n", y);
		while(y>0){
			aux1=rand()%150;
			if(reticulados[u][aux1]==0) {
				reticulados[u][aux1]=1;
				y--; 
			}
			//printf("%d %d\n", aux1, reticulados[u][aux1]);
		} 
		/*for ( int ri=0; ri<149; ri++){
				printf("%d", reticulados[u][ri]);
		}
		printf("\n");	*/
	}
}

individuo muta_ind (individuo id){
	int mi;
	for ( int i=0; i<149; i++){
		mi=rand()%1000;
		if(mi<16){
			id.regra[i]=(id.regra[i]+1)%2;
		}
	}
	return id;	
}

int torneio (int num){
	int a1, a2;
	num=num-2;
	a1=rand()%100;
	a2=rand()%100;
	while(num>0){
		if(pop[a1].nota>pop[a2].nota) a2=rand()%100;
		num--;
	}
	if(pop[a1].nota>pop[a2].nota) return a1;
	return a2;
}

int ordena (const void * a , const void * b) {
	individuo f1;
	individuo f2;
	f1=*(individuo*)a;
	f2=*(individuo*)b;
	if (f1.nota>=f2.nota){
		return -1;
	}
	else return 1;
}

void reinsercao (){
	int p=10, f=0;
	qsort (pop, 100, sizeof(individuo), ordena);  		
	qsort (filhos, 90, sizeof(individuo), ordena);  	
	for ( int tes=10; tes<100; tes++){
		if(pop[p].nota>filhos[f].nota){
			p++;
		}
		else {
			for ( int aux1=0; aux1<tama; aux1++){
				pop[p].regra[aux1]=filhos[f].regra[aux1];	
			}
			pop[p].nota=filhos[f].nota;
			f++; p++;	
		}
	}
}

void cross (int K ){
	int pai, mae, pto;
	pai=torneio(tour);
	mae=torneio(tour);
	while(mae==pai){
		pai=torneio(tour);
	}
	pto=rand()%tama;
	for ( int i=0; i<tama; i++){
		if(i<pto){
			filhos[K].regra[i]=pop[mae].regra[i];			
			filhos[K+1].regra[i]=pop[pai].regra[i];
		}
		else {
			filhos[K+1].regra[i]=pop[mae].regra[i];			
			filhos[K].regra[i]=pop[pai].regra[i];
		}
	}
	filhos[K+1]=muta_ind(filhos[K+1]);	
	filhos[K]=muta_ind(filhos[K]);
	filhos[K]=avalia_r2(filhos[K]);
	
	filhos[K+1]=avalia_r2(filhos[K+1]);
	// avaliar os filho		
}

int main (){
	int i; /**/
	srand (time(NULL));
	
	gera_reti();
	for ( int mod=0; mod<72; mod++){	
		for (  i=0; i<100; i++){
			pop[i]=gera_ind(pop[i],i%(tama+1),tama);
			pop[i]=avalia_r2(pop[i]);	
		}
		for(int ger=0; ger<100; ger++){	
			for (i=0; i<tx_c; i=i+2){
				cross(i);
			}  	
			reinsercao();		
		}
		for (  i=0; i<10; i++){
			pop[i]=avalia_r2_final(pop[i]);
		}
		
		printf("%lf\n", pop[0].nota );	
	}
		
}
