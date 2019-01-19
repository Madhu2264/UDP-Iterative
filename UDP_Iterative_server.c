#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define MAX_SEND_BUF 1600

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serv_addr, client_address;
    int address_len = sizeof(serv_addr);
    int cli_len = sizeof(client_address);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        printf("Error calling Socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8000);

    if(bind(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("Error binding");
        exit(1);
    }

   while(1)
    {
        char msg[1000];
        long data_len;
        
        data_len = recvfrom(sockfd,(char*)msg,sizeof(msg),0,(struct sockaddr *)&client_address, (socklen_t*)&cli_len);    
        if(data_len)
        {
            printf("\n\nClient connected to Iterative connectionless server\n");
            printf("File name recieved: %s\n", msg);
            
        }
        int file;                                                                  
        if((file = open(msg,O_RDWR)) == -1)
        {
            char* invalid = "INVALID";
            printf("File not found\n");
            printf("Client disconnected\n");
            printf("%s\n",strerror(errno));
            sendto(sockfd, invalid, MAX_SEND_BUF, 0, (struct sockaddr*)&client_address, sizeof(client_address));
        }
        else
        {
            printf("File opened successfully\n");
            ssize_t read_bytes;
            ssize_t sent_bytes;
            
            char send_buf[MAX_SEND_BUF];
            while( (read_bytes = read(file, send_buf, MAX_SEND_BUF)) > 0 )          
            {
            }
            sent_bytes = sendto(sockfd,send_buf, 1600 , 0,(struct sockaddr *)&client_address, sizeof(client_address));
            close(file);
            printf("\n\nClient disconnected\n\n\n");
        }
    }
    close(sockfd);
    return 0;
}
