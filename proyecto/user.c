#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "user.h"
#include "error.h"
#include "statistics.h"

int getMaxID(int descriptor)
{
	char caux[3];
	lseek(descriptor, -34, SEEK_END);
	read(descriptor, &caux, 3);
	return (atoi(caux));
}

error_t create_user(user_t *user, char *data[])
{
	user->id = 0;
	strcpy(user->name1, data[2]);
	strcpy(user->lastP, data[3]);
	strcpy(user->lastM, data[4]);

	if (!atoi(data[5]))
		return error;
	else
		strcpy(user->year, data[5]);

	if (!atoi(data[6]))
		return error;
	else
		strcpy(user->salary, data[6]);

	if (!atoi(data[7]))
		return error;
	else
		strcpy(user->hours, data[7]);

	return ok;
}

void print_user(user_t user)
{
	printf("Usuario = %d:%s:%s:%s:%s:%s:%s\n",
		   user.id,
		   user.name1,
		   user.lastP,
		   user.lastM,
		   user.salary,
		   user.year,
		   user.hours);
}

void process_datas(char *cad, int lim)
{
	int len, dif, i;
	char full[4], aux[20];
	aux[0] = 0;
	if ((len = strlen(cad)) <= lim)
	{
		dif = lim - len;
		for (i = 0; i < dif; i++)
			full[i] = 'X';
		full[i] = 0;
		strcat(cad, full);
	}
	cad[lim] = '\0';
}

void completeNumber(char *cad, int lim)
{
	int len = 0;
	char cero[MAX_LEN_NUM] = "0";
	len = strlen(cad);
	lim -= len;
	for (int i = 0; i < lim; i++)
	{
		strcat(cero, cad);
		strcpy(cad, cero);
		strcpy(cero, "0");
	}
}

void insert_user(user_t user, file_t file)
{
	int grabados, id, descriptor, i = 0;
	char buffer[MAX_LEN_T], otro, caux[35], cero = '0', c[3];
	buffer[0] = 0;
	if ((descriptor = getDescriptor(file)))
	{
		process_datas(user.name1, 5);
		process_datas(user.lastP, 5);
		process_datas(user.lastM, 5);
		completeNumber(user.year, 4);
		completeNumber(user.salary, 4);
		completeNumber(user.hours, 2);

		id = getMaxID(descriptor);
		id++;

		idtostring(c, id);

		if (descriptor)
		{
			sprintf(buffer, "%s:%s:%s:%s:%s:%s:%s\n", c,
					user.name1,
					user.lastP,
					user.lastM,
					user.year,
					user.salary,
					user.hours);
			grabados = write(descriptor, buffer, strlen(buffer));
			close(descriptor);
		}
		else
			close(descriptor);

	}
	else
		close(descriptor);
}

void idtostring(char* c, int id){
	if (id < 10)
		sprintf(c, "0%d", id);
	else
		sprintf(c, "%d", id);
}

void changeData(file_t file, feature_t* feature, int id, char* newData)
{
	int descriptor, max;
	char buffer[10];
	if ((descriptor = getDescriptor(file)))
	{
		max = getMaxID(descriptor);
		if(max >= id){
			lseek(descriptor, ((id) * 34) + feature[0], SEEK_SET);
			if(feature[1] == 5)
				process_datas(newData, feature[1]);
			else
				completeNumber(newData, feature[1]);
			
			lseek(descriptor, ((id) * 34) + feature[0], SEEK_SET);

			write(descriptor, newData, feature[1]);	
			close(descriptor);	
			printf("\tExito: Cambio realizado!\n");
		} else 
			printf("\tError: id no existe!\n");

	}
}

user_t getUser(file_t file, int id){
	user_t user;
	int descriptor = getDescriptor(file);
	char data[6][6];
	char buffer[7];
	
	int value[6], len[6];
	getDatas(value, len);

	for (int i = 0; i < DATA_LEN; i++)
	{
		lseek(descriptor, ((id) * 34) + value[i], SEEK_SET);
		read(descriptor, buffer, len[i]);
		buffer[len[i]] = 0;
		strcpy(data[i], buffer);
	}
	close(descriptor);

	user.id = id;
	strcpy(user.name1, data[0]);
	strcpy(user.lastP, data[1]);
	strcpy(user.lastM, data[2]);
	strcpy(user.year, data[3]);
	strcpy(user.salary, data[4]);
	strcpy(user.hours, data[5]);

	return user;
}