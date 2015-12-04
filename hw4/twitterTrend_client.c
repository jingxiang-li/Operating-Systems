/**
 * This file contains the main function for the client side of twitterTrend
 *
 * Author: Jingxiang Li
 * Date: Thu 03 Dec 2015 05:02:13 PM CST
 */

#include "./client_db.h"
#include "./message.h"
#include "./utility.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 256

int main(int argc, char **argv) {
    if (4 != argc) {
        fprintf(stderr,
                "Usage: ./twitterTrend_client server_host_name "
                "server_port_number file_path\n");
        exit(EXIT_FAILURE);
    }

    // parse arguments
    char *server_host_name = argv[1];
    int server_port_number = atoi(argv[2]);
    char *input_file_path = argv[3];

    // create the client database
    Client_DB *client_db = create_client_db(input_file_path);
    if (NULL == client_db) exit(EXIT_FAILURE);

    // change working directory to where the input file is
    if (-1 == change_wd(input_file_path)) exit(EXIT_FAILURE);

    // create and initialize socket ====================================
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server_entry;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd) {
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    server_entry = gethostbyname(server_host_name);
    if (NULL == server_entry) {
        fprintf(stderr, "ERROR, no such host: %s\n", server_host_name);
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server_entry->h_addr,
           server_entry->h_length);
    server_addr.sin_port = htons(server_port_number);

    if (-1 == connect(sockfd, (struct sockaddr *)&server_addr,
                      sizeof(struct sockaddr_in))) {
        perror("ERROR, failed to connect to the host");
        exit(EXIT_FAILURE);
    }

    // interact host =========================================
    // used for recording number of bytes write to the socket
    uint32_t msg_id, payload_len;
    char *payload = NULL;
    int status;

    // send shake message
    msg_id = HANDSHAKE_ID;
    payload_len = 0;

    if (-1 == send_msg(sockfd, msg_id, payload_len, NULL)) {
        fprintf(stderr, "ERROR, failed to send hand shake message to host\n");
        exit(EXIT_FAILURE);
    }

    // receive hand shake response
    msg_id = HANDSHAKE_RESPONSE_ID;
    receive_msg(sockfd, msg_id, &status);
    if (-1 == status) {
        fprintf(stderr, "ERROR, failed to read hand shake response from host\n");
        exit(EXIT_FAILURE);
    }

    // send city names from the client_db and receive response from host
    msg_id = TWITTERTREND_REQUEST_ID;
    for (int i = 0; i != client_db->size; i++) {
        // send request to host
        Client *client = get_client(client_db, i);
        payload_len = strlen(client->city_name);
        payload = client->city_name;
        if (-1 == send_msg(sockfd, msg_id, payload_len, payload)) {
            fprintf(stderr, "ERROR, failed to send keywords request to host\n");
            exit(EXIT_FAILURE);
        }

        // receive keywords from host
        msg_id = RESPONSE_MESSAGE_ID;
        payload = receive_msg(sockfd, msg_id, &status);
        if (NULL == payload || -1 == status) {
            fprintf(stderr, "ERROR, failed to receive keywords from host for city %s\n", client->city_name);
            exit(EXIT_FAILURE);
        }
        printf("%s : %s\n", client->city_name, payload);

        // receive end of response message from host
        msg_id = END_OF_RESPONSE_ID;
        receive_msg(sockfd, msg_id, &status);
        if (-1 == status) {
            fprintf(stderr, "ERROR, failed to receive end of response message from host\n");
            exit(EXIT_FAILURE);
        }
    }

    // send end of request message to host
    msg_id = END_OF_REQUEST_ID;
    payload_len = 0;
    if (-1 == send_msg(sockfd, msg_id, payload_len, NULL)) {
        fprintf(stderr, "ERROR, failed to send end of request message to host\n");
        exit(EXIT_FAILURE);
    }

    close(sockfd);
    return 0;
}
