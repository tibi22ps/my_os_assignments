
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h> 
#include <fcntl.h>

typedef struct {
    char sect_name[7];
    short sect_type;
    int sect_offset;
    int sect_size;
} SectionHeader;

typedef struct {
    char magic[2];
    short header_size;
    char version;
    char no_of_sections;
    SectionHeader *section_headers;
} Header;

int listDir(const char *path)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];
    struct stat statbuf;


    dir = opendir(path);
    if(dir == NULL) {
        perror("ERROR\ninvalid directory path");
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
        perror("ERROR\ninvalid directory path");
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

int listDirG(const char *path, off_t size_greater)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];
    struct stat statbuf;



    dir = opendir(path);
    if(dir == NULL) {
        perror("ERROR\ninvalid directory path");
        return -1;
    }
    printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL) {
        //printf("SUCCESS\n");
        snprintf(filePath, 512, "%s/%s", path, entry->d_name);
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            //snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(filePath, &statbuf) == 0) {
                if(S_ISREG(statbuf.st_mode)){
                    if(statbuf.st_size > size_greater){
                printf("%s\n", filePath);
                    }
                }
            }
        } 
    }

    closedir(dir);
    return 0;
}

void listRecG(const char *path, int one, off_t size_greater)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL) {
        perror("ERROR\ninvalid directory path");
        return;
    }
    if(one==0){
        printf("SUCCESS\n");
        one++;
    }
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if((lstat(fullPath, &statbuf) == 0)){
                if(S_ISREG(statbuf.st_mode)){
                    if(statbuf.st_size > size_greater){
                        printf("%s\n", fullPath);
                        
                    }
                }
                else if(S_ISDIR(statbuf.st_mode)) {
                    listRecG(fullPath, one, size_greater);
                    }
                }
            
        }
    }
    closedir(dir);
}

int parse_function(const char* path, Header *header){
    int fd;
    fd= open(path ,O_RDONLY);
    if(fd==-1){
        printf("ERROR\nCould not open input file\n");
        return -1;
    }

    //off_t size = lseek(fd, 0,SEEK_END);

    char buffer[18];
    buffer[17]=0;
    lseek(fd, -16,SEEK_END);
    read(fd, buffer, 16);
    header->magic[1]=buffer[15];
    header->magic[0]=buffer[14];

    if(header->magic[0]!='f' || header->magic[1]!='d'){
        printf("ERROR\nwrong magic\n");
        //printf("%c->%c->\n", header->magic[0], header->magic[1]);
        return -1;
    }

    memcpy(&header->header_size,buffer+12,sizeof(short));
    //printf("\n%d\n",(int)header->header_size);

    lseek(fd,-(header->header_size), SEEK_END);
    read(fd, buffer, 16);
    header->version=buffer[0];
    header->no_of_sections=buffer[1];
    if(header->version < 74 || header->version > 121){
        //printf("%s",buffer);
        printf("ERROR\nwrong version\n");
        return -1;
    }
    //printf("%d",header->version);
    if(header->no_of_sections<5 || header->no_of_sections>11){
        printf("ERROR\nwrong sect_nr\n");
        return -1;
    }

     SectionHeader *sectionheader=(SectionHeader*)malloc(sizeof(SectionHeader)*header->no_of_sections);
    lseek(fd,-header->header_size+2,SEEK_END);
    //printf("..%s..", buffer);
    for(int i=0;i<header->no_of_sections;i++){
        read(fd,buffer,16);
        //printf("%d", read(fd,buffer,16));
        //buffer[16]=0;
        //printf("..%s..", buffer);
        for(int j=0;j<6;j++){
                sectionheader[i].sect_name[j]=buffer[j];

        }
        sectionheader[i].sect_name[6]=0;
        memcpy(&sectionheader[i].sect_type,buffer+6,sizeof(short));
        memcpy(&sectionheader[i].sect_offset,buffer+6+sizeof(short), sizeof(int));
        memcpy(&sectionheader[i].sect_size,buffer+6+sizeof(short)+sizeof(int), sizeof(int));

    
    if(sectionheader[i].sect_type!=52 && sectionheader[i].sect_type!=26 && sectionheader[i].sect_type!=85 && sectionheader[i].sect_type!=17 && sectionheader[i].sect_type!=42 && sectionheader[i].sect_type!=88 && sectionheader[i].sect_type!=31 ){
        printf("ERROR\nwrong sect_types\n");
        //printf("%d->%d", sectionheader[i].sect_type,i);
        free(sectionheader);
        close(fd);
        
        return -1;

        }
    }
    header->section_headers=sectionheader;
    printf("SUCCESS\n");
    printf("version=%d\n", header->version);
    printf("nr_sections=%d\n", header->no_of_sections);
    for(int q=0;q<header->no_of_sections;q++){
        printf("section%d: %s %d %d\n", q+1,header->section_headers[q].sect_name, header->section_headers[q].sect_type, header->section_headers[q].sect_size);
    }

    return 0;
}

void findall(const char* path, int one) {
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;
    Header header;

    dir = opendir(path);
    if (dir == NULL) {
        printf("ERROR\ninvalid directory path");
        return;
    }

    if (one == 0) {
        printf("SUCCESS\n");
        one++;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0) {
                if (S_ISREG(statbuf.st_mode)) {
                    if (parse_function(fullPath, &header) == 0) {
                        int has_large_section = 0;
                        for (int i = 0; i < header.no_of_sections; i++) {
                            if (header.section_headers[i].sect_size > 1407) {
                                has_large_section = 1;
                                
                            }
                        }
                        if (!has_large_section) {
                            printf("%s\n", fullPath);
                        }
                    }
                } else if (S_ISDIR(statbuf.st_mode)) {
                    findall(fullPath, one);
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
        int parse=-1;
        int find_all=-1;
        for(int i=1;i<argc;i++){
        	if(strcmp(argv[i], "list") == 0){
        	list=0;
        	break;
            }
            if(strcmp(argv[i], "parse") == 0){
        	parse=0;
        	break;
        	}
            if(strcmp(argv[i], "findall") == 0){
        	find_all=0;
        	break;
            }
        }
        if(list==0){
        char* path=NULL;
        //char* filtering_options=NULL;
        int recursive=-1;
        off_t size_greater=0;
        for(int i=1;i<argc;i++){
        	if(strncmp(argv[i], "path=", 5)==0){
        	path=argv[i]+5;
            }
        }
            for(int i=1;i<argc;i++){
                if(strncmp(argv[i],"size_greater=", 13)==0){
                        
                        size_greater=(off_t) atoll(argv[i]+13);
                        //printf("%ld\n\n", size_greater);
                }
            }
            for(int i=1;i<argc;i++){
            if(strncmp(argv[i], "recursive", 10)==0){
                    recursive=1;
                }
            }
        if(recursive==-1){
            if(size_greater==0){
            listDir(path);
            }
            else{
                //printf("\n\naltceva");
                listDirG(path,size_greater);
            }
        }
        else{
            if(size_greater==0){
            listRec(path,0);
            }
            else{
                //printf("\n\nceva");
                listRecG(path,0,size_greater);
            }
        }
            }


        if(parse==0){
            Header header;
            //initializam cu 0
            header.header_size=0;
            header.magic[0]=0;
            header.magic[1]=0;
            header.no_of_sections=0;
            header.section_headers=NULL;
            header.version=0;

            char* path=NULL;
            for(int i=1;i<argc;i++){
        	    if(strncmp(argv[i], "path=", 5)==0){
        	    path=argv[i]+5;
                }
            }
            parse_function(path, &header);
        }

        if(find_all==0){
            char* path=NULL;

            for(int i=1;i<argc;i++){
        	    if(strncmp(argv[i], "path=", 5)==0){
        	    path=argv[i]+5;
                }
            }
            findall(path,0);
        }

    }
    }
    return 0;
}

