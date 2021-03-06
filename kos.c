#include <pthread.h>
#include <buffer.h>


/* Permite obter o tamanho maximo do buffer, e com isso temos a posicao 
 * correcta no buffer.*/
#define pos clientid%pos_buffere[0]->max

/* Variaveis globais relativas ao buffer e 'a shard.*/
shard** sharde;
buffer** pos_buffere;

pthread_mutex_t rwrite;

/************************ server_thread *******************************/

void *server_thread(void *arg) {
    int i=0;
    while(1){
    	for(i=0;i<pos_buffere[0]->max;i++){
			if(sem_trywait(&pos_buffere[i%pos_buffere[0]->max]->clien)==-1)
				continue;
			bufferGet(pos_buffere[i%pos_buffere[0]->max], sharde);
    		sem_post(&pos_buffere[i%pos_buffere[0]->max]->serv);
    	}
    }
	return NULL;
}

/************************** kos_init **********************************/

int kos_init(int num_server_threads, int buf_size, int num_shards) {
	int* ids=(int*) malloc(sizeof(int)*num_server_threads);
	int i,s;

	pthread_t* vec_threads=(pthread_t*)malloc(sizeof(pthread_t)*num_server_threads);
	if (num_shards <= 0 || buf_size <= 0 || num_server_threads <= 0)
		return -1;
	if ((sharde = (shard**) malloc(num_shards*sizeof(shard*))) == NULL) 
		return -1;
	if ((pos_buffere = (buffer**) malloc(buf_size*sizeof(buffer*))) == NULL)
		return -1;

	for(i=0; i<num_shards; i++){
		sharde[i] = HashInit(num_shards,i);
	}	

	for(i=0;i<num_shards;i++){
		char* nome;
		nome=Find_Name(i);
		sharde[i]->fich->name =(char*)calloc(sizeof(char),strlen(nome));
		sharde[i]->fich->name = nome;
		
		sharde[i]->fich->ficheiro = open(nome, O_CREAT|O_RDWR , S_IRUSR|S_IWUSR);
		
		//optimizador_de_espaco(sharde[i]->fich, sharde[i]);
		Read_File(sharde[i]->fich, sharde[i]);
	}

	for(i=0; i<buf_size; i++)
		pos_buffere[i] = buffer_init(buf_size);

	for (i=0; i<num_server_threads; i++) {
		ids[i]=i;
		if ((s=pthread_create(&vec_threads[i], NULL, &server_thread, NULL) ) ) {
			printf("pthread_create failed with code %d!\n",s);
			return -1;
		}
	}

	return 0;
}

/*************************** kos_get **********************************/

char* kos_get(int clientid, int shardId, char* key) {

	if(clientid < 0  && (shardId < 0 || shardId > sharde[0]->max))
			return NULL;
			bufferRemove(pos_buffere[pos]);
			bufferInsert(pos_buffere[pos], shardId, key, NULL, 0,0);
   			sem_post(&pos_buffere[pos]->clien);
			sem_wait(&pos_buffere[pos]->serv);
			pos_buffere[pos]->cliente->present_client = 0;
			return pos_buffere[pos]->cliente->resposta;
}

/**************************** kos_put *********************************/

char* kos_put(int clientid, int shardId, char* key, char* value) {

	if(clientid < 0  && (shardId < 0 || shardId > sharde[0]->max))
			return NULL;
	
			bufferInsert(pos_buffere[pos], shardId, key, value, 1, 0);
			sem_post(&pos_buffere[pos]->clien);
			sem_wait(&pos_buffere[pos]->serv);
			pos_buffere[pos]->cliente->present_client = 0;
			return pos_buffere[pos]->cliente->resposta;
	
}

/**************************** kos_remove ******************************/

char* kos_remove(int clientid, int shardId, char* key) {

	if(clientid < 0  && (shardId < 0 || shardId > sharde[0]->max))
			return NULL;
	

	bufferRemove(pos_buffere[pos]);
	bufferInsert(pos_buffere[pos], shardId, key, NULL, 2, 0);
	sem_post(&pos_buffere[pos]->clien);
	sem_wait(&pos_buffere[pos]->serv);
	pos_buffere[pos]->cliente->present_client = 0;
	return pos_buffere[pos]->cliente->resposta;
}

/************************* kos_getAllKeys *****************************/

KV_t* kos_getAllKeys(int clientId, int shardId, int* sizeKVArray) {
	if(clientId < 0 && (shardId < 0 || shardId > sharde[0]->max)){
		*sizeKVArray = -1;
		return NULL;
	}
	bufferRemove(pos_buffere[clientId%pos_buffere[0]->max]);
	bufferInsert(pos_buffere[clientId%pos_buffere[0]->max], shardId, NULL, NULL, 3, sizeKVArray);
	sem_post(&pos_buffere[clientId%pos_buffere[0]->max]->clien);
	sem_wait(&pos_buffere[clientId%pos_buffere[0]->max]->serv);
	return pos_buffere[clientId%pos_buffere[0]->max]->cliente->Array;

}
