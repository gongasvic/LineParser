#ifndef BUFFER_H
#define BUFFER_H 1

#include <hash.h>


/* Estrutura com informacao sobre o cliente. É preciso o present_client, permite ao buffer saber se 
 * ha clientes por atender; resposta, para saber se o cliente obteve o seu pedido.*/
typedef struct cliente{
	sem_t semClien;
	int present_client;//	Vai a 1 se existe um cliente.
	int* dim;
	char* resposta;
	KV_t** Array;
	int fila_de_espera;
}client;

/* Estrutura server com a informacao a enviar ao servidor. Neste caso é preciso o shardeId, para
 * saber em que shard inserir a informacao; Funcao, para saber qual das funcoes do kos a executar;
 * key e value, informacao a manipular de acordo com a funcao utilizada; present_server, permite ao buffer
 * saber se o server executou todos os procedimentos.*/
typedef struct servidor{
	char* key;
	char* value;
	int shardeId;
	int present_server;//	 vai a 1 se o server concluir todas as funcoes.
	int funcao;
}server;

/* Ponteiro para o vector de ponteiros. */
typedef struct ponteiro_buffer{
	sem_t clien;
	sem_t serv;
	int max;
	client* cliente;
	server* servidore;
}buffer;

/* buffer_init cria dois tipos de buffer: o cliente e o servidor, assim como os respectivos semaforos.
 * Recebe como argumentos o tamanho máximo do buffer, e devolve um buffer com a parte servidor e cliente. */
buffer* buffer_init(int maxi);

/* bufferGet permite que os dados do buffer do servidor sejam enviadas de acordo com a funcao
 * utlizada. Para certificar que houve comunicacao com o servidor, o valor do present_server é 
 * alterada com o valor 1. Recebe como argumentos o buffer completo, e a shard onde 
 * o servidor deve fazer as suas operações. */
void bufferGet(buffer* bufer,shard** sharde);

/* bufferInsert insere a informacao do cliente no buffer no servidor. No buffer do cliente,
 * e' alterado o valor do present_client para informar ao buffer do cliente que ha um cliente 
 * presente. Recebe como argumentos o buffer complexto, shardId para informar ao servidor onde 
 * inserir os dados, a key e value, e a func, para o buffer saber que funcoes executar do servidor.*/
void bufferInsert(buffer* bufer, int shardId, char* key, char* value, int func, int* size);

/* bufferRemove insere todos os elementos do buffer cliente e servidor a NULL ou 0
 * Recebe como argumento o respectivo buffer completo.*/
void bufferRemove(buffer* bufer);

#endif
