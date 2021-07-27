#include "epoll.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // ERROR_FAILURE
#include <string.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define DBLOG(m) \
    printf("%s\n", m);

#define DEFAULT_PORT 8765
#define DEFAULT_BACKLOG 10
#define MAX_EVENTS 20
#define MAX_BUFFER_SIZE 20

void die_with_error(const char *msg);
bool socket_make_nonblocking(int socketfd);

struct FdCollection {
    int master_socketfd;
    int epollfd;
    int client_socketfd;
};

struct DecodedMsg {
    int msg_id;
    char msg_data[MAX_BUFFER_SIZE];
};

void
die_with_error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

bool
is_ping_msg(char *buffer)
{
    if (strlen(buffer) == 4 && strncmp(buffer, "PING", 4) == 0)
        return true;
    return false;
}

void
encode_msg(char *reply_buffer,
              size_t buffer_size,
              int msg_id, char *msg)
{
    snprintf(reply_buffer, buffer_size, "[%d,\"%s\"]", msg_id, msg);
}

struct DecodedMsg
decode_msg(char *encoded_msg_buff)
{
    struct DecodedMsg decoded_msg;
    decoded_msg.msg_id = 0;
    memset(&decoded_msg.msg_data, 0, sizeof decoded_msg.msg_data);

    sscanf(encoded_msg_buff, "[%d,\"%[^\"]]", &decoded_msg.msg_id, decoded_msg.msg_data);
    return decoded_msg;
}

void
reply_client(int socketfd)
{
    char encoded_msg_buff[MAX_BUFFER_SIZE] = {0};
    ssize_t total_byte_read = 0;
    bool has_read_done = false;

    while (1) {
        total_byte_read = recv(socketfd, encoded_msg_buff, MAX_BUFFER_SIZE, 0);
        if (total_byte_read == 0) {
            fprintf(stderr, "client already closed the connection");
            has_read_done = false;
            break;
        }

        if (total_byte_read == -1) {
            if (errno & (EAGAIN | EWOULDBLOCK)) {
                fprintf(stderr, "there is no available data to read");
                has_read_done = false;
                break;
            }

            if (errno == ECONNREFUSED) {
                fprintf(stderr, "a client '%i' rufused to allow the network connection", socketfd);
                has_read_done = false;
                break;
            }
        }

        has_read_done = true;
        break;
    }

    if (!has_read_done)
        return;

    printf("Received data : %s\n", encoded_msg_buff);

    struct DecodedMsg decoded_msg = decode_msg(encoded_msg_buff);
    printf("Decoded buffer\nnumber: %d, data: %s\n", decoded_msg.msg_id, decoded_msg.msg_data);

    // send PONG as a response, if we got PING msg
    bool has_ping_msg = is_ping_msg(decoded_msg.msg_data);
    if (has_ping_msg) {
        char vim_pong[13] = {0};
        encode_msg(vim_pong, 13, decoded_msg.msg_id, "PONG");
        send(socketfd, vim_pong, strlen(vim_pong), 0);
        return;
    }

    return;
}

// set respective client socket fd who is recently active/ready for un-active/hang up
static inline void
FdColl_mod_client_socketfd(struct FdCollection *fds_coll, int new_socketfd)
{
    fds_coll->client_socketfd = new_socketfd;
}

void
start_event_loop(struct FdCollection *fds_coll)
{
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_in));

    fds_coll->epollfd = epoll_create_instance();

    struct epoll_event pollevent, pollevents[MAX_EVENTS];
    // add master socket fd for monitoring
    pollevent.events = EPOLLIN|EPOLLET;
    epoll_ctl_add_fd(fds_coll->epollfd, fds_coll->master_socketfd, &pollevent);

    while(1) {
        DBLOG("Event: loop started...");
        int num_ready_sockfds = epoll_wait(fds_coll->epollfd, pollevents, MAX_EVENTS, -1);

        if (num_ready_sockfds == -1)
            die_with_error("epoll_wait");

        for (int n_fd = 0; n_fd < num_ready_sockfds; n_fd++) {

            // some error occured or maybe client just hangup/close the connection
            if ((pollevents[n_fd].events & (EPOLLERR | EPOLLRDHUP | EPOLLHUP)) ||
                (!(pollevents[n_fd].events & EPOLLIN))) {

                FdColl_mod_client_socketfd(fds_coll, pollevents[n_fd].data.fd);
                // remove the client from monitor list
                epoll_ctl_delete_fd(fds_coll->epollfd, fds_coll->client_socketfd, &pollevent);
                printf("Event: client '%i' hangup or some error occured\n", fds_coll->client_socketfd);
                continue;

            } else if (pollevents[n_fd].data.fd == fds_coll->master_socketfd) { // new client connection, just accept it
                DBLOG("Event: new client connection");
                socklen_t clientaddr_len = sizeof(struct sockaddr_in);
                fds_coll->client_socketfd = accept(fds_coll->master_socketfd,
                                                   (struct sockaddr *)&client_addr,
                                                   &clientaddr_len);

                if (fds_coll->client_socketfd == -1)
                    die_with_error("fail to accept a new socket connection");

                socket_make_nonblocking(fds_coll->client_socketfd);

                /*
                 * add READ and WRITE (edge triggered) events to newly accepted client socket
                 * to monitor and handle the events later for I/O
                 *
                 * Quick note on EDGE TRIGGERED event
                 *
                 * an event will be fired only, when data will change from one state to another
                 * for ex: 0 to 1, 1 to 2 and so on
                 *
                 * */
                pollevent.events = EPOLLIN|EPOLLRDHUP|EPOLLET;

                epoll_ctl_add_fd(fds_coll->epollfd,
                                 fds_coll->client_socketfd,
                                 &pollevent);
            } else if (pollevents[n_fd].events & EPOLLIN) { // got a new read event, just read the data and response back to client
                FdColl_mod_client_socketfd(fds_coll, pollevents[n_fd].data.fd);
                printf("Event: client '%i' send some data\n", fds_coll->client_socketfd);
                reply_client(fds_coll->client_socketfd);
            }
        }
    }
}

bool
socket_make_nonblocking(int socketfd)
{
    int flags = fcntl(socketfd, F_GETFL, 0);
    if (flags == -1)
        return false;

    int status = fcntl(socketfd, F_SETFL, flags | O_NONBLOCK);
    if (status == -1)
        return false;

    return true;
}

bool
socket_bind(int socketfd)
{
    // internet address setup
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET; // it should be same as when socket was created
    server_addr.sin_port = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // let system decide the ip address
    memset(&server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    if (bind(socketfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == -1)
        return false;
    return true;
}

int
socket_create_endpoint(void)
{
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketfd == -1)
        die_with_error("fail to create new socket");
    return socketfd;
}

void
start_head_server(void)
{
    // different file descriptors collection
    // for easy access on later
    struct FdCollection fds_coll;

    // create new socket
    fds_coll.master_socketfd = socket_create_endpoint();

    // make address reusable
    int yes = 1;
    if (setsockopt(fds_coll.master_socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        die_with_error("fail to set socket option");

    if (!socket_bind(fds_coll.master_socketfd))
        die_with_error("fail to bind the socket");

    if (!socket_make_nonblocking(fds_coll.master_socketfd))
        die_with_error("fail to set socket as non-blocking");

    if (listen(fds_coll.master_socketfd, DEFAULT_BACKLOG) == -1)
        die_with_error("fail to listen the socket connection");

    start_event_loop(&fds_coll);
    close(fds_coll.master_socketfd);
}

int
main(void)
{
    start_head_server();
}
