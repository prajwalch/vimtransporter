/*
 * author      : Prajwal Chapagain (pub.cx00@gmail.com)
 * created at  : Tuesday Jul 20, 2021 12:53:30 NPT
 * license     : MIT
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <readtrmin/readtrmin.h>

int main()
{
    int client_sockfd;
    int len;
    struct sockaddr_in address;//Server-side Network Address Structures
    int result;
    char str1[6] = {0};

    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);//Set up client socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(2058);
    len = sizeof(address);
    result = connect(client_sockfd, (struct sockaddr *)&address, len);
    if(result == -1)
    {
        perror("oops: client2");
        exit(1);
    }

    while (1) {

        bool read_ok = readtrmin_string(str1, sizeof(str1), 4, &default_stringoption);
        if (read_ok) {
            if (strcmp(str1, "exit") == 0)
                break;
            //First reading and writing
            printf("before sending: %s\n", str1);
            write(client_sockfd, str1, 4);
            read(client_sockfd, str1, sizeof str1);
            printf("after received: %s\n", str1);
        }

    }

    close(client_sockfd);

    return 0;
}

