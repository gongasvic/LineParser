
#include <KV_t.h>

/************************** novo_par **********************************/

KV_t* novo_par(char* key,char* value) { 
  	KV_t* par=(KV_t*)calloc(1,sizeof(KV_t));
	strcpy(par->key,key);
  	strcpy(par->value,value);
  	return par;
}
