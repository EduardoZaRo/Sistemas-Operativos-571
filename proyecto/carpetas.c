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
	char * aux;
	aux = strtok(str, "\n");
	int c = 0;
	while(aux != NULL){
		if(c == 0){ c = 1; aux = strtok(NULL, "\n"); continue;}
		int carpindex = 0;
		char carpeta[500];
		for(int j = 0; j < strlen(aux);j++){
			if(	(j >= 0 && j <= 1) ||
				(j >= 2 && j <= 4) ||
				(j >= 7 && j <= 9) ||
				(j >= 12 && j <= 14) ||
				(j >= 17 && j <= 20) ||
				(j >= 21 && j <= 24))
			{carpeta[carpindex] = aux[j];carpindex ++;}
		}
		carpeta[carpindex] = 0;
		char *mkdir[] = {"mkdir", carpeta, NULL};
		pid_t child = fork();
		if(child == 0){
			execve("/bin/mkdir", mkdir, 0);
			exit(0);
		}
		else{
			printf("Creando carpeta: %s\n", carpeta);
			waitpid(child, NULL, 0);
		}
		aux = strtok(NULL, "\n");

	}
	return(0);
}
int leer_archivo(char * archivo, char * str){
	int leidos, delimitador = 0,lineas = 0, i =0;
	char c;
	str[0] = '\x0';
	int read_desc = open(archivo, O_RDONLY);
	do{
		leidos = read(read_desc, &c, 1);
		if(c != ':'){ str[i] = c;i++;}
		else {delimitador++;}
		if(delimitador == 4){delimitador = 0; lineas++;}
	}while((leidos != 0 && c != '\x0'));
	str[i] = '\x0';
	close(read_desc);
	return lineas;
}
