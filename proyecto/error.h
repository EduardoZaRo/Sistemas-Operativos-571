#ifndef ERROR_H
#define ERROR_H

#define DATA_LEN    6

typedef enum error {
    ok = 0,
    error = 1,
} error_t;

error_t validateParameters(int argv, int min,int max);
error_t selectfeature(char *feature, int * values);
error_t validatedata(char * cmd,char * value);
void getDatas(int *value,  int *len);

#endif