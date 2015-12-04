/**
 * this file contains functions for sending and receiving messages between
 * client and host
 *
 * Author: Jingxiang Li
 * Date: Thu 03 Dec 2015 07:57:00 PM CST
 */

#include "./message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>

int send_uint32_t(int sockfd, uint32_t value) {
    size_t n = 0, n_total = 0;
    while (n_total != sizeof(uint32_t)) {
        n = write(sockfd, (char *)&value + n_total, sizeof(uint32_t) - n_total);
        n_total += n;
    }
    return 0;
}

int send_str(int sockfd, char *str, uint32_t len) {
    // if a NULL pointer is passed, then treat is as an empty string
    if (NULL == str) return 0;

    size_t n = 0, n_total = 0;
    while (n_total != len) {
        n = write(sockfd, str + n_total, len - n_total);
        n_total += n;
    }
    return 0;
}

int send_msg(int sockfd, uint32_t id, uint32_t payload_len, char *payload) {
    if (-1 == send_uint32_t(sockfd, id)) return -1;
    if (-1 == send_uint32_t(sockfd, payload_len)) return -1;
    if (-1 == send_str(sockfd, payload, payload_len)) return -1;
    return 0;
}

char *receive_msg(int sockfd, uint32_t *id, int *status) {
    size_t n, n_total;
    uint32_t ret_id, payload_len;
    char *payload = NULL;
    *status = -1;

    // read id
    n = 0;
    n_total = 0;
    while (n_total != sizeof(uint32_t)) {
        n = read(sockfd, (char *)&ret_id + n_total, sizeof(uint32_t) - n_total);
        n_total += n;
    }
    //====================================================
    // modify id here
    *id = ret_id;
    //=====================================================

    // read payload_len
    n = 0;
    n_total = 0;
    while (n_total != sizeof(uint32_t)) {
        n = read(sockfd, (char *)&payload_len + n_total,
                 sizeof(uint32_t) - n_total);
        n_total += n;
    }

    //===============================================================
    // if the length of payload is 0, return immediately
    if (0 == payload_len) {
        *status = 0;
        return NULL;
    }
    //===============================================================

    // read string
    payload = (char *)malloc(payload_len * sizeof(char));
    if (NULL == payload) {
        perror("ERROR, failed to allocate space for payload");
        return NULL;
    }

    n = 0;
    n_total = 0;
    while (n_total != payload_len) {
        n = read(sockfd, payload + n_total, payload_len - n_total);
        n_total += n;
    }

    *status = 0;
    return payload;
}
