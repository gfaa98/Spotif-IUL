#include "includes.h"

void listen_playlist(char * nome_pl,Tplaylist*pl,Tmusica*mus,int id);
void login(char * nick, char * pass,Touvinte *ouv,int id);
void list_musics(Tmusica *mus,int id);
void get_playlist(char * nome_pl, Tplaylist *pl, Touvinte *ouv, int id);
void logout(int id);

int num,status_sem_ouv,status_sem_mus,status_sem_pl,manuntencao=1;
MsgOuvinte2Server o2s;
MsgServer2Ouvinte s2o;

int main(){ 
	int id = msgget (88119,0);
	s2o.dados.manuntencao=1;
	int status;
	int status1;
	int status2;
	int status4;
	int loggedin=0;
	
	int mem_ouv = shmget(879081, MAX_OUV * sizeof(Touvinte), 0);
	exit_on_error(mem_ouv, "shmget ouvinte1");
	Touvinte *ouv = (Touvinte *)shmat(mem_ouv,0,0);
	exit_on_null(ouv,"erro no attach");
		
	int mem_mus = shmget(879082, MAX_MUS * sizeof(Tmusica), 0);
	exit_on_error(mem_mus, "shmget musica1");
	Tmusica *mus = (Tmusica *)shmat(mem_mus,0,0);
	exit_on_null(mus,"erro no attach");
			
	int mem_pl = shmget(879083, MAX_PL * sizeof(Tplaylist), 0);
	exit_on_error(mem_pl, "shmget playlist1");
	Tplaylist *pl = (Tplaylist *)shmat(mem_pl,0,0);
	exit_on_null(pl,"erro no attach");
	
	int sem_ouv = semget (8790811, 1,0);
	exit_on_error (sem_ouv, "Criação/Ligação");
	
	int sem_mus = semget (8790822, 1,0);
	exit_on_error (sem_mus, "Criação/Ligação");
	
	int sem_pl = semget (8790833, 1,0);
	exit_on_error (sem_pl, "Criação/Ligação");
	
	int sem_man = semget (8790844, 1, 0 );
	exit_on_error (sem_man, "Criação/Ligação");
	
	while(1){
		status = msgrcv(id, &o2s, sizeof(o2s.dados), 1, 0);
		printf("Mensagem recebida\n");
		exit_on_error(status, "erro ao receber");
		manuntencao=valor_semaforo(sem_man,0);
		if(manuntencao!=0){
			if(strcmp(o2s.dados.operacao,"login")==0){
				printf("Mensagem do login recebida\n");
				//Semaforos down
				status_sem_ouv = semop ( sem_ouv, &DOWN, 1 );
				exit_on_error (status_sem_ouv, "DOWN");
				printf("Down\n");
				//---------------
				login(o2s.dados.info1,o2s.dados.info2,ouv,id);
				//Semaforos up
				status_sem_ouv = semop ( sem_ouv, &UP, 1 );
				exit_on_error (status_sem_ouv, "UP");
				printf("Up\n");
				//---------------
			}
			if(strcmp(o2s.dados.operacao,"listen_playlist")==0){
				printf("Funcao: listen_playlist\n");
				//Semaforos down
				status_sem_mus = semop ( sem_mus, &DOWN, 1 );
				exit_on_error (status_sem_mus, "DOWN");
				status_sem_pl= semop ( sem_pl, &DOWN, 1 );
				exit_on_error (status_sem_pl, "DOWN");
				//---------------
				listen_playlist(o2s.dados.info2,pl,mus,id);
				//Semaforos up
				status_sem_mus = semop ( sem_mus, &UP, 1 );
				exit_on_error (status_sem_mus, "UP");
				status_sem_pl= semop ( sem_pl, &UP, 1 );
				exit_on_error (status_sem_pl, "UP");
				//---------------
			}
			if(strcmp(o2s.dados.operacao,"list_musics")==0){
				printf("Funcao: list_musics\n");
				//Semaforos down
				status_sem_mus = semop ( sem_mus, &DOWN, 1 );
				exit_on_error (status_sem_mus, "DOWN");
				//---------------
				list_musics(mus,id);
				//Semaforos up
				status_sem_mus = semop ( sem_mus, &UP, 1 );
				exit_on_error (status_sem_mus, "UP");
				//---------------
			}
			if(strcmp(o2s.dados.operacao,"get_playlist")==0){
				printf("Funcao: get_playlist\n");
				//Semaforos down
				status_sem_ouv = semop ( sem_ouv, &DOWN, 1 );
				exit_on_error (status_sem_ouv, "DOWN");
				status_sem_pl= semop ( sem_pl, &DOWN, 1 );
				exit_on_error (status_sem_pl, "DOWN");
				//---------------
				get_playlist(o2s.dados.info1, pl, ouv, id);
				//Semaforos up
				status_sem_pl= semop ( sem_pl, &UP, 1 );
				exit_on_error (status_sem_pl, "UP");
				status_sem_ouv = semop ( sem_ouv, &UP, 1 );
				exit_on_error (status_sem_ouv, "UP");
				//---------------
			}
			if(strcmp(o2s.dados.operacao,"logout")==0){
				printf("Funcao: logout\n");
				logout(id);
			}			
		}else{
			s2o.dados.manuntencao=0;
			s2o.tipo = o2s.dados.myid;
			strcpy(s2o.dados.info1,"");
			s2o.dados.valor1=num;
			status2 = msgsnd(id,&s2o,sizeof(s2o.dados),0);
			printf("Mensagem fail enviada ao ouvinte\n");
			exit_on_error(status2, "erro ao enviar");
			printf("Mensagem enviada ao ouvinte\n");
			while(valor_semaforo(sem_man,0)==0){
				sleep(1);
			}
			s2o.dados.manuntencao=1;
		}
	}
}

void logout(int id){
	int status1;
	
	s2o.tipo = o2s.dados.myid;
	strcpy(s2o.dados.info1,"sucesso");
	s2o.dados.valor1=num;
	status1 = msgsnd(id,&s2o,sizeof(s2o.dados),0);
	printf("Mensagem enviada ao ouvinte\n");
	exit_on_error(status1, "erro ao receber");
}

void login(char * nick, char * pass,Touvinte *ouv,int id){
	int result = 0;
	int status1;
	int status2;
	for(int i = 0; ouv[i].num!=0;i++){
		if(strcmp(ouv[i].nick,nick)==0 && strcmp(ouv[i].pass,pass)==0){
			result = 1;
			num = ouv[i].num;
		}
	}
	if(result == 1){
		printf("Login valido\n");
		s2o.tipo = o2s.dados.myid;
		strcpy(s2o.dados.info1,"sucesso");
		s2o.dados.valor1=num;
		status1 = msgsnd(id,&s2o,sizeof(s2o.dados),0);
		printf("Mensagem enviada ao ouvinte\n");
		exit_on_error(status1, "erro ao receber");
	}else{
		printf("Login invalido");
		s2o.tipo = o2s.dados.myid;
		strcpy(s2o.dados.info1,"");
		s2o.dados.valor1=num;
		status2 = msgsnd(id,&s2o,sizeof(s2o.dados),0);
		printf("Mensagem fail enviada ao ouvinte\n");
		exit_on_error(status1, "erro ao receber");
	}
}

void listen_playlist(char * nome_pl,Tplaylist*pl,Tmusica*mus,int id){
	char tmp[100];
	int status;
	int valid = 0;
	for(int i = 0; i!=MAX_PL && valid!=1;i++){
		if(strcmp(pl[i].nome,nome_pl)==0){
			valid = 1;
			s2o.dados.status=valid;
			int n = 0;
			for(int j=0;j<strlen(pl[i].musicas);j++){
				if(pl[i].musicas[j] == ':'){
					n++;
				}
			}
			for(int k=0;k<n+1;k++){
				obter_substring(pl[i].musicas,tmp,':',k);
				for(int p = 0; p!=MAX_MUS;p++){
					if(strcmp(mus[p].id_mus,tmp)==0){
						if(k==n){
							s2o.dados.status=0;
						}
						mus[p].top++;
						s2o.tipo = o2s.dados.myid;
						s2o.dados.musica = mus[p];
						status = msgsnd(id,&s2o,sizeof(s2o.dados),0);
						exit_on_error(status, "erro ao enviar");
					}
				}
			}
		}
	}
	if(valid==0){
		s2o.dados.status=valid;
		status = msgsnd(id,&s2o,sizeof(s2o.dados),0);
		printf("Mensagem fail enviada ao ouvinte\n");
		exit_on_error(status, "erro ao enviar");
	}
	s2o.dados.status=1;
}

void list_musics(Tmusica *mus,int id){
	int status;
	s2o.tipo = o2s.dados.myid;
	s2o.dados.status=1;
	int i;
	for(i=0;(i!=MAX_MUS) && (strcmp(mus[i].id_mus,"vazio")!=0);i++){
		if(strcmp(mus[i+1].id_mus,"vazio")==0 || i+1==MAX_MUS){
			s2o.dados.status=0;
		}
		s2o.dados.musica = mus[i];
		status = msgsnd(id,&s2o,sizeof(s2o.dados),0);
		exit_on_error(status, "erro ao enviar");
	}
	s2o.dados.status=1;
}

void get_playlist(char * nome_pl, Tplaylist *pl, Touvinte *ouv, int id){
	char tmp[50];
	int status;
	int ver_nome_pl=0;
	int ver_num_ouv=0;
	int n_de_pls=0;
	int ver_ja_associado=0;
	int i,j;
	char aux_pl_id[10];
	
	s2o.dados.status=-1;
	s2o.tipo = o2s.dados.myid;
	//verificart nome pl
	for(i=0; i!= MAX_PL && (strcmp(pl[i].nome,"vazio")!=0);i++){
		if(strcmp(pl[i].nome, nome_pl)==0){
			ver_nome_pl=1;
			stpcpy(aux_pl_id,pl[i].id_pl);
			break;
		}
	}
	for(j=0; ver_nome_pl==1 && j!= MAX_OUV && o2s.dados.myid!=0;j++){
		if(o2s.dados.myid == ouv[j].num){
			ver_num_ouv=1;
			break;
		}
	}
	for(int a=0; ver_nome_pl==1 && ver_num_ouv==1 && a<strlen(ouv[j].playlists);a++){
		if(ouv[j].playlists[a] == ':'){
			n_de_pls++;
		}
	}
	for(int k=0;ver_nome_pl==1 && ver_num_ouv==1 && k<n_de_pls+1;k++){
		obter_substring(ouv[j].playlists,tmp,':',k);
		if(strcmp(aux_pl_id, tmp)==0){
			ver_ja_associado=1;
			break;
		}
	} 
	if(ver_nome_pl==1 && ver_num_ouv==1 && ver_ja_associado!=1){
		printf("SUCESSO\n");
		strcat(ouv[j].playlists,":");
		strcat(ouv[j].playlists,aux_pl_id);
		s2o.dados.status=1;
		status = msgsnd(id,&s2o,sizeof(s2o.dados),0);
		exit_on_error(status, "erro ao enviar");
	}
	else{
		printf("INSUCESSO\n");
		s2o.dados.status=0;
		status = msgsnd(id,&s2o,sizeof(s2o.dados),0);
		exit_on_error(status, "erro ao enviar");
	}
}