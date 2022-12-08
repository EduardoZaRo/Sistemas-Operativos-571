#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#include"user.h"
#include"file.h"
#include"statistics.h"

int main(int argv, char* argc[]){
    file_t file, file2;
    user_t user;
    int size, descriptor, max, id;
    char buffer[7], aux[6];

    strcpy(file.path, "users.txt");
	file.conf = O_RDONLY;
	file.permissions = 0700;

    strcpy(file2.path, "report.txt");
	file2.conf = O_RDWR|O_APPEND|O_CREAT;
	file2.permissions = 0700;

    if(validateParameters(argv, 4, 5) == ok){
        descriptor = getDescriptor(file);
        max = getMaxID(descriptor);

        if (!strcmp(argc[1], "-g"))
        {
            strcpy(buffer, argc[4]);
            size = strlen(buffer);
            size = (size - size/2);
            close(descriptor);

            for (int i = 0; i < size; i++)
            {   
                sprintf(aux,"%c", buffer[(i)? (i * 2): i]);
                if(id = atoi(aux)){
                    if(id <= max){
                        user = getUser(file, id);
                        insert_user(user, file2);
                    } else {
                        printf("\tError: limite usuarios superado!\n");
                        exit(0);
                    }
                } else {
                    printf("\tError: No es un numero valido!\n");
                    exit(0);
                }

            }
            char res[200];
            
            if(getStadistics(file2,argc[2], argc[3], res) == ok){
                descriptor = getDescriptor(file2);
                write(descriptor, res, strlen(res));
                printf("\tExito: Reporte generado!\n");
            } else {
                printf("\tError: Reporte no se pudo generar!\n");
                exit(0);
            }
        } else {
            printf("\tError: No es un comando valido!\n");
        }
        close(descriptor);
    }

    return 0;
}
