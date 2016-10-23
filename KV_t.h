#ifndef KV_T_H
#define KV_T_H 1

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <kos_client.h>
#include <string.h>
#include <pthread.h>



/* novo_par cria um novo par, com uma chave e um valor, para mais tarde 
 * inserir na hash da database. */
KV_t* novo_par(char* key,char* value);

#endif
