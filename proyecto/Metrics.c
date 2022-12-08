#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#include"user.h"
#include"file.h"
#include"statistics.h"
#include"error.h"

int main(int argv, char * argc[]){
    file_t file;
    char res[200];
    strcpy(file.path, "users.txt");
    file.conf = O_RDONLY;
	file.permissions = 0700;

    if(validateParameters(argv, 3, 4) == ok){
        getStadistics(file, argc[1], argc[2], res);
        printf("%s", res);
    }
    return 0;
}