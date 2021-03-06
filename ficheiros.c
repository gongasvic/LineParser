#include <hash.h>

#define EOFILE 0
#define SIZE 20

int zeros=0;

file* file_init(file* files){
	files = (file*) malloc(sizeof(file));
	files->name = (char*)calloc(sizeof(char),1);
	files->maxlinha = 1;
	return files;
}


int Read_File(file* files,shard* sharde){
	int linha=0;//num linhas
	KV_t* par;
	char valor[SIZE],chave[SIZE];
	
	char nada[SIZE];
	bzero(nada,SIZE);
	
	lseek(files->ficheiro,0,SEEK_SET);

	while(read(files->ficheiro,chave,SIZE)!=0){
		read(files->ficheiro,valor,SIZE);
		if(memcmp(nada,chave,SIZE)!=0){
			par=novo_par(chave,valor);

			if(HashSearch(sharde,par->key)!=NULL){
				++linha;
				continue;
			}
			HashInsert(sharde, par);
			++linha;
			continue;
		}
		zeros=1;
	}

	return linha;

}
void optimizador_de_espaco(file* files,shard* sharde){
	
	int linhas=Read_File(files,sharde);
	if(zeros==0)
		return;
	char ficheiro[SIZE*2*linhas];

	char valor[SIZE],chave[SIZE];
	
	char nada[SIZE];
	bzero(nada,SIZE);
	bzero(ficheiro,SIZE*2*linhas);

	lseek(files->ficheiro,0,SEEK_SET);

	while(memcmp(nada,chave,SIZE)==0){
		read(files->ficheiro,chave,SIZE);
		read(files->ficheiro,valor,SIZE);
	}
	
	memcpy(ficheiro,chave,SIZE);
	memcpy(ficheiro,valor,SIZE);
	while(read(files->ficheiro,chave,SIZE)!=0){
		read(files->ficheiro,valor,SIZE);
		if(memcmp(nada,chave,SIZE)!=0){
			memcpy(ficheiro,chave,SIZE);
			memcpy(ficheiro,valor,SIZE);
		}
	}

	close(files->ficheiro);
	open(files->name, O_CREAT|O_RDWR|O_TRUNC , S_IRUSR|S_IWUSR);
	write(files->ficheiro, ficheiro,SIZE*2*linhas);
}


void Write_File(file* files, KV_t* pare){
	/*no original o lseek e para o SEEK_END*/
	lseek(files->ficheiro,0,SEEK_SET);
	char chave[SIZE],valor[SIZE];
	int flag=0;
	bzero(chave,SIZE);
	bzero(valor,SIZE);
	/*escrever por cima de linhas apagadas*/
	while(read(files->ficheiro,chave,SIZE)!=0)
		if(memcmp(valor,chave,SIZE)==0){
			flag = 1;
			break;
		}
	lseek(files->ficheiro,-20,SEEK_CUR);
	if(!flag)
		lseek(files->ficheiro,0,SEEK_END);
	/*ate aqui o original comeca na 58 outravez*/
	memcpy(chave,pare->key,SIZE);
	write(files->ficheiro,chave,SIZE);
	memcpy(valor,pare->value,SIZE);
	write(files->ficheiro,valor,SIZE);

}

int Search_File(shard* sharde, KV_t* pare){
	file* files = sharde->fich;
	int linha=0;//num linhas

	char pare_key[SIZE],chave[SIZE];
	bzero(pare_key,SIZE);
	memcpy(pare_key,pare->key,SIZE);

	lseek(files->ficheiro,0,SEEK_SET);

	while(read(files->ficheiro,chave,SIZE)!=0){
		if(memcmp(pare_key,chave,SIZE)==0)
			return linha;
		lseek(files->ficheiro,SIZE,SEEK_CUR);
		++linha;
	}


	return -1;
}

void Remove_Line(shard* sharde, KV_t* pare){
	int linha=Search_File(sharde,pare);
	file* files = sharde->fich;

	char conteudo[SIZE*2];
	bzero(conteudo,SIZE*2);

	if(linha!=-1){
		lseek(files->ficheiro,linha*SIZE*2,SEEK_SET);
		write(files->ficheiro,conteudo,SIZE*2);
	}
}



char* Find_Name(int shardId){
	int x,w=1,n=1, c=0;
	for(x=shardId; x>=10; x=x/10,w++);
	char* nome = (char*)calloc(1,w+5*sizeof(char));
	strcpy(nome,"f");
	for(c=shardId; w>0; c=c/10,w--){
		n=c%10;
		nome[w]=('0'+n);
	}
	strcat(nome, ".txt");
	return nome;

}

void Altera_Value(shard* sharde, KV_t* pare){
	int linha=0;
	file* files = sharde->fich;
	char conteudo[SIZE];
	bzero(conteudo,SIZE);

	linha = Search_File(sharde, pare);//encontra a linha

	if(linha!=-1){
		lseek(files->ficheiro,linha*SIZE*2+SIZE,SEEK_SET);
		memcpy(conteudo,pare->value,SIZE);
		write(files->ficheiro,conteudo,SIZE);
	}
}
