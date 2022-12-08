#ifndef STATISTICS_H
#define STATISTICS_H

#include "user.h"
#include "file.h"

#define DATA_SIZE   30

typedef struct statistic{

    int data[DATA_SIZE];
    int size;
    float mean;
    float median;
    float mode;
    int max;
    int min;
}statistic_t;

error_t getStadistics(file_t file, char data1[] , char data2[], char* cad);
void getColum(file_t file, feature_t* feature, statistic_t* data);
void printData(statistic_t data);
void orderdata(statistic_t *data);
void printAll(statistic_t data, char* cad, char * cmd);
void setStatistics(statistic_t* data);

#endif