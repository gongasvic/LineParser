
#include <server.h>
#include <buffer.h>



/************************** server_get ********************************/

char* server_get(shard* sharde, char* key) {
	sem_wait(&sharde->lista[hash(key)]->semList);
	delay();
	KV_t* pare = (KV_t*)calloc(1,sizeof(KV_t));
	pare = HashSearch(sharde, key);
	if(pare == NULL){
		sem_post(&sharde->lista[hash(key)]->semList);
		return NULL;
	}
	else{
		sem_post(&sharde->lista[hash(key)]->semList);
		return pare->value;
	}
}

/************************** server_put ********************************/

char* server_put(shard* sharde, char* key, char* value) {
	int valorSem_t;
	sem_getvalue(&sharde->sem, &valorSem_t);
	
	if(valorSem_t == 0){
		sem_wait(&sharde->sem);
		delay();
		sem_post(&sharde->sem);
	}
	sem_wait(&sharde->lista[hash(key)]->semList);
	delay();
	
	if (key == NULL || value == NULL)
		return NULL;
	
	KV_t* pare = (KV_t*)calloc(1,sizeof(KV_t));
	pare = HashSearch(sharde, key);
	if(pare == NULL){
		pare = novo_par(key,value);
		HashInsert(sharde,pare);
		pthread_mutex_lock(&sharde->fich->rwritelock);
		delay();
		Write_File(sharde->fich, pare);
		pthread_mutex_unlock(&sharde->fich->rwritelock);
		sem_post(&sharde->lista[hash(key)]->semList);
		return NULL;
	}
	else {
		KV_t* par = (KV_t*)calloc(1,sizeof(KV_t));
		HashRemove(sharde,pare);
		par = novo_par(key,value);
		HashInsert(sharde,par);
		pthread_mutex_lock(&sharde->fich->rwritelock);
		delay();
		Altera_Value(sharde, pare);
		pthread_mutex_unlock(&sharde->fich->rwritelock);
		sem_post(&sharde->lista[hash(key)]->semList);
		return pare->value;
	}
}

/************************* server_remove ******************************/

char* server_remove(shard* sharde, char* key) {
	int valorSem_t; 
    sem_getvalue(&sharde->sem, &valorSem_t);
	
	if(valorSem_t == 0){
		sem_wait(&sharde->sem);
		delay();
		sem_post(&sharde->sem);
	}
	sem_wait(&sharde->lista[hash(key)]->semList);
	delay();
	KV_t* pare = (KV_t*)malloc(sizeof(KV_t));
	pare = HashSearch(sharde, key);
	
	if(pare == NULL){
		sem_post(&sharde->lista[hash(key)]->semList);
		return NULL;
	}
	else{
		HashRemove(sharde, pare);
		pthread_mutex_lock(&sharde->fich->rwritelock);
		delay();
		Remove_Line(sharde, pare);
		pthread_mutex_unlock(&sharde->fich->rwritelock);
		sem_post(&sharde->lista[hash(key)]->semList);
		return pare->value;
	}
}

/*********************** server_getAllKeys ****************************/

KV_t* server_getAllKeys(shard* sharde, buffer* pos_buffere, int *sizeKVArray) {
	sem_wait(&sharde->sem);
	//delay();
	int i,n=0;
	node* ptr = (node*)calloc(1,sizeof(node));
	int size = sharde->numKV;
	KV_t** vector= (KV_t**)calloc(size,sizeof(KV_t*));
	
	if (HashEmpty(sharde)){
		sem_post(&sharde->sem);
		return NULL;
	}
	for( i = 0; i < HT_SIZE; i++){
		if(sharde->lista[i]->first != NULL){
			for(ptr = sharde->lista[i]->first;  ptr->next != NULL; ptr=ptr->next);{
				vector[n] = ptr->par;
				n++;
			}
		}
	}
	*sizeKVArray = size;
	sem_post(&sharde->sem);
	return vector;
}
