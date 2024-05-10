#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#define FIFO_NAME "RESP_PIPE_90436"
#define FIFO_2 "REQ_PIPE_90436"
#define MAX_BUFFER_SIZE 256
#define MAX 100

void handle_request(const char* request, char* response) {
    
}


int main(void){
	int fd1 = -1;
	int fd2= -1;
	char x[MAX]= "CCONNECT" ;
	char y[MAX];
	char request[MAX_BUFFER_SIZE];
    char response[MAX_BUFFER_SIZE];
	
	
	if(mkfifo(FIFO_NAME, 0600) !=0 ){
	perror("ERROR\ncannot create the response pipe | cannot open the request pipe");
	return 1;
	}
	
	fd2 = open(FIFO_2, O_RDONLY);
	if(fd2 == -1) {
		perror("ERROR\ncannot create the response pipe | cannot open the request pipe");
		return 1;
		}
	 
	fd1=open(FIFO_NAME, O_WRONLY);
	if(fd1 == -1){
		perror("ERROR\ncannot create the response pipe | cannot open the request pipe");
		return 1;
		}
	write(fd1, &x, sizeof(x));
	
	while (1) {
        // Citește mesajul de request din pipe
        read(fd2, &y, sizeof(y));
	//printf("The read value is: %d\n", x);
 
            handle_request(request, response);

            // Scrie rezultatul tratării în pipe-ul de response
            write(fd2, &response, sizeof(response));
            }
    const char* mesaj="START!";
	if(write(fd1, mesaj,strlen(mesaj)) == -1){
		perror("ERROR\ncannot write to the response pipe");
		exit(EXIT_FAILURE);
	}
	
	close(fd1);
	close(fd2);
	
	unlink(FIFO_NAME);
	
	
	
	return 0;

}
