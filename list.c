
#include <list.h>

/************************* init_list **********************************/

list* init_list(){
	list *lst = (list*)calloc(1,sizeof(list));
	sem_init(&lst->semList, 0, 1);
	lst->first = NULL;
	return lst;
}

/************************* insert_begin *******************************/

void insert_begin(list* lst, KV_t* pare){
	node * l = (node*)calloc(1,sizeof(node));
	l->par = (KV_t*)calloc(1,sizeof(KV_t));
	strcpy(l->par->key,pare->key);
	strcpy(l->par->value,pare->value);
	l->next = lst->first;
	lst->first = l;
}

/************************ search_list *********************************/

KV_t* search_list(list* lst, char* chave){
	node * ptr = (node*)calloc(1,sizeof(node));
	for(ptr=lst->first; ptr!=NULL; ptr=ptr->next){
		if(strcmp(ptr->par->key,chave) == 0)
			return ptr->par;
	}
	return NULL;
}

/************************* rem_list ***********************************/

void rem_list(list* lst, KV_t* pare){
	node** ptr;
	for(ptr=&(lst->first); *ptr!=NULL; ptr=&((*ptr)->next)){
		if(strcmp((*ptr)->par->key, pare->key) == 0)
			break;
		}
	if(ptr!=NULL && *ptr!=NULL){
		node * current = *ptr; 
		(*ptr) = (*ptr)->next;
		free(current);
	}
}


