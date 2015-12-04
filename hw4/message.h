/**
 * This file contains definitions and declarations of functions for messages
 *between
 * client and host
 *
 * Author: Jingxiang Li
 * Date: Thu 03 Dec 2015 07:56:04 PM CST
 */
#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/socket.h>
#include <stdint.h>

// define all message ID's here
#define HANDSHAKE_ID 100L
#define HANDSHAKE_RESPONSE_ID 101L
#define TWITTERTREND_REQUEST_ID 102L
#define RESPONSE_MESSAGE_ID 103L
#define END_OF_REQUEST_ID 104L
#define END_OF_RESPONSE_ID 105L
#define ERROR_MESSAGE_ID 106L

/**
 * send a uint32_t to host
 * @param  sockfd socket file descriptor
 * @param  value  value to be sent
 * @return        0 on success, -1 on error
 */
int send_uint32_t(int sockfd, uint32_t value);

/**
 * send a string to host
 * @param  sockfd socket file descriptor
 * @param  str    string to be sent
 * @param  len    length of the string
 * @return        0 on success, -1 on error
 */
int send_str(int sockfd, char *str, uint32_t len);

/**
 * send a formatted message to host
 * @param  sockfd      socket file descriptor
 * @param  id          message id
 * @param  payload_len length of payload
 * @param  payload     payload content
 * @return             0 on success, otherwise -1
 */
int send_msg(int sockfd, uint32_t id, uint32_t payload_len, char *payload);

/**
 * receive message from host
 * @param  sockfd socket file descriptor
 * @param  id     pointer to a uint32_t, will be modified to be the id of
 *                received message
 * @param  status return status of this function, 0 for success, -1 for failure
 * @return        message payload on success, otherwise NULL
 */
char *receive_msg(int sockfd, uint32_t *id, int *status);

#endif  // MESSAGE_H
