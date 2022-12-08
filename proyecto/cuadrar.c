#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
int leer_archivo(char * archivo, char * str);
int main(){
	int descriptor,leidos, cont=0;
	char carac, cad[25];
	int lineas = 0;
	char str[500];
	lineas = leer_archivo("users.txt", str);
	int index = 0;
	int i, j, k = 0;
	char *aux;
	aux[0] = 0;
	aux = strtok(str, "\n");
	char sub[100] = {0};
	char * header = "ID:NAMEX:LASTP:LASTM:YEAR:SALA:HR";
	while(aux != NULL){
		if(strcmp(aux, header) == 0){
			aux = strtok(NULL, "\n");
			strcat(sub, header);
			strcat(sub, "\n");
			continue;
		}
		char word[100];
		int index = 0;
		int col = 0;
		for(int i = 0; col < 7; i++){
			word[index] = aux[i];
			if(aux[i] == ':' || aux[i] == 0){
				word[index] = 0;
				char a[100];
				if(col == 0){
					if(strlen(word) > 2)
						sprintf(a, "%-2.*s:",2, &word[index-2]);
					else 	sprintf(a, "%02d:", atoi(word));
				}else{
					if(col >= 1 && col <= 3){
						if(strlen(word) > 5)
						 	sprintf(a, "%-5.*s:",5, word);
						else sprintf(a, "%-5s:", word);
					}else{
						if(col >= 4 && col <= 5){//Interesan los ultimos caracteres, no los primers
							if(strlen(word) > 4)
								sprintf(a, "%-4.*s:",4, &word[index-4]);
							else 	sprintf(a, "%04d:", atoi(word));
						}else
							if(strlen(word) > 2)
								sprintf(a, "%-2.*s\n",2, &word[index-2]);
							else 	sprintf(a, "%02d\n", atoi(word));
					}
				}
				strcat(sub, a);
				index = 0;
				col++;
				continue;
			}
			index++;
		}
		aux = strtok(NULL, "\n");
	}

	descriptor = open("users.txt", O_WRONLY | O_CREAT | O_TRUNC, 0700);
	int grabados = write(descriptor, sub, strlen(sub));
	close(descriptor);
	return(0);
}
int leer_archivo(char * archivo, char * str){
	int leidos, delimitador = 0,lineas = 0, i =0;
	char c;
	str[0] = '\x0';
	int read_desc = open(archivo, O_RDONLY);
	do{
		leidos = read(read_desc, &c, 1);
		if(c != '\n'){ str[i] = c;i++;}
		else {lineas++; str[i] = c; i++;}
	}while((leidos != 0 && c != '\x0'));
	lineas--;
	str[i] = '\n';
	str[i++] = '\x0';
	close(read_desc);
	//printf("Lineas: %d | bytes = %d\n", lineas, i);
	return lineas;
}
