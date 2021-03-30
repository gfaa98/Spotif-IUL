# Spotif-IUL
Project for Operating Systems ISCTE 2019

A plataforma Spotif-IUL destina-se à partilha de músicas entre estudantes, docentes e colaboradores do
ISCTE-IUL. Este programa (fictício!) pretende a estimular o estudo e o exercício físico. Para além de
poderem ouvir as músicas preferidas, poderão também criar e partilhar playlists com os outros ouvintes.
Ouvinte: é a pessoa que deseja ouvir música e criar playlists. Tem um determinado perfil que inclui um
nickname, password, número, nome, e-mail, curso e playlists a que está associado.
Música: é uma música que o ouvinte pode escolher, caracterizada pelo seguinte conjunto de atributos:
identificador da música (da forma MUS_número), nome da música, artista, duração (em segundos),
ano de produção, género, posição no top.
Playlist: conterá a lista de playlists criadas pelos ouvintes, caracterizada pelo seguinte conjunto de
atributos: identificador da playlist (da forma PL_número), nome da playlist, músicas (identificador de
músicas da forma MUS_número).
Os ouvintes ligam-se à aplicação Spotif-IUL, fazendo login com o seu nickname e a sua password, e
escolhendo operações como listar músicas ou ouvir uma playlist. Um programa servidor irá tratar destes
pedidos, enviando as respostas indicadas. O trabalho inclui também o desenvolvimento de uma aplicação
admin que estará encarregue da manutenção das estruturas de dados e ficheiros envolvidos na parte do
servidor.

A lista de ouvintes do sistema passará a estar disponível numa memória partilhada, que será utilizada
tanto pelo programa admin.c como pelo servidor.c. A comunicação entre os vários processos de ouvinte
e o servidor é feita através de uma fila de mensagens, através da qualserão feitos pedidos para diferentes
tipos de operações, tais como: login, listar músicas, ouvir playlist, associar a playlist e logout. Os
programas devem utilizar semáforos sempre que necessário, especialmente quando são utilizadas
memórias partilhadas, por forma a garantir exclusão mútua e evitar colisões.

ouvintes.txt
Este ficheiro contém os ouvintes registados na aplicação Spotif-IUL, contendo as credenciais para login
entre outras informações sobre cada ouvinte:
nickname:password:numero_aluno:nome:email:curso:lista_de_playlists
Exemplo:

Batman:robin:12345:João Caniço:a12345@iscte-iul.pt:IGE:PL_1:PL_2\n
asterix:obelix:67890:Marta Santos:a67890@iscte-iul.pt:ETI:PL_2\n
...

musicas.txt
Este ficheiro guarda a lista de músicas que existe na aplicação. Deve seguir o seguinte formato (é, na
prática, o mesmo formato da parte 2, mas agora o último campo da música corresponde ao número de
vezes que essa música foi ouvida):
ID_musica:nome_musica:intérprete:duração:ano_produção:género:num_vezes_ouvida
Exemplo:

MUS_1:Bohemian Rhapsody:Queen:359:1975:rock:37\n
MUS_2:Ai se ele cai:Xutos e Pontapes:191:2004:rock:7\n
MUS_3:We are the champions:Queen:190:1977:rock:13\n
MUS_4:Estou alem:Antonio Variacoes:303:1983:rock:13\n
MUS_5:Like a prayer:Madonna:338:1989:pop:9\n
...

playlists.txt
Este ficheiro guarda as playlists registadas no sistema, ou seja, a lista ordenada de músicas que pertence
a cada playlist. Deve seguir o seguinte formato (é, na prática, o mesmo formato das partes 1 e 2):
ID_playlist:nome_playlist:lista_ordenada_de_identificadores_musicas
Exemplo:

PL_1:Jogging:MUS_3:MUS_2\n
PL_2:Estudo:MUS_5:MUS_3:MUS_4\n
PL_3:Classicas:MUS_8:MUS_10:MUS_12:MUS_1:MUS_15\n


Admin.c
Este programa é responsável por criar as memórias partilhadas e colocar lá os dados dos utilizadores. Ao
correr, deve começar por verificar se a memória partilhada já existe. Caso exista, deve apenas associar-se
a ela. Caso não exista, deverá criá-la.
Este programa é também o responsável por criar e inicializar os semáforos que serão usados para garantir
exclusão mútua no acesso à memória partilhada. Deve começar por verificar seu grupo de semáforos já
existe. Caso exista deve apenas associar-se a ele. Caso não exista deverá criá-los e iniciá-los
adequadamente.
Deve também criar a fila de mensagens, necessária para a comunicação entre servidor e ouvintes.
De seguida deve apresentar um menu com as seguintes opções:

-------- MENU --------
1. Carregar ficheiros
2. Descarregar memória
3. Manutenção aplicação
4. Mostrar memória
0. Sair


 Carregar ficheiros: ler a informação sobre os ouvintes, músicas e playlists a partir dos respetivos
ficheiros e carregar essa informação para estruturas de dados em memória (que devem ser
previamente iniciadas com valores indicando “registo vazio”). Para cada estrutura deverá ser criada
uma memória partilhada (Mem_musicas, Mem_playlists e Mem_ouvintes) e respetivo semáforo para
controlo de acesso (Sem_musicas, Sem_playlists e Sem_ouvintes). Recomenda-se que os semáforos
sejam a última funcionalidade a ser implementada.
 Descarregar memória: ler a informação das memórias partilhadas e atualizar os respetivos ficheiros
musicas.txt, playlists.txt e ouvintes.txt.
 Manutenção aplicação: Apesar de neste trabalho não vir a ser desenvolvida nenhuma ação em
concreto, pretende-se mimicar a necessidade de manutenção deste tipo de aplicações, durante a qual
as memórias estarão inacessíveis. Quando esta funcionalidade for selecionada, o programa deve
imprimir uma mensagem como se mostra abaixo e adormecer durante 10 segundos:
*************************************
EM MANUTENÇÃO
*************************************
Após os 10 segundos, escreve uma mensagem a dizer que a manutenção terminou e volta ao menu.
 Mostrar memória: apresenta o conteúdo de cada uma das memórias partilhadas de forma
estruturada.
 Sair: desliga a interface do administrador. Salienta-se que as memórias, fila de mensagens e
semáforos devem continuar disponíveis, apesar de o programa admin ser desligado.


Servidor.c
Este é um dos principais componentes da arquitetura do Spotif-IUL. É ele o responsável por dar resposta
a todos os pedidos dos vários clientes. Deve operar os semáforos sempre que se justifique e implementar
a resposta aos pedidos que chegam via fila de mensagens. Nomeadamente, deve garantir resposta a
mensagens:
 Pedido de autenticação: deve validar o nickname e a password, com base na informação presente na
memória partilhada dos ouvintes e responder ao ouvinte, confirmando, ou não, o login (por exemplo,
utilizar a variável status como sendo booleana).
 Listar músicas de playlist: deve responder ao pedido do ouvinte (“listen_playlist”) com várias
mensagens, uma por cada música associada à playlist solicitada, com a informação sobre a mesma.
Caso a playlist não seja válida, deve enviar uma mensagem de erro ao ouvinte (por exemplo, utilizar
a variável status como sendo booleana).
 Ouvir playlist: deve responder ao pedido com várias mensagens, uma por cada música associada à
playlist solicitada, com a informação sobre a mesma, e atualizar o campo correspondente ao número
de vezes que as músicas foram ouvidas. Caso a playlist não seja válida, deve enviar uma mensagem
de erro ao ouvinte (por exemplo, utilizar a variável status como sendo booleana).
 Associar a playlist: Responde ao pedido do ouvinte “get_playlist”. Deve aceder à memória partilhada
de playlists e verificar se a playlist, a que o ouvinte se quer associar, existe. Deve, também, aceder à
memória partilhada dos ouvintes e verificar se o utilizador já está ou não associado à playlist pedida.
Caso o ouvinte não esteja associado e a playlist exista, deve associar o utilizador à playlist, registando
essa ação na memória partilhada dos ouvintes e responder, ao ouvinte, em conformidade.
Note que em qualquer acesso às memórias partilhadas, o servidor deve previamente verificar se a
aplicação se encontra em manutenção (opção 3 do programa administrador). Se assim for, deve
responder ao utilizador com essa informação e ignorar o pedido.


Ouvinte.c
Este programa é aquele que é usado pelos ouvintes para interagirem com a aplicação Spotif-IUL. Ao ser
executado, deverá pedir o nickname e password ao utilizador e tentar fazer a sua autenticação, enviando
ao servidor uma mensagem com o campo operacao preenchido com “login”. A resposta do servidor é
recebida através de uma mensagem cujo tipo é o PID do processo ouvinte. Caso a autenticação tenha
sido bem-sucedida, o ouvinte deverá obter o seu número de aluno (num) como resposta, que será
utilizado para futuras comunicações com o servidor. Nas mensagens de autenticação, o ouvinte envia o
seu PID na mensagem (no campo myid) e ficará à espera de uma mensagem de resposta do servidor com
o tipo igual ao seu PID. Na operação de login, o servidor reponde com o sucesso ou insucesso da operação.
Após receber uma mensagem de autenticação com “sucesso”, o programa ouvinte deve apresentar um
menu com as seguintes opções:

-------- MENU --------
1. Listar músicas de playlist
2. Ouvir playlist
3. Associar a playlist
0. Sair (Logout)

 Listar músicas de playlist: envia pedido ao servidor preenchendo o campo operacao da estrutura de
mensagem com “list_musics” e fica a aguardar a resposta do servidor.
 Ouvir playlist: envia pedido ao servidor preenchendo os campos operacao e info1 da estrutura de
mensagem com “listen_playlist” e o nome da playlist, previamente solicitado ao utilizador,
respetivamente. Após receber as mensagens do servidor, deve apresentar as playlists e respetivas
músicas ao utilizador.
 Associar a playlist: envia pedido ao servidor preenchendo os campos operacao e info1 com
“get_playlist” e o nome da playlist, previamente solicitado ao utilizador, respetivamente. De seguida,
fica a aguardar a mensagem de confirmação do servidor.
 Sair (Logout): envia pedido ao servidor, preenchendo o campo operacao com “logout”. Deve aguardar
a resposta (sucesso/insucesso) do servidor.
Salienta-se que o servidor poderá indicar que a aplicação se encontra em manutenção. Nesta situação, o
pedido do ouvinte não será executado e deverá ser dado feedback ao ouvinte para tentar novamente
mais tarde.

As mensagens que o ouvinte envia ao servidor são sempre do tipo 1.
 A resposta do servidor a um pedido de autenticação é feita através de uma mensagem com o tipo
igual ao PID do processo ouvinte.
 As restantes respostas do servidor são sempre do tipo igual ao num (número de aluno) desse ouvinte,
que terá sido enviado ao ouvinte se a autenticação tiver sido bem-sucedida, o qual deve ser sempre
preenchido no campo myid.
