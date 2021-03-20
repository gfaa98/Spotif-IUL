#include "includes.h"
void menu(int id);
void listar_musicas_playlist(int id);
void listar_musicas(int id);
void login(int id);
void associar_playlist(int id);
void logout(int id);

char opcao[5];
int nopcao;
int numero_ouvinte;
MsgOuvinte2Server o2s; //o2s
int login_verify=0; 
MsgServer2Ouvinte s2o; //s2o
int status,status1;
int nopcao=-1;
char nome_pl[50];
int manuntencao=1;


int main(){
	int id = msgget (88119,0);
	while(login_verify!=1){
		login(id);
	}
	o2s.dados.myid = numero_ouvinte;
	while(login_verify!=0){
		if(manuntencao!=0){
			menu(id);
		}else{
			printf("-> A sair");
			login_verify=0;
		}
	}

	
}

void login(int id){
	
	o2s.tipo = 1;
	
	char nick[50];
	char pass[50];
	
	strcpy(o2s.dados.operacao,"login");
	
	printf("Nickname: ");
	fgets(nick, 50, stdin);
	nick[strlen(nick)-1]='\0';
	strcpy(o2s.dados.info1,nick);
	
	printf("Password: ");
	fgets(pass, 50, stdin);
	pass[strlen(pass)-1]='\0';
	strcpy(o2s.dados.info2,pass);
	
	o2s.dados.myid = getpid();
	
	status = msgsnd(id,&o2s,sizeof(o2s.dados),0);
	exit_on_error(status, "erro ao enviar");
	
	while(1){
		status1 = msgrcv(id, &s2o, sizeof(s2o.dados), getpid(), 0);
		exit_on_error(status1, "erro ao receber");
		manuntencao = s2o.dados.manuntencao;
		if(manuntencao==0){
			printf("Manuntencao a Decorrer\n");
			exit;
		}
		//printf(strcmp(msg_login.dados.info1,"sucesso")");
		if(strcmp(s2o.dados.info1,"sucesso")==0){
			printf("\nSucesso\n\n");
			printf("Numero: %d\n\n",s2o.dados.valor1);
			numero_ouvinte = s2o.dados.valor1;
			login_verify=1;
			break;
		}else{
			printf("Insucesso\n");
			break;
		}
	}
}

void menu(int id){
	printf("-------- MENU --------\n1. Listar musicas de playlist\n2. Ouvir playlist\n3. Associar a playlist\n0. Sair (Logout)\n");
	fgets(opcao, 5, stdin);
	nopcao= atoi(opcao);
	if(nopcao==1){
		listar_musicas(id);
	}
	else if(nopcao==2){
		listar_musicas_playlist(id);
	}
	else if(nopcao==3){
		associar_playlist(id);
	}
	else if(nopcao==0){
		logout(id);
	}
	else{
		printf("Opcao Invalida");
	}
}

void logout(int id){
	strcpy(o2s.dados.operacao,"logout");
	status = msgsnd(id,&o2s,sizeof(o2s.dados),0);
	exit_on_error(status, "erro ao enviar");
	while(1){
		status1=msgrcv(id, &s2o, sizeof(s2o.dados),o2s.dados.myid,0);
		exit_on_error(status1, "erro ao receber");
		manuntencao = s2o.dados.manuntencao;
		if(manuntencao==0){
			printf("Manuntencao a Decorrer\n");
			exit;
		}
		else{
			printf("Logout a decorrer\n -> ADEUS\n");
			login_verify=0;
			break;
		}
	}
}

void listar_musicas(int id){
	strcpy(o2s.dados.operacao,"list_musics");
	status = msgsnd(id,&o2s,sizeof(o2s.dados),0);
	exit_on_error(status, "erro ao enviar");
	int continua=1;
	s2o.dados.status=1;
	while(continua!=0){
		status1=msgrcv(id, &s2o, sizeof(s2o.dados),o2s.dados.myid,0);
		exit_on_error(status1, "erro ao receber");
		manuntencao = s2o.dados.manuntencao;
		if(manuntencao==0){
			printf("Manuntencao a Decorrer\n");
			exit;
		}
		else{
		printf("Musica: %s %s %s %d %d %s %d \n",s2o.dados.musica.id_mus,s2o.dados.musica.titulo,s2o.dados.musica.artista,
													s2o.dados.musica.duracao,s2o.dados.musica.ano,s2o.dados.musica.genero,s2o.dados.musica.top);
		}
		continua=s2o.dados.status;
	}
}


void listar_musicas_playlist(int id){
	printf("Diga a playlist que quer ouvir: ");
	fgets(nome_pl, 50, stdin);
	nome_pl[strlen(nome_pl)-1]='\0';
	strcpy(o2s.dados.info2,nome_pl);
	strcpy(o2s.dados.operacao,"listen_playlist");
	status = msgsnd(id,&o2s,sizeof(o2s.dados),0);
	exit_on_error(status, "erro ao enviar");
	s2o.dados.status=1;
	int continua=1;
	while(continua!=0){
		status1=msgrcv(id, &s2o, sizeof(s2o.dados),o2s.dados.myid,0);
		exit_on_error(status1, "erro ao receber");
		manuntencao = s2o.dados.manuntencao;
		if(manuntencao==0){
			printf("Manuntencao a Decorrer\n");
			exit;
		}else{
			printf("\nMusica: %s\n %s\n %s\n %d\n %d\n %s\n %d\n \n",s2o.dados.musica.id_mus,s2o.dados.musica.titulo,s2o.dados.musica.artista,
						s2o.dados.musica.duracao,s2o.dados.musica.ano,s2o.dados.musica.genero,s2o.dados.musica.top);
		}
		continua=s2o.dados.status;
	}
}

void associar_playlist(int id){
	printf("Diga a playlist que quer associar: ");
	fgets(nome_pl, 50, stdin);
	nome_pl[strlen(nome_pl)-1]='\0';
	strcpy(o2s.dados.info1,nome_pl);
	strcpy(o2s.dados.operacao,"get_playlist");
	status = msgsnd(id,&o2s,sizeof(o2s.dados),0);
	exit_on_error(status, "erro ao enviar");
	int continua=-1;
	s2o.dados.status=1;
	while(continua!=1 && continua != 0){
		status1=msgrcv(id, &s2o, sizeof(s2o.dados),o2s.dados.myid,0);
		exit_on_error(status1, "erro ao receber");
		manuntencao = s2o.dados.manuntencao;
		if(manuntencao==0){
			printf("Manuntencao a Decorrer\n");
			exit;
		}
		continua=s2o.dados.status;
	}
	if(continua == 1){
		printf("Associação sucedida");
	}else{
		printf("Erro na associação");
	}
	
}