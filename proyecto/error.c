#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#include"error.h"

char* features[] = {
    "name",
    "lastP",
    "lastM",
    "year",
    "salary",
    "hours"
};

int values[] = {
    3,
    9,
    15,
    21,
    26,
    31
};

int lens[] = {
	5,
	5,
	5,
	4,
	4,
	2,
};

error_t validateParameters(int argv, int min,int max){
    if (argv < min)
        printf("\tError: Parametros incompletos!\n");
    else if(argv > max)
    	printf("\tError: Parametros inesesarios!\n");
    else 
        return ok;
    return error;
}

error_t selectfeature(char *feature, int * len){
	for (int i = 0; i < DATA_LEN; i++)
		if(!strcmp(feature, features[i]))
		{
			len[0] = values[i];
			len[1] = lens[i];
			return ok;
		}
	return error;
}

error_t validatedata(char * cmd, char * value){
	if (!strcmp(cmd, features[3]) ||
		!strcmp(cmd, features[4]) ||
		!strcmp(cmd, features[5])) 
	{
		if(atoi(value))
			return ok;
	} else 
		return ok;
	return error;
}

void getDatas(int *value,  int *len){
	for (int i = 0; i < 6; i++)
	{
		value[i] = values[i];
		len[i] = lens[i];
	}
}