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
response_normal_string(int socketfd,
                       char *buf,
                       int msg_id,
                       const char *msg_data)
{
    //const char *fmt = "[%d,\"%s\"]";
    serialize_msg(buf,
                  CHCMD_NORMAL_STRING,
                  "[%d,\"%s\"]",
                  msg_id, msg_data);
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
response_redraw_cmd(int socketfd,
                    char *buf,
                    const char *forced)
{
    serialize_msg(buf,
                  CHCMD_REDRAW,
                  "[\"redraw\",\"%s\"]",
                  forced);
    send(socketfd, buf, strlen(buf), 0);
}

void
response_ex_cmd(int socketfd,
                char *buf,
                const char *cmd)
{
    serialize_msg(buf,
                  CHCMD_EX,
                  "[\"ex\",\"%s\"]",
                  cmd);
    send(socketfd, buf, strlen(buf), 0);
}

void
response_normal_cmd(int socketfd,
                    char *buf,
                    const char *norm_mode_cmd)
{
    serialize_msg(buf,
                  CHCMD_NORMAL,
                  "[\"normal\",\"%s\"]",
                  norm_mode_cmd);
    send(socketfd, buf, strlen(buf), 0);
}

void
response_expr_cmd(int socketfd,
                  char *buf,
                  const char *expr)
{
    serialize_msg(buf,
                  CHCMD_EXPR,
                  "[\"expr\",\"%s\"]",
                  expr);
    send(socketfd, buf, strlen(buf), 0);
}

void
response_call_cmd(int socketfd,
                  char *buf,
                  const char *func_name,
                  const char *arg_list)
{
    serialize_msg(buf,
                  CHCMD_CALL,
                  "[\"call\",\"%s\",\"%s\"]",
                  func_name, arg_list);
    send(socketfd, buf, strlen(buf), 0);
}

