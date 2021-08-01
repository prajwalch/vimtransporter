/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Friday Jul 30, 2021 16:13:59 NPT
 * License     : MIT
 */

#ifndef TRANSPORTER_DESERIALIZER_H

#define TRANSPORTER_DESERIALIZER_H

#include <stdbool.h>

struct DeserializedObj {
    int msg_id;
    char msg_data[4096];
    char svr_cmd[10];
};

bool
deserialize_msg(const char *raw_msg_str, struct DeserializedObj *obj);

#endif /* TRANSPORTER_DESERIALIZER_H */

