#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>

int leer_archivo(char * archivo, char * str){
	int leidos, espacios = 0,lineas = 0, i =0;
	char c;
	str[0] = '\x0';
	int read_desc = open(archivo, O_RDONLY);
	do{
		leidos = read(read_desc, &c, 1);
		if(c != ' '){ str[i] = c;i++;}
		else {espacios++;}
		if(espacios == 2){espacios = 0; lineas++;}
	}while((leidos != 0 && c != '\x0'));
	str[i] = '\x0';

	close(read_desc);
	return lineas;
}
int main(){
	int descriptor,leidos, cont=0;
	char carac, cad[25];
	int lineas = 0;
	char str[500];
	lineas = leer_archivo("ventas.txt", str);
	int  index = 0;
	for(int i = 0; i < lineas; i++){
		char aux[500];
		strncpy(aux, str + index, 9);
		aux[9] = 0;
		index += 10;
		int a = (aux[6]-'0')*100;
		int b = (aux[7]-'0')*10;
		int c = (aux[8]-'0');
		int total = (aux[5] - '0') * (a+b+c);
		printf("Total %d : %d\n", i, total);
	}
	//char *str[] = {"mkdir", "hola", NULL};
	//execve("/bin/mkdir",str,0);
	return(0);
}
