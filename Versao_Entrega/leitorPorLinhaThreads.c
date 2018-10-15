/*--------------------------------------------------------------------------|
Autores:                Raffaela Monteiro & Renata Urbanski                 |
Faculdade:              Escola Politécnica - PUCRS                          |
Trabalho:               Enunciado T1: Pattern Matching em OpenMP            |
Atualização:  			15 - 10 - 2018                                      |
Resultados:				1 - sequential	   ms		                        |
----------------------------------------------------------------------------*/

#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

long timediff(clock_t t1, clock_t t2) 
{
    long elapsed;
    elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000000;
    return elapsed;
}

bool wildCmp(char *padrao, char *palavra)  //http://www.firmcodes.com/write-c-program-wildcard-pattern-matching-algorithm/
{
	//printf("-- Padrão: %s --\n", pattern);
	//printf("-- Palavra: %s --\n", string);
	if(*padrao=='\0' && *palavra=='\0')	// Verifica se acabou a palavra e o padrão
		return true;
		
	if(*padrao=='?' || *padrao==*palavra)	// Verifica se é um '?' (um unico caracter faltante), ou se deu match
		return wildCmp(padrao+1,palavra+1);
		
	if(*padrao=='*') // Verifica se é um '*' (multiplos caracteres faltantes) e se a palavra não está vazia
		return wildCmp(padrao+1,palavra) || wildCmp(padrao,palavra+1);
		
	return false;
}

int encontrarPalavra(char palavra[])
{

	FILE *file;
	char *line = NULL;
	size_t length = 0;
	ssize_t read;
	char* filename = "teste";
	int count = 0;
	
	clock_t t1, t2;
    long elapsed;

	file = fopen(filename, "rb");
	if (file == NULL){
		printf("Arquivo não pode ser aberto %s \n", filename);
        	return 1;
	}
	
	t1 = clock();

	char *palavras;


	while ((read = getline(&line, &length, file)) != -1) {
		// Marca o fim da linha
		int len_linha = strlen(line);
		line[len_linha - 1] = '\0';
		
		//printf("thread: %d - len linha: %d - linha: %s\n",tid, len_linha, line);
		//printf("Linha: %s\n", line);

		// A thread vai quebrar a linha em um vetor de palavras
		char * palavraSeparada = strtok(line," ,.-");

		// Enquanto a linha não acabar
		while (palavraSeparada != NULL && palavraSeparada != "") {
			//printf("%s\n",palavraSeparada);
			// A thread vai verificar se a palavra corresponde ao padrao, incrementando o count caso positivo
			if(wildCmp(palavra, palavraSeparada)) {
				omp_set_lock(&lockb);
					count++;
					printf("Thread %d achou a %dª ocorrência!\n", tid, count);
				omp_unset_lock(&lockb);
			}
			
			// pega a proxima palavra da linha
			palavraSeparada = strtok(NULL, " ,.-");
		}

	}















	int nthreads = 2;
	int tid = -1;
	omp_lock_t locka, lockb;

	omp_init_lock(&locka);
	omp_init_lock(&lockb);
	#pragma omp parallel if(nthreads >= 1) num_threads(nthreads) private(tid, line, length) shared(nthreads, file, count, locka, lockb)
	{
		// Obtem o identificador de thread
		tid = omp_get_thread_num();
			//printf("Thread %d: Hello!\n", tid);

		if (nthreads != omp_get_num_threads())
			printf("Error: NTHREADS\n");			

		// A thread pega uma linha para processar
		omp_set_lock(&locka);
		while ((read = getline(&line, &length, file)) != -1) {
		omp_unset_lock(&locka);
			// Marca o fim da linha
			int len_linha = strlen(line);
			line[len_linha - 1] = '\0';
			
			//printf("thread: %d - len linha: %d - linha: %s\n",tid, len_linha, line);
			//printf("Linha: %s\n", line);

			// A thread vai quebrar a linha em um vetor de palavras
			char * palavraSeparada = strtok(line," ,.-");

			// Enquanto a linha não acabar
			while (palavraSeparada != NULL && palavraSeparada != "") {
				//printf("%s\n",palavraSeparada);
				// A thread vai verificar se a palavra corresponde ao padrao, incrementando o count caso positivo
				if(wildCmp(palavra, palavraSeparada)) {
					omp_set_lock(&lockb);
						count++;
						printf("Thread %d achou a %dª ocorrência!\n", tid, count);
					omp_unset_lock(&lockb);
				}
				
				// pega a proxima palavra da linha
				palavraSeparada = strtok(NULL, " ,.-");
			}

		}
		//printf("Thread %d: Bye!\n", tid);

		//printf("Valor do retorno do match: %d \n", retorno_match);
	}
	omp_destroy_lock(&locka);
	omp_destroy_lock(&lockb);
		
	fclose(file);
    	if (line){
		free(line);
	}

	printf("Valor total de palavras com o padrao encontradas: %d \n", count);
	
	t2 = clock();
    elapsed = timediff(t1, t2);
    printf("\n\nelapsed: %ld us\n", elapsed);

}

int main(){

	//printf("Leitura do arquivo texto \n");
	//leitor();

	char str[100];

	printf( "Entre uma palavra para encontrar no arquivo texto: \n");
	scanf("%s", str);
	encontrarPalavra(str);

}
