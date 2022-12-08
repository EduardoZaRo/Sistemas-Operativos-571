#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#include "file.h"
#include "user.h"

int getDescriptor(file_t file){
	int descriptor;
	descriptor=open(file.path,file.conf,file.permissions);
	return (descriptor == 0)? 0: descriptor;
}

void showAll(file_t file){
	char data[MAX_LEN_T];
	int descriptor, i, max;

	if((descriptor = getDescriptor(file))){
		max = getMaxID(descriptor);
		for(i = 0; i < max; i++ ) {
			lseek(descriptor, (i*34), SEEK_SET);
			read(descriptor, data, 34);
			data[34] = 0;
			printf("%s", data);
		}
	}
	close(descriptor);
}
