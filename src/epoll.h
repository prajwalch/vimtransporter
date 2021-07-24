/*
 * author      : Prajwal Chapagain (pub.cx00@gmail.com)
 * created at  : Wednesday Jul 21, 2021 17:50:18 NPT
 * license     : MIT
 */

#ifndef TRANSPORTER_EPOLL_H

#define TRANSPORTER_EPOLL_H

#include <sys/epoll.h>

int
epoll_create_instance(void);

void
epoll_ctl_add_fd(int epollfd, int socketfd, struct epoll_event *event);

void
epoll_ctl_update_fd(int epollfd, int socketfd, struct epoll_event *event);

void
epoll_ctl_delete_fd(int epollfd, int socketfd, struct epoll_event *event);

#endif /* TRANSPORTER_EPOLL_H */

