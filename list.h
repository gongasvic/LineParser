#ifndef LIST_H
#define LIST_H 1

#include <KV_t.h>

/* Estrutura com um par do tipo KV_t, anteriormente criado pela funcao novo_par, e com
 * um ponteiro para o elemento seguinte da lista.*/
typedef struct node_s{
	KV_t* par;
	struct node_s* next;
}node;

/* Estrutura com um ponteiro para o primeiro elemento da lista.*/
typedef struct lista{
	sem_t semList;
	node * first;
}list;

/* init_list cria uma nova lista. Devolve uma lista com um elemento a NULL e o 
 * respectivo ponteiro.*/
list* init_list();
/* insert_begin insere um novo elemento na lista. Tem como argumentos o par KV_t a inserir 
 * e o ponteiro da lista.*/
void insert_begin(list* lst, KV_t* pare);
/* rem_list remove um elemento da lista. Tem como argumentos o ponteiro da lista e o par
 * KV_t a remover. */
void rem_list(list* lst, KV_t* pare);
/* seach_list procura na lista. Tem como argumentos o ponteiro da lista e a key a procurar.
 * Devolve o par KV_t. */
KV_t* search_list(list* lst, char* chave);



#endif
