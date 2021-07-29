/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Tuesday Jul 27, 2021 19:43:43 NPT
 * License     : MIT
 */

#include "msg_util.h"

#include <stdio.h>
#include <string.h>

bool
stringify_msg(char *buffer,
              int buffer_size,
              int msg_id,
              const char *msg_data)
{
    snprintf(buffer, buffer_size, "[%d,\"%s\"]", msg_id, msg_data);
    return true;
}

bool
parse_msg(const char *msg_string, struct ParsedMsg *parsed_msg)
{
    parsed_msg->msg_id = 0;
    memset(&parsed_msg->msg_data, 0, sizeof(parsed_msg->msg_data));

    if (sscanf(msg_string, "[%d,\"%[^\"]]", &(parsed_msg->msg_id), parsed_msg->msg_data) != 2) {
        fprintf(stderr, "fail to decode the message\n");
        return false;
    }
    return true;
}

