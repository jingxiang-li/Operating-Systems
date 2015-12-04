#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/socket.h>
#include <stdint.h>

#define HANDSHAKE_ID 100
#define HANDSHAKE_RESPONSE_ID 101
#define TWITTERTREND_REQUEST_ID 102
#define RESPONSE_MESSAGE_ID 103
#define END_OF_REQUEST_ID 104
#define END_OF_RESPONSE_ID 105
#define ERROR_MESSAGE_ID 106

int send_uint32_t(int sockfd, uint32_t value);

int send_str(int sockfd, char *str, uint32_t len);

int send_msg(int sockfd, uint32_t id, uint32_t payload_len, char *payload);

char *receive_msg(int sockfd, uint32_t id, int *status);

#endif  // MESSAGE_H
