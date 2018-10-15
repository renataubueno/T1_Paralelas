/*--------------------------------------------------------------------------|
Autores:                Raffaela Monteiro & Renata Urbanski                 |
Faculdade:              Escola Politécnica - PUCRS                          |
Trabalho:               Enunciado T1: Pattern Matching em OpenMP            |
Atualização:  			15 - 10 - 2018                                      |
Resultados:				1 - sequential	   ms		                        |
----------------------------------------------------------------------------*/

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


	while ((read = getline(&line, &length, file)) != -1) {
		//manda um thread processar a linha
		
		// Marca o fim da linha
		int len_linha = strlen(line);
		line[len_linha - 1] = '\0';

		// A thread vai quebrar a linha em um vetor de palavras
		char * pch;
		//printf("Linha: %s\n", line);
		pch = strtok(line," ,.-");

		// Enquanto a linha não acabar
		while (pch != NULL && pch != "") {
			//printf("%s\n",pch);
			// A thread vai verificar se a palavra corresponde ao padrao, incrementando o count caso positivo
			if(wildCmp(palavra, pch)) {
				count++;
				printf("Achou até agora: %d:\n", count);
			}

			// pega a proxima palavra da linha
			pch = strtok(NULL, " ,.-");
		}

		//printf("Valor do retorno do match: %d \n", retorno_match);
    }
		
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
