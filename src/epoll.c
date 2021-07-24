/*
 * author      : Prajwal Chapagain (pub.cx00@gmail.com)
 * created at  : Wednesday Jul 21, 2021 17:07:27 NPT
 * license     : MIT
 */

#include "epoll.h"

#include <errno.h>
#include <stdio.h>

extern void die_with_error(const char *msg);

int
epoll_create_instance()
{
    int epollfd = epoll_create1(0);
    if (epollfd == -1)
        die_with_error("fail to initilize the epoll");
    return epollfd;
}


static void
SET_EVENT_DATA_FD(struct epoll_event *event, int socketfd)
{
    event->data.fd = socketfd;
}

void
epoll_ctl_add_fd(int epollfd, int socketfd, struct epoll_event *event)
{
    SET_EVENT_DATA_FD(event, socketfd);
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, event) == -1) {
        if (errno == EBADF) {
            fprintf(stderr, "file descriptor '%i' or '%i' is not a valid descriptor", epollfd,  socketfd);
            return;
        }

        if (errno == EEXIST) {
            fprintf(stderr, "file descriptor '%i' is already registered", socketfd);
            return;
        }

        die_with_error("epoll_ctl: fail to add/register the socket fd");
    }
}

void
epoll_ctl_update_fd(int epollfd, int socketfd, struct epoll_event *event)
{
    SET_EVENT_DATA_FD(event, socketfd);
    if (epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, event) == -1) {
        if (errno == EBADF) {
            fprintf(stderr, "file descriptor '%i' or '%i' is not a valid descriptor", epollfd,  socketfd);
            return;
        }

        if (errno == ENOENT) {
            fprintf(stderr, "file descriptor '%i' is not registered with epoll", socketfd);
            return;
        }
        die_with_error("epoll_ctl: fail to update/modify the socket fd");
    }
}

void
epoll_ctl_delete_fd(int epollfd, int socketfd, struct epoll_event *event)
{
    SET_EVENT_DATA_FD(event, socketfd);
    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, socketfd, event) == -1) {
        if (errno == EBADF) {
            fprintf(stderr, "file descriptor '%i' or '%i' is not a valid descriptor", epollfd,  socketfd);
            return;
        }

        if (errno == ENOENT) {
            fprintf(stderr, "file descriptor '%i' is not registered with epoll", socketfd);
            return;
        }
        die_with_error("epoll_ctl: fail to remove/deregister the socket fd");
    }
}
