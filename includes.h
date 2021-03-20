#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <unistd.h>
#define exit_on_error(s,m) if (s<0) {perror(m); exit(1);}
#define exit_on_null(s,m) if (s==NULL) {perror(m); exit(1);}
#define MAX_OUV 20
#define MAX_PL 20
#define MAX_MUS 20

struct sembuf DOWN = {0, -1, 0}; 
struct sembuf UP = {0, 1, 0};


typedef struct {
char id_mus[10]; // Identificador da música, de formato MUS_xxxxx
char titulo[60]; // Título da Música
char artista[50]; // Nome do intérprete
int duracao; // Duração da música (em segundos)
int ano; // Ano de lançamento da música
char genero[20]; // Género musical
int top; // Posição da música no top
} Tmusica;

typedef struct {
char id_pl[10]; // Identificador da playlist, de formato PL_xxxxx
char nome[60]; // Nome da playlist
char musicas[250];// Lista ordenada de músicas, separadas por “:”
} Tplaylist;

typedef struct {
	char nick[50]; // Nickname do ouvinte (para o Login)
	char pass[50]; // Password do ouvinte (para o Login)
	int num; // Número de aluno
	char nome[50]; // Nome do ouvinte
	char email[50]; // E-mail
	char curso[50]; // Curso
	char playlists[250]; // Lista de playlists registadas separada por “:”
} Touvinte;

typedef struct {
	long tipo;
	struct {
		char operacao[20];
		char info1[250];
		char info2[250];
		int myid;
	} dados;
} MsgOuvinte2Server;

typedef struct {
	long tipo;
	struct {
		Tmusica musica;
		char info1[250];
		int manuntencao;
		int valor1;
		int status;
	} dados;
} MsgServer2Ouvinte;

void estado_semaforo(int id_sem, unsigned short num_sem){
	int status = semctl(id_sem, num_sem, GETVAL, 0);
	printf("estado do semaforo: %d\n", status);
}

int valor_semaforo(int id_sem, unsigned short num_sem){
	int status = semctl(id_sem, num_sem, GETVAL, 0);
	return status;
}

void down(int sem){
	int status_sem = semop ( sem, &DOWN, 1 );
	exit_on_error (status_sem, "DOWN");
	printf("\nDown\n");
}

void up(int sem){
	int status_sem = semop ( sem, &UP, 1 );
	exit_on_error (status_sem, "UP");
	printf("\nUp\n");
}

void obter_substring(char linha[], char resultado[], char separador, int indice) {
   int i, j=0, meu_indice = 0;
   for (i=0; linha[i] != '\0'; i++) {
        if ( linha[i] == separador ) {
            meu_indice++;
        } else if (meu_indice == indice) {
         resultado[j++]=linha[i];
        }
   }
   resultado[j]='\0';
}
