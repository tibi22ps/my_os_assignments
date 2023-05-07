#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <semaphore.h>

#include <pthread.h>

sem_t sem3_1;
sem_t sem3_2;
sem_t sem5;
sem_t *semaphore1=NULL;
sem_t *semaphore2=NULL;


//pthread_barrier_t bariera; 

void* threadFnP3 (void* arg){
    int id=*(int*)arg;

    if(id==1){
        sem_wait(&sem3_1);
        info(BEGIN, 3, id);
        info(END, 3, id);
        sem_post(&sem3_2);
    }
    else if(id==2){
        info(BEGIN, 3, id);
        sem_post(&sem3_1);
        sem_wait(&sem3_2);
        info(END, 3, id);
    }
    else if(id ==3) {
        sem_wait(semaphore1);
        info(BEGIN, 3, id);

        info(END, 3, id);
        sem_post(semaphore2);
          }

    else{
        info(BEGIN, 3, id);
        info(END, 3, id);
    }

    pthread_exit(NULL);
}

void* threadFnP5 (void* arg){
    int id=*(int*)arg;
    sem_wait(&sem5);
    info(BEGIN, 5, id);

    info(END, 5, id);
    sem_post(&sem5);
    pthread_exit(NULL);
}

void* threadFnP6 (void* arg){
    int id=*(int*)arg;
    if(id == 4){ 
        info(BEGIN, 6, id);
    
        info(END, 6, id);
        sem_post(semaphore1);
        }
        else if(id == 2)
            { 
                sem_wait(semaphore2);
                info(BEGIN, 6, id);

                info(END, 6, id);
            }
            else{
                info(BEGIN, 6, id);

                info(END, 6, id);
            }

    
    pthread_exit(NULL);
}


int main(){
    init();

    info(BEGIN, 1, 0);

    sem_unlink("semafor1");
    sem_unlink("semafor2");
    semaphore1 = sem_open("/semafor1", O_CREAT, 0644, 0);
    semaphore2 = sem_open("/semafor2", O_CREAT, 0644, 0);

    pid_t pid2, pid3, pid4, pid5, pid6, pid7;

    // Crearea proceselor P2 si P6
    pid2 = fork();
    if (pid2 < 0) {
        fprintf(stderr, "Eroare la crearea procesului P2\n");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) { // P2
        info(BEGIN, 2, 0);

        // Crearea procesului P6
        pid6 = fork();
        if (pid6 < 0) {
            fprintf(stderr, "Eroare la crearea procesului P6\n");
            exit(EXIT_FAILURE);
        } else if (pid6 == 0) { // P6
            info(BEGIN, 6, 0);

            //sem_init(&semaphore, 0, 0);
            //sem_init(&semaphore2, 0, 0);

            pthread_t tid1=0;
            pthread_t tid2=0;
            pthread_t tid3=0;
            pthread_t tid4=0;
            int id[4]={1,2,3,4};
            if(pthread_create(&tid1, NULL, threadFnP6, &id[0]) != 0){
            perror("Error creating thread");
            return 1;
            }
            
            if(pthread_create(&tid2, NULL, threadFnP6, &id[1]) != 0){
            perror("Error creating thread");
            return 1;
            }
            
            if(pthread_create(&tid3, NULL, threadFnP6, &id[2]) != 0){
            perror("Error creating thread");
            return 1;
            }
           
            if(pthread_create(&tid4, NULL, threadFnP6, &id[3]) != 0){
            perror("Error creating thread");
            return 1;
            }

            pthread_join(tid1, NULL);
            pthread_join(tid2, NULL);
            pthread_join(tid3, NULL);
            pthread_join(tid4, NULL);



            info(END, 6, 0);
            exit(EXIT_SUCCESS);
        } else { // P2
            wait(NULL); // Asteapta terminarea procesului P6
        }
        info(END, 2, 0);
        exit(EXIT_SUCCESS);
    } else { // P1
        // Crearea procesului P3
        pid3 = fork();
        if (pid3 < 0) {
            fprintf(stderr, "Eroare la crearea procesului P3\n");
            exit(EXIT_FAILURE);
        } else if (pid3 == 0) { // P3
            info(BEGIN, 3, 0);
            pthread_t tid1=0;
            pthread_t tid2=0;
            pthread_t tid3=0;
            pthread_t tid4=0;
            pthread_t tid5=0;
            int id3[5]={1,2,3,4,5};
            if(pthread_create(&tid1, NULL, threadFnP3, &id3[0]) != 0){
            perror("Error creating thread");
            return 1;
            }

            if(sem_init(&sem3_1, 0, 0) != 0) {
                perror("Error initializing semaphore");
                return 1;
             }

             if(sem_init(&sem3_2, 0, 0) != 0) {
                perror("Error initializing semaphore");
                return 1;
             }

            if(pthread_create(&tid2, NULL, threadFnP3, &id3[1]) != 0){
            perror("Error creating thread");
            return 1;
            }

            if(pthread_create(&tid3, NULL, threadFnP3, &id3[2]) != 0){
            perror("Error creating thread");
            return 1;
            }

            if(pthread_create(&tid4, NULL, threadFnP3, &id3[3]) != 0){
            perror("Error creating thread");
            return 1;
            }

             if(pthread_create(&tid5, NULL, threadFnP3, &id3[4]) != 0){
            perror("Error creating thread");
            return 1;
            }

            pthread_join(tid1, NULL);
            pthread_join(tid2, NULL);
            sem_destroy(&sem3_1);
            sem_destroy(&sem3_2);
            pthread_join(tid3, NULL);
            pthread_join(tid4, NULL);
            pthread_join(tid5, NULL);




            // Crearea procesului P4
            pid4 = fork();
            if (pid4 < 0) {
                fprintf(stderr, "Eroare la crearea procesului P4\n");
                exit(EXIT_FAILURE);
            } else if (pid4 == 0) { // P4
                info(BEGIN, 4, 0);

                info(END, 4, 0);
                exit(EXIT_SUCCESS);
            } else { 
                pid5 = fork();
                if (pid5 < 0) {
                    fprintf(stderr, "Eroare la crearea procesului P5\n");
                    exit(EXIT_FAILURE);
                } else if (pid5 == 0) { // P5
                    info(BEGIN, 5, 0);

                    pthread_t tid[48];
                    int id48[48]={0};
                    sem_init(&sem5, 0 , 4);
                    for(int i=0; i<48; i++){
                        id48[i]=i+1;
                        if(pthread_create(&tid[i], NULL, threadFnP5, &id48[i]) != 0){
                            perror("Error creating thread");
                            return 1;
                        }
                    }
                    for(int i=0; i<48;i++){
                        sem_wait(&sem5);
                        pthread_join(tid[i], NULL);
                        sem_post(&sem5);
                    }
                    sem_destroy(&sem5);
                    info(END, 5, 0);
                    exit(EXIT_SUCCESS);
                } else { // P3
                    wait(NULL); // Asteapta terminarea procesului P4
                    wait(NULL); // Asteapta terminarea procesului P5
                    }
                    //sem_destroy(&semaphore);
                    //sem_destroy(&semaphore2);
            }
        info(END, 3, 0);
        exit(EXIT_SUCCESS);
        } else { 
            pid7 = fork();
            if (pid7 < 0) {
                fprintf(stderr, "Eroare la crearea procesului P7\n");
                exit(EXIT_FAILURE);
            }
            else if (pid7==0){ // P2
            info(BEGIN, 7,0);

            info(END, 7, 0);
            exit(EXIT_SUCCESS);
            }
             else{
            // P1
            wait(NULL); // Asteapta terminarea procesului P2
            wait(NULL); // Asteapta terminarea procesului P3
            wait(NULL); // Asteapta terminarea procesului P7
            }
        }
    }
    sem_close(semaphore1);
    sem_close(semaphore2);
    info(END, 1, 0);
    return 0;
}
