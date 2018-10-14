#include <stdio.h>
#include <string.h>

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

//encontrar uma palavra inteira
int encontrarPalavra(char palavra[]){

	FILE *file;
	char str[MAX];
	char c;
	char* filename = "teste";
	int count = 0;
	int palavra_len = strlen(palavra);
	int control = 0;
	char buf[palavra_len];

	file = fopen(filename, "rb");
	if (file == NULL){
		printf("Arquivo não pode ser aberto %s \n",filename);
        	return -1;
	}

	printf("Palavra para ser encontrada: %s \n", palavra);
	printf("Palavra no buffer no início: %s \n", buf);
	printf("Tamanho da palavra: %d \n", palavra_len);
	while((c = fgetc(file)) != EOF)
	{
		if (c == palavra[control])
		{
			while(c == palavra[control] && control != palavra_len){
				buf[control] = c;
				buf[control+1] = '\0';
				control++;
				printf("Palavra no buffer: %s \n", buf);
				printf("Char no momento: %c \n", c);
				printf("Control: %d \n", control);
			}
			
			if (strcmp(buf,palavra) == 0){
				printf("Entrei na comparação \n");
				printf("%s \n", buf);
				control = 0;
				memset(buf, 0, palavra_len);
				count++;
			}
		}
	}

	printf("Tamanho da palavra '%s': %d \n", palavra, palavra_len);
	printf("Quantidade de palavras '%s' encontradas: %d \n", palavra, count);
	return count;
}

//encontrar um padrão, por exemplo al*o deve encontrar qualquer palavra que comece com 'al' e termine com 'o'
//int encontrarPadrao(){

//}

int main(){

	//printf("Leitura do arquivo texto \n");
	//leitor();

	char str[100];

	printf( "Entre uma palavra para encontrar no arquivo texto: \n");
	scanf("%s", str);
	encontrarPalavra(str);

}
