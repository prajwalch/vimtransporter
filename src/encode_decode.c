/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Tuesday Jul 27, 2021 19:43:43 NPT
 * License     : MIT
 */

#include "encode_decode.h"

#include <stdio.h>
#include <string.h>

bool
encode_msg(char *buffer,
           int buffer_size,
           int msg_id,
           const char *msg_data)
{
    snprintf(buffer, buffer_size, "[%d,\"%s\"]", msg_id, msg_data);
    return true;
}

bool
decode_msg(const char *encoded_msg, struct DecodedMsg *decoded_msg)
{
    decoded_msg->msg_id = 0;
    memset(&decoded_msg->msg_data, 0, sizeof(decoded_msg->msg_data));

    if (sscanf(encoded_msg, "[%d,\"%[^\"]]", &(decoded_msg->msg_id), decoded_msg->msg_data) != 2) {
        fprintf(stderr, "fail to decode the message\n");
        return false;
    }
    return true;
}

