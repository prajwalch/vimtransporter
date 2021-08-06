/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Friday Jul 30, 2021 16:13:52 NPT
 * License     : MIT
 */

#include "deserializer.h"

#include <stdio.h>
#include <string.h>

static bool
is_cmd_data(const char *msg_data)
{
    if (msg_data[0] != '$')
        return false;
    return true;
}

bool
deserialize_msg(const char *raw_msg_str, struct DeserializedObj *obj)
{
    if (sscanf(raw_msg_str, "[%d,\"%[^\"]\"]", &(obj->msg_id), obj->msg_data) != 2) {
        fprintf(stderr, "message deserialization fail\n");
        return false;
    }

    if (is_cmd_data(obj->msg_data)) {
        if (sscanf(obj->msg_data, "${%[^:]:%[^}]", obj->svr_cmd, obj->msg_data) != 2) {
            fprintf(stderr, "command data deserialization fail\n");
            return false;
        }
    }

    return true;
}

