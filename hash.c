
#include <hash.h>

/**************************** HashInit ********************************/

shard* HashInit(int maxi,int Id){
	int i;
	shard* sharde = (shard*)calloc(1,sizeof(shard));
	sharde->fich = file_init(sharde->fich);
	sem_init(&sharde->sem, 0, 1);
	pthread_mutex_init(&(sharde->fich->rwritelock), NULL);
	sharde->Id=Id;
	sharde->max = maxi-1;
	sharde->numKV = 0;
	for( i = 0; i < HT_SIZE; i++){
		sharde->lista[i] = init_list();
	}
	return sharde;
}

/****************************** hash **********************************/

int hash(char* key) {
    int i=0;
    if (key == NULL)
        return -1;
    while (*key != '\0') {
        i+=(int) *key;
        key++;
    }
    i=i%HT_SIZE;
    return i;
}

/**************************** HashSearch ******************************/

KV_t* HashSearch(shard* sharde ,char* key){
	int i = hash(key);
	return search_list(sharde->lista[i],key);
}

/*************************** HashInsert *******************************/

void HashInsert(shard* sharde, KV_t* pare){
	int i = hash(pare->key);
	sharde->numKV ++;
	insert_begin(sharde->lista[i], pare);
}

/**************************** HashPrint *******************************/

void HashPrint(shard* sharde){
	int i;
	node * ptr;
	list* lst;
	KV_t* pare;
	for(i=0;i<HT_SIZE;i++){
		lst = sharde->lista[i];
		for(ptr =lst->first; ptr != NULL; ptr = ptr->next){
			pare=novo_par(ptr->par->key,ptr->par->value);
			Write_File(sharde->fich,pare);
		}
	}
}

/*************************** HashRemove *******************************/

void HashRemove(shard* sharde, KV_t* pare){
	int i = hash(pare->key);
	sharde->numKV--;
	rem_list(sharde->lista[i], pare);
}

/**************************** HashEmpty *******************************/

int HashEmpty(shard* sharde){
	int i;
	for( i = 0; i < HT_SIZE; i++)
		if(sharde->lista[i]->first != NULL)
			return 0;
	return 1;
}
