#ifndef HASH_H
#define HASH_H 1

#include <list.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define HT_SIZE 10

typedef struct {
	int ficheiro;//file descriptor
	ssize_t maxlinha;//max char por linha
	pthread_mutex_t rwritelock;
	char* name;
}file;

/* Estrutura com algumas informacoes sobre cada shard. Um semaforo para controlar 
 * a actividade de cada shard, max para saber quantas listas ligadas estao na respectiva shard e
 * o ponteiro para a lista da shard.*/
typedef struct hasher{
	file* fich;
	int max;
	int numKV;
	int Id;
	sem_t sem;
	list* lista[HT_SIZE];
}shard;



/* HashInit cria a hash_table. Devolve uma hash_table, com o numero anteriormente
 * definido de listas ligadas. O valor é definido pelo define HT_SIZE. */
shard* HashInit(int max,int Id);
 
/* hash decide qual a shard onde inserir o valor (funcao fornecida pelo 
 * corpo docente). */
int hash(char* key);

/* HashSearch procura na lista o value da respectiva key dada. Recebe como argumentos
 * a shard, para saber em que shard procurar, e a key. Devolve o respectivo par KV_t. */
KV_t* HashSearch(shard* sharde ,char* key);

/* HashInsert insere um novo par KV_t na lista, na shard escolhida pela funcao hash. */
void HashInsert(shard* lista, KV_t* par);

/* HashPrint imprime todos os elementos. Recebe como argumento uma shard, ao qual imprime todos os
 * pares KV_t. */
void HashPrint(shard* sharde);

/* HashRemove remove um par KV_t. Recebe como argumentos a shard, onde o par está inserido,
 * e o respectivo par KV_t. */
void HashRemove(shard* sharde, KV_t* pare);

/* HashEmpty verifica se a shard esta vazia. Recebe como argumentos a shard a verificar. Devolve 
 * 0 se estiver vazia, 1 caso contrario. */
int HashEmpty(shard* sharde);

/*Read_File ve se o ficheiro ja existe devolve 0 se nao existir e 1 se nao existir*/
int Read_File(file* files, shard* sharde);

//void Import_Line(char* linha, shard* sharde);

/*file_init inicializa a estrutura do ficheiro*/
file* file_init(file* files);

void Write_File(file* files, KV_t* pare);

int Search_File(shard* sharde, KV_t* pare);

void Remove_Line(shard* sharde ,KV_t* pare);

void Shard_To_File(shard* sharde);

void HashImport(shard* sharde, KV_t* pare);

void FLine_File(int fich);

char* Find_Name(int shardId);

void Read_Line(shard* sharde,int line, char * content);

void Altera_Value(shard* sharde, KV_t* pare);

void optimizador_de_espaco(file* files,shard* sharde);

#endif
