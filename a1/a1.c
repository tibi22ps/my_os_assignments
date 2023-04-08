
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h> 

int listDir(const char *path)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];
    struct stat statbuf;


    dir = opendir(path);
    if(dir == NULL) {
        perror("Could not open directory");
        return -1;
    }
    printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL) {
        //printf("SUCCESS\n");
        snprintf(filePath, 512, "%s/%s", path, entry->d_name);
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            //snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(filePath, &statbuf) == 0) {
                printf("%s\n", filePath);
        } 
        else {
            printf("ERROR\n ");
        }
    }
    }
    closedir(dir);
    return 0;
}

void listRec(const char *path, int one)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL) {
        perror("ERROR\n");
        return;
    }
    if(one==0){
        printf("SUCCESS\n");
        one++;
    }
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                printf("%s\n", fullPath);
                if(S_ISDIR(statbuf.st_mode)) {
                    listRec(fullPath, one);
                }
            }
        }
    }
    closedir(dir);
}

int main(int argc, char **argv){
    if(argc >= 2){
        if(strcmp(argv[1], "variant") == 0){
            printf("90436\n");
        }
        else{
        int list=-1;
        for(int i=1;i<argc;i++){
        	if(strcmp(argv[i], "list") == 0){
        	list=0;
        	break;
        	}
        }
        if(list==0){
        char* path=NULL;
        //char* filtering_options=NULL;
        int recursive=-1;
        for(int i=1;i<argc;i++){
        	if(strncmp(argv[i], "path=", 5)==0){
        	path=argv[i]+5;
        	}
            else {
                if(strncmp(argv[i], "recursive", 10)==0){
                    recursive=1;
                }
            }
        }
        if(recursive==-1){
            listDir(path);
        }
        else{
            listRec(path,0);
        }
            }

        }
    }
    return 0;
}

