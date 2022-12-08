#ifndef USER_H
#define USER_H

#include "file.h"
#include "error.h"

#define MAX_LEN_N   9
#define MAX_LEN_NUM 6
#define MAX_LEN_H   2

#define DATA_LEN    6

typedef struct user{
    int id;
	char name1[MAX_LEN_N];
	char lastP[MAX_LEN_N];
    char lastM[MAX_LEN_N];
    char year[MAX_LEN_NUM];
    char salary[MAX_LEN_NUM];
    char hours[MAX_LEN_H];
}user_t;

typedef int feature_t;

/* Gestionar usuarios*/
int getMaxID(int descriptor);
error_t create_user(user_t *user, char *data[]);
void print_user(user_t user);
void insert_user(user_t user, file_t file); 

/* Completar cadenas para cuadrarlas*/
void process_datas(char *cad, int lim); 
void completeNumber(char* cad, int lim);
void changeData(file_t file, feature_t* feature, int id, char* newData);
void idtostring(char* c, int id);
user_t getUser(file_t file, int id);

#endif