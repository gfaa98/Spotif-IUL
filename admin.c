#include "includes.h"

Touvinte *ouv;
char opcao[5];
int nopcao;
int status_ouv;
int status_mus;
int status_pl;

void menu(Touvinte * ouv, Tmusica * mus, Tplaylist * pl,int sem_ouv,int sem_mus,int sem_pl,int sem_man);
void inicializa_mem_ouvintes(Touvinte *ouv);
void inicializa_mem_playlists(Tplaylist *pl);
void inicializa_mem_musicas(Tmusica *mus);
void descarrega_memoria_ouvintes(Touvinte* ouv);
void descarrega_memoria_musicas(Tmusica* mus);
void descarrega_memoria_playlists(Tplaylist* pl);
void carrega_mem_ouvintes(Touvinte *ouv);
void carrega_mem_musicas(Tmusica *mus);
void carrega_mem_playlists(Tplaylist *pl);
void mostra_mem_ouvintes(Touvinte * ouv);
void mostra_mem_musicas(Tmusica *mus);
void mostra_mem_playlists(Tplaylist *pl);

void down_all(int sem_ouv,int sem_mus,int sem_pl){
	int status = semop ( sem_ouv, &DOWN, 1 );
	exit_on_error (status, "DOWN");
	int status1 = semop ( sem_mus, &DOWN, 1 );
	exit_on_error (status1, "DOWN");
	int status2= semop ( sem_pl, &DOWN, 1 );
	exit_on_error (status2, "DOWN");
	printf("Down all\n");
}

void up_all(int sem_ouv,int sem_mus,int sem_pl){
	int status = semop ( sem_ouv, &UP, 1 );
	exit_on_error (status, "UP");
	int status1 = semop ( sem_mus, &UP, 1 );
	exit_on_error (status1, "UP");
	int status2= semop ( sem_pl, &UP, 1 );
	exit_on_error (status2, "UP");
	printf("Up all\n");
}

void menu(Touvinte * ouv, Tmusica * mus, Tplaylist * pl,int sem_ouv,int sem_mus,int sem_pl,int sem_man){
		
	printf("-------- MENU --------\n1. Carregar ficheiros\n2. Descarregar memória\n3. Manutenção aplicação\n4. Mostrar memória\n5.Limpar Memórias\n0. Sair\n");
	fgets(opcao, 5, stdin);
	nopcao= atoi(opcao);
	if(nopcao==1){
		printf("1\n");
		down_all(sem_ouv,sem_mus,sem_pl);
		carrega_mem_ouvintes(ouv);
		carrega_mem_musicas(mus);
		carrega_mem_playlists(pl);
		up_all(sem_ouv,sem_mus,sem_pl);
	}
	else if(nopcao==2){
		printf("2\n");
		down_all(sem_ouv,sem_mus,sem_pl);
		descarrega_memoria_ouvintes(ouv);
		descarrega_memoria_musicas(mus);
		descarrega_memoria_playlists(pl);
		up_all(sem_ouv,sem_mus,sem_pl);
	}
	else if(nopcao==3){
		printf("3\n");
		printf("*************************************\nEM MANUTENÇÃO\n*************************************\n");
		int status = semop ( sem_man, &DOWN, 1 );
		exit_on_error (status, "DOWN");
		sleep(10);
		printf("MANUNTENÇÃO TERMINOU\n");
		int status2= semop ( sem_man, &UP, 1 );
		exit_on_error (status2, "UP");
	}
	else if(nopcao==4){
		printf("4\n");
		down_all(sem_ouv,sem_mus,sem_pl);
		mostra_mem_ouvintes(ouv);
		mostra_mem_musicas(mus);
		mostra_mem_playlists(pl);
		up_all(sem_ouv,sem_mus,sem_pl);
	}
	else if(nopcao==5){
		down_all(sem_ouv,sem_mus,sem_pl);
		inicializa_mem_ouvintes(ouv);
		inicializa_mem_musicas(mus);
		inicializa_mem_playlists(pl);
		up_all(sem_ouv,sem_mus,sem_pl);
	}
	else if(nopcao==0){
		printf("0\n");
	}
	else{
		printf("Opcao Invalida\n");
	}
}

void inicializa_mem_ouvintes(Touvinte *ouv){
	for(int i=0;i!=MAX_OUV;i++){
		strcpy(ouv[i].nick,"vazio");
		strcpy(ouv[i].pass,"vazio");
		ouv[i].num=0;
		strcpy(ouv[i].nome,"vazio");
		strcpy(ouv[i].email,"vazio");
		strcpy(ouv[i].curso,"vazio");
		strcpy(ouv[i].playlists,"vazio");
	}
}

void inicializa_mem_playlists(Tplaylist *pl){
	for(int i=0;i!=MAX_PL;i++){
		strcpy(pl[i].id_pl,"vazio");
		strcpy(pl[i].nome,"vazio");
		stpcpy(pl[i].musicas,"vazio");
	}
}

void inicializa_mem_musicas(Tmusica *mus){
	for(int i=0;i!=MAX_OUV;i++){
		strcpy(mus[i].id_mus,"vazio");
		strcpy(mus[i].titulo,"vazio");
		strcpy(mus[i].artista,"vazio");
		mus[i].duracao=0;
		mus[i].ano=0;
		strcpy(mus[i].genero,"vazio");
		mus[i].top=0;
	}
}

void descarrega_memoria_ouvintes(Touvinte* ouv){
	FILE *f = fopen("ouvintes.txt", "w");
	if (f==NULL) { perror("Erro de escrita: "); exit(3); } ;
	int i;
	for (i=0; (i < MAX_OUV) && ( ouv[i].num != 0 ) ; i++ ) {
		fprintf(f, "%s:%s:%d:%s:%s:%s:%s\n",ouv[i].nick, ouv[i].pass, ouv[i].num,ouv[i].nome,ouv[i].email,ouv[i].curso,ouv[i].playlists );
	}
	fclose(f);
}

void descarrega_memoria_musicas(Tmusica* mus){
	FILE *f = fopen("musicas.txt", "w");
	if (f==NULL) { perror("Erro de escrita: "); exit(3); } ;
	int i;
	for (i=0; (i < MAX_MUS) && (strcmp(mus[i].id_mus,"vazio")!=0) ; i++ ) {
		fprintf(f, "%s:%s:%s:%d:%d:%s:%d\n",mus[i].id_mus, mus[i].titulo, mus[i].artista,mus[i].duracao,mus[i].ano,mus[i].genero,mus[i].top );
	}
	fclose(f);
}

void descarrega_memoria_playlists(Tplaylist* pl){
	FILE *f = fopen("playlists.txt", "w");
	if(f==NULL) {perror("Erro de escrita: "); exit(3);};
	int i;
	for(i = 0; (i<MAX_PL) && (strcmp(pl[i].id_pl,"vazio")!=0);i++){
		fprintf(f, "%s:%s%s\n",pl[i].id_pl,pl[i].nome,pl[i].musicas);
	}
	fclose(f);
}

void carrega_mem_ouvintes(Touvinte *ouv){
	int n=0;
	char linha[250];
	char tmp[100];
	char plists[250]="";
	FILE *fouvinte =fopen("ouvintes.txt","r");
	int i = 0;
	int pos = 0;
	if (NULL != fouvinte) {
		fseek (fouvinte, 0, SEEK_END);
		int size = ftell(fouvinte);

		if (0 == size) {
			printf("ouvintes.txt is empty\n");
		}else{
			fseek(fouvinte,0,SEEK_SET);
			printf("ouvintes.txt is NOT empty\n");
			while(fgets(linha,250,fouvinte) != NULL){
				printf("linha\n");
				linha[strlen(linha)-1]='\0';
				if(pos < MAX_OUV){
					strcpy(ouv[pos].playlists, "");
					obter_substring(linha,ouv[pos].nick,':',0);
					obter_substring(linha,ouv[pos].pass,':',1);
					obter_substring(linha,tmp,':',2);
					ouv[pos].num=atoi(tmp);
					obter_substring(linha,ouv[pos].nome,':',3);
					obter_substring(linha,ouv[pos].email,':',4);
					obter_substring(linha,ouv[pos].curso,':',5);
					for(int j=0;j<strlen(linha);j++){
						if(linha[j] == ':'){
							n++;
						}
					}
					printf("n: %d\n",n);
					for(int k=6;k<n+1;k++){
						obter_substring(linha,tmp,':',k);
						strcat(tmp,":");
						strcat(ouv[pos].playlists,tmp);
						strcat(tmp,"");
						printf("playlists: %s\n",ouv[pos].playlists);
					}
					
					ouv[pos].playlists[strlen(ouv[pos].playlists)-1]='\0';
					n=0;
					pos++;
				}else{
					printf("Impossivel carregar, memoria cheia");
				}
			}
		}
	}
	fclose(fouvinte);
}

void carrega_mem_musicas(Tmusica *mus){
	
	int n=0;
	char linha[250];
	char tmp_duracao[100];
	char tmp_ano[100];
	char tmp_top[100];
	FILE *fmusica =fopen("musicas.txt","r");
	int i = 0;
	int pos = 0;
	if (NULL != fmusica) {
		fseek (fmusica, 0, SEEK_END);
		int size = ftell(fmusica);

		if (0 == size) {
			printf("musicas.txt is empty\n");
		}else{
			fseek(fmusica,0,SEEK_SET);
			while(fgets(linha,250,fmusica) != NULL){
				printf("Linha\n");
				linha[strlen(linha)-1]='\0';
				if(pos < MAX_MUS){
					printf("Entrei");
					obter_substring(linha,mus[pos].id_mus,':',0);
					obter_substring(linha,mus[pos].titulo,':',1);
					obter_substring(linha,mus[pos].artista,':',2);
					obter_substring(linha,tmp_duracao,':',3);
					mus[pos].duracao=atoi(tmp_duracao);
					obter_substring(linha,tmp_ano,':',4);
					mus[pos].ano=atoi(tmp_ano);
					obter_substring(linha,mus[pos].genero,':',5);
					obter_substring(linha,tmp_top,':',6);
					mus[pos].top=atoi(tmp_top);
					pos++;
				}else{
					printf("Impossivel carregar, memoria cheia");
				}
			}
		}
	}
	fclose(fmusica);
}

void carrega_mem_playlists(Tplaylist *pl){
	char linha[250];
	FILE *fplaylist =fopen("playlists.txt","r");
	char tmp[100];
	int n = 0;
	int pos = 0;
	if (NULL != fplaylist) {
		fseek (fplaylist, 0, SEEK_END);
		int size = ftell(fplaylist);

		if (0 == size) {
			printf("playlists.txt is empty\n");
		}else{
			fseek(fplaylist,0,SEEK_SET);
			while(fgets(linha,250,fplaylist) != NULL){
				printf("linha\n");
				if(pos < MAX_MUS){
					strcpy(pl[pos].musicas, "");
					printf("Passei aqui\n");
					obter_substring(linha,pl[pos].id_pl,':',0);
					printf("id: %s\n",pl[pos].id_pl);
					obter_substring(linha,pl[pos].nome,':',1);
					for(int j=0;j<strlen(linha);j++){
						if(linha[j] == ':'){
							n++;
						}
					}
					for(int k=2;k<n;k++){
						obter_substring(linha,tmp,':',k);
						strcat(tmp,":");
						strcat(pl[pos].musicas,tmp);
						printf("musicas: %s\n",pl[pos].musicas);
						strcat(tmp,"");
					}
					printf("musicas finais: %s\n",pl[pos].musicas);
					pl[pos].musicas[strlen(pl[pos].musicas)-1]='\0';
					
					n=0;
					pos++;
				}else{
					printf("Impossivel carregar, memoria cheia");
				}
			}
		}
	}
	fclose(fplaylist);
}

void mostra_mem_ouvintes(Touvinte * ouv){
	for(int i = 0;i!=MAX_OUV;i++){
		printf("Ouvinte %d: %s %s %d %s %s %s %s\n",i,ouv[i].nick,ouv[i].pass,ouv[i].num,ouv[i].nome,ouv[i].email,ouv[i].curso,ouv[i].playlists);
	}
}

void mostra_mem_musicas(Tmusica *mus){
	for(int i=0;i!=MAX_MUS;i++){
		printf("Musica %d: %s %s %s %d %d %s %d \n",i,mus[i].id_mus,mus[i].titulo,mus[i].artista,mus[i].duracao,mus[i].ano,mus[i].genero,mus[i].top);
	}
}

void mostra_mem_playlists(Tplaylist *pl){
	for(int i=0; i!=MAX_PL;i++){
		printf("Playlist %d: %s %s %s\n",i,pl[i].id_pl,pl[i].nome,pl[i].musicas);
	}
}

int main(){
	system("clear");
	opcao[5];
	nopcao=-1;
	
	int mem_ouv = shmget(879081, MAX_OUV * sizeof(Touvinte), IPC_CREAT| IPC_EXCL | 0666);
	int mem_mus = shmget(879082, MAX_MUS * sizeof(Tmusica), IPC_CREAT| IPC_EXCL | 0666);
	int mem_pl = shmget(879083, MAX_PL * sizeof(Tplaylist), IPC_CREAT| IPC_EXCL | 0666);

	if(mem_ouv>=0){
		Touvinte *ouv = (Touvinte *)shmat(mem_ouv,0,0);
		exit_on_null(ouv,"erro no attach");
		inicializa_mem_ouvintes(ouv);
	}
	if(mem_mus>=0){
		Tmusica *mus = (Tmusica *)shmat(mem_mus,0,0);
		exit_on_null(mus,"erro no attach");
		inicializa_mem_musicas(mus);
	}
	if(mem_pl>=0){
		Tplaylist *pl = (Tplaylist *)shmat(mem_pl,0,0);
		exit_on_null(pl,"erro no attach");
		inicializa_mem_playlists(pl);
	}
	
	mem_ouv = shmget(879081, MAX_OUV * sizeof(Touvinte), 0);
	exit_on_error(mem_ouv, "shmget ouvinte1");
	Touvinte *ouv = (Touvinte *)shmat(mem_ouv,0,0);
	exit_on_null(ouv,"erro no attach");
	
	mem_mus = shmget(879082, MAX_MUS * sizeof(Tmusica), 0);
    exit_on_error(mem_mus, "shmget musica1");
    Tmusica *mus = (Tmusica *)shmat(mem_mus,0,0);
	exit_on_null(mus,"erro no attach");
		
	mem_pl = shmget(879083, MAX_PL * sizeof(Tplaylist), 0);
    exit_on_error(mem_pl, "shmget playlist1");
    Tplaylist *pl = (Tplaylist *)shmat(mem_pl,0,0);
    exit_on_null(pl,"erro no attach");
	
	//Semáforos
	int sem_ouv = semget (8790811, 1, 0600 | IPC_CREAT );
	exit_on_error (sem_ouv, "Criação/Ligação");
	int status_ouv = semctl ( sem_ouv, 0, SETVAL, 1);
	exit_on_error (status_ouv, "Inicialização");
	
	int sem_mus = semget (8790822, 1, 0600 | IPC_CREAT );
	exit_on_error (sem_mus, "Criação/Ligação");
	int status_mus = semctl ( sem_mus, 0, SETVAL, 1);
	exit_on_error (status_mus, "Inicialização");
	
	int sem_pl = semget (8790833, 1, 0600 | IPC_CREAT );
	exit_on_error (sem_pl, "Criação/Ligação");
	int status_pl = semctl ( sem_pl, 0, SETVAL, 1);
	exit_on_error (status_pl, "Inicialização");
	
	int sem_man = semget (8790844, 1, 0600 | IPC_CREAT );
	exit_on_error (sem_man, "Criação/Ligação");
	int status_man = semctl ( sem_man, 0, SETVAL, 1);
	exit_on_error (status_man, "Inicialização");
	//---------
	
	int id = msgget(88119,0666|IPC_CREAT);	
	
	printf("Criada a fila");
	
	
	while(nopcao!=0){
		menu(ouv,mus,pl,sem_ouv,sem_mus,sem_pl,sem_man);
	}
	printf("A SAIR\n");
	system("clear");
}
