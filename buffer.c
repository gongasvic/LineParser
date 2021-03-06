
#include <buffer.h>
#include <server.h>

/************************** buffer_init *******************************/

buffer* buffer_init(int maxi){
	//Construtor geral.

	buffer* buffere = (buffer*)malloc(sizeof(buffer));
	buffere->max= maxi;
	sem_init(&buffere->clien, 0, 0);
	sem_init(&buffere->serv, 0, 0);
	//Construtor direcionado ao buffer do cliente.
	buffere->cliente = (client*)malloc(sizeof(client));
	buffere->cliente->present_client = 0;
	sem_init(&buffere->cliente->semClien, 0, 1);
	buffere->cliente->dim = (int*)calloc(1,sizeof(int));
	buffere->cliente->resposta = (char*)calloc(1,sizeof(char));
	buffere->cliente->Array = (KV_t**)calloc(1,sizeof(KV_t*));
	buffere->cliente->fila_de_espera=0;
	//Construtor direcionado ao buffer do server.
	buffere->servidore = (server*)malloc(sizeof(server));
	buffere->servidore->value = NULL; 
	buffere->servidore->key = NULL;
	buffere->servidore->present_server = 0;
	buffere->servidore->shardeId = 0;
	buffere->servidore->funcao = 0;	
	return buffere;
}

/************************** bufferInsert ******************************/

void bufferInsert(buffer* bufer, int shardId, char* key, char* value, int func, int* size){
	//Buffer do cliente.
	bufer->cliente->present_client = 1;
	bufer->cliente->dim = size;
	//Buffer do servidor.
	bufer->servidore->key = key;
	bufer->servidore->value = value;
	bufer->servidore->shardeId = shardId;
	bufer->servidore->funcao = func;
}

/************************** bufferGet *********************************/

void bufferGet(buffer* bufer,shard** sharde){

	bufer->servidore->present_server = 1;
	switch (bufer->servidore->funcao){
		case 0:{
			bufer->cliente->resposta = server_get(sharde[bufer->servidore->shardeId], bufer->servidore->key);
			break;
		}
		case 1:{
			bufer->cliente->resposta = server_put(sharde[bufer->servidore->shardeId], bufer->servidore->key, bufer->servidore->value);
			break;
		}
		case 2:{
			bufer->cliente->resposta = server_remove(sharde[bufer->servidore->shardeId], bufer->servidore->key);
			break;
		}
		case 3:{
			bufer->cliente->Array = (KV_t**)calloc(sharde[bufer->servidore->shardeId]->numKV,sizeof(KV_t*));
			bufer->cliente->Array = server_getAllKeys(sharde[bufer->servidore->shardeId], bufer, bufer->cliente->dim);
			break;
		}
	}
}


/************************** bufferRemove ******************************/

void bufferRemove(buffer* bufer){
/*	//Volta ao estado inicial todos os dados do buffer do cliente.
	bufer->cliente->present_client = 0;
	bufer->cliente->resposta = NULL;
	//Volta ao estado inicial todos os dados do buffer do servidor.
	bufer->servidore->key = NULL;
	bufer->servidore->value = NULL;	
	bufer->servidore->present_server = 0;
	bufer->servidore->shardeId = 0;
	bufer->servidore->funcao = 0;
*/}
