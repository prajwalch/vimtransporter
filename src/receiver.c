/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Saturday Jul 31, 2021 19:17:40 NPT
 * License     : MIT
 */

#include "receiver.h"
#include "responder.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

bool
receive_msg(int socketfd, struct DeserializedObj *obj)
{
    char raw_msg_str[4096];
    memset(raw_msg_str, 0, sizeof(raw_msg_str));

    bool has_read_done = false;
    int total_byte_read = 0;

    while (1) {
        total_byte_read = recv(socketfd, raw_msg_str, 4096, 0);
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
        return false;

    if (!deserialize_msg(raw_msg_str, obj)) {
        response_error_string(socketfd);
        return false;
    }

    return true;
}
