/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Saturday Jul 31, 2021 19:17:44 NPT
 * License     : MIT
 */

#ifndef TRANSPORTER_RECEIVER_H

#define TRANSPORTER_RECEIVER_H

#include <stdbool.h>
#include "deserializer.h"

bool
receive_msg(int socketfd, struct DeserializedObj *obj);

#endif /* TRANSPORTER_RECEIVER_H */

