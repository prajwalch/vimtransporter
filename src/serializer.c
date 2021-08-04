/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Thursday Jul 29, 2021 20:38:19 NPT
 * License     : MIT
 */

#include "serializer.h"

#include <math.h> // floor, log10
#include <stdio.h>
#include <stdlib.h> // abs
#include <stdarg.h> // va_list, va_start, va_arg
#include <string.h>

#define NULL_BYTE 1
#define STR_MODIFIER_LEN 2 // %s = 2 characters
#define INT_MODIFIER_LEN 2 // %s = 2 characters

static int
msg_id_len(int msg_id)
{
    if (msg_id == 0)
        return 1;
    return (floor(log10(abs(msg_id))) + 1);
}

bool
serialize_msg(char *buf, enum ChannelCommand cmd, const char *fmt,...)
{
    va_list ap;
    va_start(ap, fmt);

    const char *msg_data = NULL;
    int msg_size = 0;
    int num_chars_printed = 0;

    switch (cmd) {
        case (CHCMD_REDRAW):
        case (CHCMD_EX):
        case (CHCMD_NORMAL):
        case (CHCMD_EXPR):
            msg_data = va_arg(ap, const char *);
            msg_size = (strlen(fmt) - STR_MODIFIER_LEN) + strlen(msg_data);
            num_chars_printed = snprintf(buf, msg_size + NULL_BYTE, fmt, msg_data);
            break;
        case (CHCMD_CALL): {
            msg_data = va_arg(ap, const char *); // func name
            const char *func_args = va_arg(ap, const char *);
            msg_size = (strlen(fmt) - (STR_MODIFIER_LEN + STR_MODIFIER_LEN)) + strlen(msg_data) + strlen(func_args);
            num_chars_printed = snprintf(buf, msg_size + NULL_BYTE, fmt, msg_data, func_args);
            break;
        }
        case (CHCMD_NORMAL_STRING):
        default: {
            int msg_id = va_arg(ap, int);
            msg_data = va_arg(ap, const char *);
            msg_size = (strlen(fmt) - (INT_MODIFIER_LEN + STR_MODIFIER_LEN)) + msg_id_len(msg_id) + strlen(msg_data);
            num_chars_printed = snprintf(buf, msg_size + NULL_BYTE, fmt, msg_id, msg_data);
            break;
        }
    }

    va_end(ap);
    if (msg_size != num_chars_printed) {
        fprintf(stderr, "message serialization fail\n");
        return false;
    }
    return true;
}

