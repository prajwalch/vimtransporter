/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Tuesday Jul 27, 2021 19:44:03 NPT
 * License     : MIT
 */

#ifndef TRANSPORTER_ENCODE_DECODE_H

#define TRANSPORTER_ENCODE_DECODE_H

#include <stdbool.h>
#include <stddef.h>

#define MSG_DATA_BUFFER 30

struct DecodedMsg {
    int msg_id;
    char msg_data[MSG_DATA_BUFFER];
};

bool
encode_msg(char *reply_buffer,
           int buffer_size,
           int msg_id, char *msg);

bool
decode_msg(char *encoded_msg_buff, struct DecodedMsg *decoded_msg);

#endif /* TRANSPORTER_ENCODE_DECODE_H */

