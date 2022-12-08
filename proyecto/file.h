#ifndef FILE_H
#define FILE_H

#define MAX_LEN_T 	250

typedef struct file{
	char path[MAX_LEN_T];
	int conf;
	int permissions;
}file_t;

int getDescriptor(file_t file);
void showAll(file_t file);

#endif