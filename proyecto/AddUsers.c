#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#include"user.h"
#include"file.h"
#include"error.h"

error_t addUser(file_t file, user_t user, char *data[], int size);
error_t editUser(file_t file, char *data[], int *len, int size);

int main(int argv, char * argc[]){
	/* Estructuras */
	user_t user;
	file_t file;

	/* Configuracion del archivo */
	strcpy(file.path, "users.txt");
	file.conf = O_RDWR|O_CREAT|O_APPEND;
	file.permissions = 0700;

	/* Seleccion de comando interno */
	if (!strcmp(argc[1], "-add"))
	{
		file.conf = O_RDWR|O_CREAT|O_APPEND;
		addUser(file, user, argc, argv);
	} 
	else if (!strcmp(argc[1], "-edit"))
	{
		int len[2];
		file.conf = O_RDWR;
		editUser(file,argc,len, argv);
	} else 
		printf("\tError: Comando no valido!\n");

    return 0;
}

error_t addUser(file_t file, user_t user, char *data[], int size){
	/* Verificacion de los datos ingresados */

	if(validateParameters(size, 8, 9) == ok){
		if(create_user(&user, data) == ok){
			printf("\tExito: Usuario Creado Correctamente!\n");
			insert_user(user, file);
			return ok;
		} else
			printf("\tError: Error al crear el usuario!\n");
	}
	return error;
}	

error_t editUser(file_t file, char *data[], int *len, int size){
	/* Verifica los datos ingresados */
	if (validateParameters(size, 5, 6) == ok)
	{
		if (selectfeature(data[3], len) == ok)
			if(validatedata(data[3],data[4]) == ok){
				changeData(file, len, atoi(data[2]), data[4]);
				return ok;
			}
			else
				printf("\tError valor no valido\n");
	}
	return error;
}