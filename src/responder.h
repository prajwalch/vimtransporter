/*
 * Author      : Prajwal Chapagain <prajjwal2058@gmail.com>
 * Date        : Saturday Jul 31, 2021 15:42:05 NPT
 * License     : MIT
 */

#ifndef RESPONDER_H

#define RESPONDER_H


void
response_normal_string(int socketfd,
                       char *buf,
                       int msg_id,
                       const char *msg_data);

void
response_redraw_cmd(int socketfd,
                    char *buf,
                    const char *forced);

void
response_ex_cmd(int socketfd,
                char *buf,
                const char *cmd);

void
response_normal_cmd(int socketfd,
                    char *buf,
                    const char *norm_mode_cmd);

void
response_expr_cmd(int socketfd,
                  char *buf,
                  const char *expr);

void
response_call_cmd(int socketfd,
                  char *buf,
                  const char *func_name,
                  const char *arg_list);

void
response_error_string(int socketfd);

#endif /* RESPONDER_H */

