/**
 * This file contains the main function for the client side of twitterTrend
 *
 * Author: Jingxiang Li
 * Date: Thu 03 Dec 2015 05:02:13 PM CST
 */

#include "./queue.h"
#include "./client_db.h"
#include "./message.h"
#include "./client_utility.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <libgen.h>
#include <errno.h>

static const int kBufferSize = 256;

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr,
                "Usage: ./twitterTrend_client server_host_name "
                "server_port_number file_path1 file_path2 ...\n");
        exit(EXIT_FAILURE);
    }

    // parse arguments
    char *server_host_name = argv[1];
    int server_port_number = atoi(argv[2]);


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
    memcpy(&server_addr.sin_addr.s_addr, server_entry->h_addr_list[0],
           server_entry->h_length);
    server_addr.sin_port = htons(server_port_number);

    if (-1 ==
        connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
        perror("ERROR, failed to connect to the host");
        exit(EXIT_FAILURE);
    }
    printf("client connects\n");

    // INTERACT WITH HOST ==================================================
    uint32_t msg_id, payload_len;
    char *payload = NULL;
    int status;

    // receive hand shake message
    receive_msg(sockfd, &msg_id, &status);
    if (-1 == status || HANDSHAKE_ID != msg_id) {
        fprintf(stderr,
                "ERROR, failed to read hand shake response from host\n");
        exit(EXIT_FAILURE);
    }

    // send shake message response
    while (-1 == send_msg(sockfd, HANDSHAKE_RESPONSE_ID, 0, NULL)) {
        if (ECONNREFUSED == errno) continue;
        fprintf(stderr, "ERROR, failed to send hand shake message to host\n");
        exit(EXIT_FAILURE);
    }
    printf("client sends handshake response: (101,0,)\n");

    // PROCESS FILES ==========================================================
    int num_files = argc - 3;
    for (int i = 0; i != num_files; i++) {
        // READ DATABASE ======================================================
        char *input_file_path = argv[3 + i];

        // create the client database
        Client_DB *client_db = create_client_db(input_file_path);
        if (NULL == client_db) exit(EXIT_FAILURE);

        // create output file
        char output_path[kBufferSize];
        sprintf(output_path, "%s.result", input_file_path);
        FILE *output_file = fopen(output_path, "w");
        if (NULL == output_file) {
            fprintf(stderr, "Failed to create file %s\n", output_path);
            perror(NULL);
            return -1;
        }

        // send city names from the client_db and receive response from host
        for (int i = 0; i != client_db->size; i++) {
            // get client information
            Client *client = get_client(client_db, i);
            payload_len = strlen(client->city_name);
            payload = client->city_name;

            // send request to host
            if (-1 == send_msg(sockfd, TWITTERTREND_REQUEST_ID, payload_len,
                               payload)) {
                fprintf(stderr,
                        "ERROR, failed to send keywords request to host\n");
                exit(EXIT_FAILURE);
            }
            printf("client sends twitterTrend request: (102,%d,\"%s\")\n",
                   payload_len, payload);

            // receive keywords from host
            payload = receive_msg(sockfd, &msg_id, &status);
            if (-1 == status || RESPONSE_MESSAGE_ID != msg_id ||
                NULL == payload) {
                fprintf(
                    stderr,
                    "ERROR, failed to receive keywords from host for city %s\n",
                    client->city_name);
                exit(EXIT_FAILURE);
            }
            // printf("%s : %s\n", client->city_name, payload);
            compile_output(output_file, client->city_name, payload);

            // receive end of response message from host
            receive_msg(sockfd, &msg_id, &status);
            if (-1 == status || END_OF_RESPONSE_ID != msg_id) {
                fprintf(stderr,
                        "ERROR, failed to receive end of response message from "
                        "host\n");
                exit(EXIT_FAILURE);
            }
        }
        fclose(output_file);
        free_client_db(client_db);
    }

    // send end of request message to host
    if (-1 == send_msg(sockfd, END_OF_REQUEST_ID, 0, NULL)) {
        fprintf(stderr,
                "ERROR, failed to send end of request message to host\n");
        exit(EXIT_FAILURE);
    }
    printf("client sends end of request: (104,0,)\n");
    close(sockfd);
    return 0;
}
