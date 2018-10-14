#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 1000

int leitor(){

	FILE *file;
	char str[MAX];
	char* filename = "teste";
 
	file = fopen(filename, "rb");
	if (file == NULL){
		printf("Arquivo não pode ser aberto %s \n",filename);
        	return 1;
	}

	//este laço percorre todo o arquivo texto e imprime o seu conteúdo no terminal
	while (fgets(str, MAX, file) != NULL){
		printf("%s", str);
	}

	fclose(file);
	return 0;

}

int match(char *linha, char *padrao)
{
	int len = strlen(linha);
	linha[len - 1] = '\0';
	
	printf("Conteúdo de linha: %s \n", linha);
	printf("Conteúdo do padrao: %s \n", padrao);
	if (strcmp(linha, padrao) == 0) {
		return 1;
	} else {
		return -1;
	}
}

int patternMatching(char *linha, char *padrao)
{
	int len_linha = strlen(linha);
	int len_padrao = strlen(padrao);
	int len_buffer = 0;
	int len_controle = 0;
	int i = 0;
	int j = 0;
	char buffer[200];

	char conteudo_linha;
	char conteudo_padrao;

	linha[len_linha - 1] = '\0';

	if(len_padrao == 0){
		return -1;
	}

	//printf("Conteúdo de linha: %s \n", linha);
	//printf("Conteúdo do padrao: %s \n", padrao);
	
	//ideia do algoritmo para casos onde começa com *:
	if(padrao[j] == '*'){
		conteudo_linha = linha[i];
		conteudo_padrao = padrao[j+1];
		while(conteudo_linha != conteudo_padrao){
			buffer[i] = conteudo_linha;
			buffer[i+1] = '\0';
			i++;
			conteudo_linha = linha[i];
		}
		//printf("Conteúdo do buffer: %s \n", buffer);
		j++;
		len_buffer++;
		conteudo_padrao = padrao[j];
		while(conteudo_linha == conteudo_padrao && len_buffer < len_padrao){
			buffer[i] = conteudo_linha;
			buffer[i+1] = '\0';
			i++;
			j++;
			len_buffer++;
			conteudo_linha = linha[i];
			conteudo_padrao = padrao[j];
			//printf("Conteúdo do buffer 2.0: %s \n", buffer);
		}

		memset(buffer, 0, len_padrao);
		if(len_buffer == len_padrao){
			return 1;
		} else {
			return -1;
		}
	// quando começa com ?		
	} else if (padrao[j] == '?'){
		conteudo_linha = linha[i];
		conteudo_padrao = padrao[j];
		buffer[i] = conteudo_linha;
		buffer[i+1] = '\0';
		i++;
		conteudo_linha = linha[i];
		j++;
		len_buffer++;
		conteudo_padrao = padrao[j];
		while(conteudo_linha == conteudo_padrao && len_buffer < len_padrao){
			buffer[i] = conteudo_linha;
			buffer[i+1] = '\0';
			i++;
			j++;
			len_buffer++;
			conteudo_linha = linha[i];
			conteudo_padrao = padrao[j];
		}

		memset(buffer, 0, len_padrao);
		if(len_buffer == len_padrao){
			return 1;
		} else {
			return -1;
		}
	// quando o * ou a ? estão no meio da palavra
	} else {
		printf("Conteúdo de linha: %s \n", linha);
		while(len_controle < len_linha){
			len_controle++;
			conteudo_linha = linha[i];
			conteudo_padrao = padrao[j];
			//printf("Conteúdo da linha na posição %d: %c \n", i, conteudo_linha);
			while((conteudo_linha == conteudo_padrao || conteudo_padrao == '?' || conteudo_padrao == '*') && len_buffer < len_padrao){
				//printf("Entrei no while para verificar padrão \n");
				if(conteudo_padrao == conteudo_linha || conteudo_padrao == '?'){
					buffer[i] = conteudo_linha;
					buffer[i+1] = '\0';
					i++;
					j++;
					len_buffer++;
					conteudo_linha = linha[i];
					conteudo_padrao = padrao[j];
				} else {
					conteudo_linha = linha[i];
					conteudo_padrao = padrao[j+1];
					while(conteudo_linha != conteudo_padrao){
						buffer[i] = conteudo_linha;
						buffer[i+1] = '\0';
						i++;
						conteudo_linha = linha[i];
					}
					j++;
					len_buffer++;
					conteudo_padrao = padrao[j];
				}
			}
			memset(buffer, 0, len_padrao);
			i++;
			if(len_buffer == len_padrao){
				return 1;
			} else if(len_controle == len_linha) {
				return -1;
			}
		}
	}

	return 0;
}

int encontrarPalavra(char palavra[]){

	FILE *file;
	char *line = NULL;
	size_t length = 0;
	ssize_t read;
	char* filename = "teste";
	int retorno_match = 0;
	int count = 0;

	file = fopen(filename, "rb");
	if (file == NULL){
		printf("Arquivo não pode ser aberto %s \n", filename);
        	return 1;
	}

	while ((read = getline(&line, &length, file)) != -1) {
		retorno_match = patternMatching(line, palavra);
		if(retorno_match == 1){
			count++;
		}
		printf("Valor do retorno do match: %d \n", retorno_match);
    	}

	fclose(file);
    	if (line){
		free(line);
	}

	printf("Valor total de palavras com o padrao encontradas: %d \n", count);

}

int main(){

	//printf("Leitura do arquivo texto \n");
	//leitor();

	char str[100];

	printf( "Entre uma palavra para encontrar no arquivo texto: \n");
	scanf("%s", str);
	encontrarPalavra(str);

}
