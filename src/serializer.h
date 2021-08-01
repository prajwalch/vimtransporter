/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Thursday Jul 29, 2021 20:38:26 NPT
 * License     : MIT
 */

#ifndef TRANSPORTER_SERIALIZER_H

#define TRANSPORTER_SERIALIZER_H

#include <stdbool.h>

enum ChannelCommand {
    CHCMD_REDRAW,
    CHCMD_EX,
    CHCMD_NORMAL,
    CHCMD_EXPR,
    CHCMD_CALL,
    CHCMD_NORMAL_STRING
};

bool
serialize_msg(char *buf,
              enum ChannelCommand cmd,
              const char *fmt, ...);

#endif /* TRANSPORTER_SERIALIZER_H */

