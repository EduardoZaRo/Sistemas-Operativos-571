#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "statistics.h"

char* listcmd[] = {
    "-mean",
    "-median",
    "-mode",
    "-max",
    "-min",
    "-all"
};

error_t getStadistics(file_t file, char data1[] , char data2[], char* cad){
    int len[2];
    statistic_t statistic;
    if (selectfeature(data2, len) == ok){
        getColum(file, len, &statistic);
        orderdata(&statistic);
        setStatistics(&statistic);
        
        if(!strcmp(data1, listcmd[0])){
            sprintf(cad,"Column: %s\nResult:\n \tMean: %f\n", data2, statistic.mean);
        } else if(!strcmp(data1, listcmd[1])){
            sprintf(cad,"Column: %s\nResult:\n \tMedian: %f\n", data2, statistic.median);
        } else if(!strcmp(data1, listcmd[2])){
            sprintf(cad,"Column: %s\nResult:\n \tMode: %f\n", data2, statistic.mode);
        } else if(!strcmp(data1, listcmd[3])){
            sprintf(cad,"Column: %s\nResult:\n \tMax: %d\n", data2, statistic.max);
        } else if(!strcmp(data1, listcmd[4])){
            sprintf(cad,"Column: %s\nResult:\n \tMin: %d\n", data2, statistic.min);
        } else if(!strcmp(data1, listcmd[5])){
            printAll(statistic, cad, data2);
        } else {
            printf("\tError: Comando no valido!\n");
            return error;
        }
    } else {
        printf("\tError: Columna no valida!\n");
        return error;
    }
	
    return ok;
}

void getColum(file_t file, feature_t* feature, statistic_t* data)
{
	int descriptor, max;
	char buffer[10];
	if ((descriptor = getDescriptor(file)))
	{
		max = getMaxID(descriptor);
        data->size = max + 1;
		for (int i = 0; i < max + 1; i++)
		{
			lseek(descriptor, (i * 34) + feature[0], SEEK_SET);
			read(descriptor, buffer, feature[1]);
           
			data->data[i]= atoi(buffer);
        }
	}
    close(descriptor);	
}

void mean(statistic_t *data){
    float res;
    int max =data->size;
    for (int i = 0; i < max; i++)
        res+= (float) data->data[i];
    data->mean = (res / max);
}

void median(statistic_t *data){
    float res;
    int f;
    int max = data->size;
    if (max%2)
        res = (float) ((data->data[max/2] + data->data[max/2 + 1] )/ 2); 
    else 
        res = (float) ((data->data[max/2])); 
    data->median = res;
}

void mode(statistic_t *data){
    float res;
    int max = data->size;
    int aux[max];

    for(int i = 0; i < max; i++) {
        aux[i]=0;
    }

    for(int i =0; i < max; i++) {
        int numero = data->data[i];
        int posicion = i;
        for(int j = i; j < max; j++) {
            if(data->data[j]==numero) aux[posicion]++;
        }
    }

    int mayor=aux[0];
    int posicionmayor = 0;
    for(int j = 0; j < max ; j++) {
        if(aux[j] > mayor) {
            posicionmayor = j;
            mayor = aux[j];
        }
    }
    data->mode = data->data[posicionmayor];
}

void orderdata(statistic_t* data){
    int temp, max = data->size;
    int *aux = data->data;

    for (int i=1;i< max;i++)
        for (int j=0; j < max-i ;j++)
            if (aux[j] > aux[j+1])
            {
                temp = aux[j];
                aux[j] = aux[j+1];
                aux[j+1] = temp;
            }
    for (int i = 0; i < max; i++)
        data->data[i] = aux[i];
}

void printData(statistic_t data){
    for (int i = 0; i < data.size; i++)
        printf(":%d:\n", data.data[i]);
}

void setStatistics(statistic_t* data){
    int max = data->size;
    mean(data);
    median(data);
    mode(data);
    data->max = data->data[max - 1];
    data->min = data->data[1];
}

void printAll(statistic_t data, char* cad, char * cmd){
    sprintf(cad,"Column: %s\nAll data:\n \tMax: %d\n \tMin: %d\n \tMean: %.2f\n \tMedian: %.2f\n \tMode: %.2f\n \tSize: %d\n", 
            cmd,
            data.max, 
            data.min, 
            data.mean, 
            data.median,
            data.mode, 
            data.size);
}