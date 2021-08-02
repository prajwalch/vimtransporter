/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Saturday Jul 31, 2021 14:16:26 NPT
 * License     : MIT
 */

#include "responder.h"
#include "serializer.h"

#include <string.h>
#include <sys/socket.h>

void
send_error_WO_SER(int socketfd)
{
    const char msg[] = "[0,\"invalid message\"]";
    send(socketfd, msg, strlen(msg), 0);
}

void
response_normal_string(int socketfd, char *buf, int msg_id, const char *msg_data)
{
    //const char *fmt = "[%d,\"%s\"]";
    if (!serialize_msg(buf, CHCMD_NORMAL_STRING, "[%d,\"%s\"]", msg_id, msg_data)) {
        send_error_WO_SER(socketfd);
        return;
    }
    send(socketfd, buf, strlen(buf), 0);
}

void
response_error_string(int socketfd)
{
    char res_buf[50];
    memset(res_buf, 0, sizeof(res_buf));
    response_normal_string(socketfd, res_buf, 0, "invalid msg");
}

void
response_redraw_cmd(int socketfd, char *buf, const char *forced)
{
    if (!serialize_msg(buf, CHCMD_REDRAW, "[\"redraw\",\"%s\"]", forced)) {
        send_error_WO_SER(socketfd);
        return;
    }
    send(socketfd, buf, strlen(buf), 0);
}

void
response_ex_cmd(int socketfd, char *buf, const char *cmd)
{
    if (!serialize_msg(buf, CHCMD_EX, "[\"ex\",\"%s\"]", cmd)) {
        send_error_WO_SER(socketfd);
        return;
    }
    send(socketfd, buf, strlen(buf), 0);
}

void
response_normal_cmd(int socketfd, char *buf, const char *norm_mode_cmd)
{
    if (!serialize_msg(buf, CHCMD_NORMAL, "[\"normal\",\"%s\"]", norm_mode_cmd)) {
        send_error_WO_SER(socketfd);
        return;
    }
    send(socketfd, buf, strlen(buf), 0);
}

void
response_expr_cmd(int socketfd,.char *buf, const char *expr)
{
    if (!serialize_msg(buf, CHCMD_EXPR, "[\"expr\",\"%s\"]", expr)) {
        send_error_WO_SER(socketfd);
        return;
    }
    send(socketfd, buf, strlen(buf), 0);
}

void
response_call_cmd(int socketfd, char *buf, const char *func_name, const char *arg_list)
{
    if (!serialize_msg(buf, CHCMD_CALL, "[\"call\",\"%s\",\"%s\"]", func_name, arg_list)) {
        send_error_WO_SER(socketfd);
        return;
    }
    send(socketfd, buf, strlen(buf), 0);
}

