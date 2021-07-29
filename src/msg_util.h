/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Tuesday Jul 27, 2021 19:44:03 NPT
 * License     : MIT
 */

#ifndef TRANSPORTER_MSG_UTIL_H

#define TRANSPORTER_MSG_UTIL_H

#include <stdbool.h>
#include <stddef.h>

#define MSG_DATA_BUFFER 30

struct ParsedMsg {
    int msg_id;
    char msg_data[MSG_DATA_BUFFER];
};

bool
stringify_msg(char *buffer,
              int buffer_size,
              int msg_id,
              const char *msg_data);

bool
parse_msg(const char *msg_string, struct ParsedMsg *parsed_msg);

#endif /* TRANSPORTER_MSG_UTIL_H */

