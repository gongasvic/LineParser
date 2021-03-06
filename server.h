#ifndef SERV_H
#define SERV_H 1

#include <hash.h>
#include <buffer.h>

/* server_get permite estabelecer uma ligacao com a shard para obter o value da respectiva key.
 * Recebe como argumentos a shard onde esta inserida o value da key, bem como a respectiva key.
 * Devolve o value.*/
char* server_get(shard* sharde, char* key);

/* server_put permite estabelecer uma ligacao com a shard para inserir o value e a key. 
 * Se na shard ja existir a key, o value é substituido. Caso contrario e' criado um novo_par. 
 * Recebe como argumentos a shard onde sera inserida a key e a value. 
 * Devolve NULL se for criado um novo_par, caso contrario devolve o value substituido. */
char* server_put(shard* sharde, char* key, char* value);

/* server_remove permite estabelecer uma ligacao com a shard para remover o value da respectiva key.
 * Se nao existir a key, nada e' feito na shard. Caso contrario remove o value e a key.
 * Recebe como argumentos a shard onde esta inserida a key, bem como a respectiva key.
 * Devolve NULL se nao existir a key na shard, caso contrario devolve o value removido.*/
char* server_remove(shard* sharde, char* key);

/* server_getAllKeys permite estabelecer uma ligacao com a shard para imprimir
 * todos os pares da shard. Caso a shard seja vazia, nada e' feito.*/
KV_t* server_getAllKeys(shard* sharde, buffer* pos_buffere, int* sizeKVArray);

#endif
