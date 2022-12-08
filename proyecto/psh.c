#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXCHARS 1000 // max chars recibidos
#define MAXCOMS 100 // max comandos
#define MAXPIPED 5

/*
Compilar con:
	gcc psh.c -L/usr/include -lreadline -o psh
	Si no se tiene readline pos hay que instalar vea
Ejecutar con: ./psh
Salir con: ctrl + c
Este shell puede:
	-Ejecutar comandos individuales con argumentos
	-Ejecutar comandos entubados con argumentos
		*Cuantos? Depende de MAXPIPED
		*Como? Usando una funcion que crea un hijo por comando entubado
		 y pasa la salida de uno a la entrada del siguiente
	-Ejecutar nuestros comandos
		*Usar comando 'ayudita' para mas info

Ejemplo de uso:
	ls | sort | grep <coincidencia>
*/
void clear();
int leerLinea(char*);
int parsePipe(char *, char ** strpiped);
void parseWord(char *, char **);
void comandoSimple(char **);
void comandosPipes_Generalizado(char **, int);

int main(){
	char entrada[MAXCHARS], *parsedArgs[MAXCOMS];
	char* parsedArgsPiped[MAXCOMS];
	clear();
	puts("Para abrir la ayuda use el comando 'ayudita'");
	while (1) {
		if (leerLinea(entrada))
		    continue;
		char * strpiped[MAXPIPED];
		int piped = parsePipe(entrada, strpiped);
		//printf("%d\n", piped);
		if(piped){
			if(piped >=  MAXPIPED){ //Sin este if se puede dar violacion de segmento
				puts("Maxima cantidad de comandos pipeados :(");
				continue;
			}
			comandosPipes_Generalizado(strpiped, piped);
		}
		else{
			parseWord(entrada, parsedArgs);
			comandoSimple(parsedArgs);
		}
	}
	return 0;
}

//Limpia pantalla y posiciona cursor en la coordenada 0,0
void clear(){puts("\e[2J"); puts("\e[0;0H"); }

int leerLinea(char* str){
	char* entrada;
	char* username = getenv("USER");
	char aux[100];
	sprintf(aux, "%s:psh $>>", username);
	entrada = readline(aux);
	if (strlen(entrada) != 0) {
		add_history(entrada);
		strcpy(str, entrada);
		return 0;
	}else return 1;
}

int parsePipe(char* str, char** strpiped){
	/*
		Se podria usar while((token = strsep(&str, "|")) != NULL)
		pero se tendria que usar memoria dinamica o tener un contador 
		en el while
	*/
	for(int i = 0; i < MAXPIPED; i++) {
		strpiped[i] = strsep(&str, "|");
		//Si strpiped[1] tiene algo es que hay al menos 1 pipe
		if (strpiped[i] == NULL) return strpiped[1] == NULL ? 0 : i;
	}
}

void parseWord(char* str, char** parsed){
	int i;
	for (i = 0; i < MAXCOMS; i++) {
		parsed[i] = strsep(&str, " ");
		if (parsed[i] == NULL) break;
		if (strlen(parsed[i]) == 0) i--;
	}
}
void nuestrosComandos(char ** parsed){
	if(strcmp(parsed[0], "cuadrar") == 0){
		execl("./cuadrar", NULL, 0);
		exit(0);
	}
	if(strcmp(parsed[0], "carpetas") == 0){
		execl("./carpetas", NULL, 0);
		exit(0);
	}
	if(strcmp(parsed[0], "addu") == 0){
		execve("./user", parsed, 0);
		exit(0);
	}
	if(strcmp(parsed[0], "metricas") == 0){
		execve("./metrics", parsed, 0);
		exit(0);
	}
	if(strcmp(parsed[0], "reporte") == 0){
		execve("./reporter", parsed, 0);
		exit(0);
	}
	if(strcmp(parsed[0], "ayudita") == 0){
		printf(	"Comandos disponibles:\n"
			"\t1)Cuadrar archivos (cuadrar)\n"
			"\t2)Crear carpetas (carpetas)\n"
			"\t3)Agregar usuario (addu)\n"
			"\t4)Metricas (metricas)\n"
			"\t5)Reportes (reporte)\n"
			"\tTodos los comandos del sistema!\n");
		exit(0);
	}


}
void comandoSimple(char **parsed){
	pid_t pid;
	if(pid = fork() == 0){
		//Si el comando no es del sistema puede ser de este shell
		if(execvp(parsed[0], parsed) == -1){
			nuestrosComandos(parsed);
			puts("Comando no existente! Para ver comandos diponibles use 'ayudita'");
		}
		exit(0);
	}
	else if(pid == -1){
		puts("Error fork");
	}
	else{
		wait(NULL);
	}
}

void comandosPipes_Generalizado(char ** strpiped, int piped){
	pid_t main_fork;
	if(main_fork = fork() == 0){
		char entrada[MAXCHARS], *parsedArgs[MAXCOMS];
		char* parsedArgsPiped[MAXCOMS];
		int i, fd[2];
		int prevpipe = STDIN_FILENO;
		for(i = 0; i < (piped-1); i++){
			parseWord(strpiped[i], parsedArgs);
			pid_t pid;
			if(pipe(fd) == -1){puts("Error pipe"); exit(1);}
			if(pid = fork() == 0){
				if(prevpipe != STDIN_FILENO){
					dup2(prevpipe, STDIN_FILENO);
					close(prevpipe);
				}
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
				if(execvp(parsedArgs[0], parsedArgs) == -1){
					//Aca van nuestros comandos
					nuestrosComandos(parsedArgs);
					puts("Comando no existente! Para ver comandos diponibles use 'ayudita'");
					exit(1);
				}
			}else if(pid == -1){
				puts("Error fork");
			}
			else{
				close(prevpipe);
				close(fd[1]);
				prevpipe = fd[0];
				waitpid(pid, NULL, 0);
			}
		}
		pid_t pid;
		parseWord(strpiped[i], parsedArgs);
		if(prevpipe != STDIN_FILENO){
			dup2(prevpipe, STDIN_FILENO);
			close(prevpipe);
		}
		if(execvp(parsedArgs[0], parsedArgs) == -1){
			//Aca van nuestros comandos
			nuestrosComandos(parsedArgs);
			puts("Comando no existente! Para ver comandos diponibles use 'ayudita'");
			exit(1);
		}
	}
	else if(main_fork == -1){
		puts("Error main fork pipe");
	}
	else{wait(NULL);}
}
