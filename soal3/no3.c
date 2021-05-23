#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[1000000];
pid_t child;
char cwd[1000];
char folder[1000];
char deepfolder[1000];
char deepfolder2[1000];
int err;

void *move(void *arg);
void minf(int argc, char *argv[]);
void other(int argc, char *argv[]);
void dalam();

int main(int argc, char **argv) {
	getcwd(cwd, sizeof(cwd));
	memset(folder, '\0', sizeof(folder));
	memset(deepfolder, '\0', sizeof(deepfolder));
	memset(deepfolder2, '\0', sizeof(deepfolder2));
	if(argc < 2) {
		printf("Butuh Argumen -f atau -d atau \\*\n");
		return 0;
	}
	if(strcmp(argv[1], "-f") == 0) {
		minf(argc,argv);
	}
	else {
		other(argc,argv);
	}
	return 0;
}

void *move(void *arg) {
	char *filepath = (char *)arg;
	char *extension = NULL;
	extension = strchr(filepath, '.');
	char ext[1000];
	memset(ext, '\0', sizeof(ext));
	
	if((extension-filepath-strlen(cwd)+1)==2 || (extension-filepath+1)==1) strcpy(ext, "Hidden");
	else if(extension) {
		extension++;
        for(int i=0; i<strlen(extension); i++) {
               	ext[i] = tolower(extension[i]);
    	}
	}
	else strcpy(ext, "Unknown");
	
    char *filename = NULL;
    filename = strrchr(filepath, '/');
    if(filename) filename++;
	else filename = filepath;

	char folderpath[1000];
	strcpy(folderpath, cwd);
	strcat(folderpath, "/");
	strcat(folderpath, ext);
	mkdir(folderpath, S_IRWXU);
	
	if(strlen(deepfolder2)>1){
		char fullname[1000];

		strcpy(fullname, deepfolder2);
		strcat(fullname, "/");
		strcat(fullname, filename);
		strcat(folderpath, "/");
		strcat(folderpath, filename);
	rename(fullname, folderpath);
	}
	else if(strlen(folder) > 1) {
		char fullname[1000];

		strcpy(fullname, folder);
		strcat(fullname, "/");
		strcat(fullname, filename);
		strcat(folderpath, "/");
		strcat(folderpath, filename);
	rename(fullname, folderpath);
	}
	else {					
	    strcat(folderpath, "/");
        strcat(folderpath, filename);
		rename(filepath, folderpath);
	}
	return NULL;
}

void minf(int argc, char *argv[]){
	for(int i=2; i<argc; i++) {
		err = pthread_create(&tid[i], NULL, move, (void *)argv[i]);
		if(err != 0) printf("File %d : Sad, gagal :(\n", i-1);
        else printf("File %d : Berhasil Dikategorikan\n", i-1);
	}
	for(int j=2; j<argc; j++) pthread_join(tid[j], NULL);
    return;
}

void other(int argc, char *argv[]){
	DIR *dir;
	struct dirent *tmp;
	int i=0;
	if(strcmp(argv[1], "-d") == 0) {
    	dir = opendir(argv[2]);
		strcpy(folder, argv[2]);
	}
	else if((argv[1][0]=='*') && (strlen(argv[1])==1)) {
		dir = opendir(cwd);
		strcpy(folder, cwd);
	}
	else {
		printf("Butuh Argumen -f atau -d atau \\*\n");
        return;
	}
	while((dir!=NULL) && (tmp=readdir(dir))) {
		if(tmp->d_type==DT_DIR && strcmp(tmp->d_name, ".")!=0 && strcmp(tmp->d_name, "..")!=0){
			strcpy(deepfolder,folder);
			strcat(deepfolder,"/");
			strcat(deepfolder,tmp->d_name);
			strcat(deepfolder,"/");
			dalam();
		}
        if(strcmp(tmp->d_name, ".")==0 || strcmp(tmp->d_name, "..")==0 || strcmp(tmp->d_name, "soal3.c")==0 || strcmp(tmp->d_name, "soal3")==0 || tmp->d_type==DT_DIR) 
		continue;
        err = pthread_create(&tid[i], NULL, move, (void*) tmp->d_name);
        if(err != 0) printf("Yah, gagal disimpan :(\n");
        else printf("Direktori sukses disimpan!\n");
        i++;
    }
    for(int j=0; j<i; j++) pthread_join(tid[j], NULL);
    closedir(dir);
    return;
}

void dalam(){
	DIR *dir;
	struct dirent *tmp;
	int i=0;
	memset(deepfolder2, '\0', sizeof(deepfolder2));
	strcpy(deepfolder2,deepfolder);
	printf("%s\n", deepfolder2);
	dir = opendir(deepfolder2);
	
	while((dir!=NULL) && (tmp=readdir(dir))) {
		if(tmp->d_type==DT_DIR && strcmp(tmp->d_name, ".")!=0 && strcmp(tmp->d_name, "..")!=0){
			strcat(deepfolder2,tmp->d_name);
			strcat(deepfolder2,"/");
			dalam();
		}
        if(strcmp(tmp->d_name, ".")==0 || strcmp(tmp->d_name, "..")==0 || strcmp(tmp->d_name, "soal3.c")==0 || strcmp(tmp->d_name, "soal3")==0 || tmp->d_type==DT_DIR) 
		continue;
        err = pthread_create(&tid[i], NULL, move, (void*) tmp->d_name);
        if(err != 0) printf("Yah, gagal disimpan :(\n");
        else printf("Direktori sukses disimpan!\n");
        i++;
    }
    
    for(int j=0; j<i; j++) pthread_join(tid[j], NULL);
    closedir(dir);
    return;
}
