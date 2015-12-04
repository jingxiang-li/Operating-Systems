#include "client_db.h"
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int kBufferSize = 256;

Client_DB *create_client_db(const char *filepath) {
    if (NULL == filepath) {
        fprintf(stderr,
                "Failed to create the client_db, illegal access to a NULL "
                "pointer\n");
        return NULL;
    }

    FILE *client_db_file = fopen(filepath, "r");
    if (NULL == client_db_file) {
        fprintf(stderr, "Failed to open the Client_DB file %s\n", filepath);
        perror(NULL);
        return NULL;
    }

    Client_DB *client_db = (Client_DB *)malloc(sizeof(Client_DB));
    if (NULL == client_db) {
        perror("Failed to allocate space for the clinet_db");
        return NULL;
    }

    memset(client_db->clients, 0, MAX_DB_SIZE);
    client_db->size = 0;

    char line_buffer[kBufferSize];

    while (NULL != fgets(line_buffer, kBufferSize, client_db_file)) {
        if (strcmp(line_buffer, "\n") == 0) continue;

        Client **client = &client_db->clients[client_db->size];
        *client = (Client *)malloc(sizeof(Client));
        if (NULL == *client) {
            perror("Failed to allocate space for client");
            return NULL;
        }

        // initialize the client
        line_buffer[strlen(line_buffer) - 1] = '\0';

        // name of the client
        strncpy((*client)->name, line_buffer, MAX_CLIENT_NAME_SIZE);

        // index of the client
        (*client)->index = client_db->size;

        // city_name of the client
        char *filepath_dup = strdup(filepath);
        char client_filepath[kBufferSize];
        sprintf(client_filepath, "%s/%s", dirname(filepath_dup), line_buffer);
        if (-1 == get_city_name(client_filepath, (*client)->city_name)) {
            fprintf(stderr, "Failed to get the city name from %s\n",
                    client_filepath);
            return NULL;
        }
        free(filepath_dup);

        (client_db->size)++;
    }

    fclose(client_db_file);
    return client_db;
}

int free_client_db(Client_DB *client_db) {
    if (NULL == client_db) {
        fprintf(
            stderr,
            "Failed to free the client_db, illegal access to a NULL pointer\n");
        return -1;
    }

    for (int i = 0; i != client_db->size; i++) {
        free(client_db->clients[i]);
    }

    free(client_db);

    return 0;
}

Client *get_client(Client_DB *client_db, int index) {
    if (NULL == client_db) {
        fprintf(stderr,
                "Failed to get client information from the client_db, illegal "
                "access to a NULL pointer\n");
        return NULL;
    }

    return client_db->clients[index];
}

int get_city_name(char *filepath, char *city_name) {
    if (NULL == filepath || NULL == city_name) {
        fprintf(stderr,
                "Failed to get city name from the client file, either filepath "
                "or city_name is NULL\n");
        return -1;
    }

    FILE *input_file = fopen(filepath, "r");
    if (NULL == input_file) {
        fprintf(stderr, "Failed to open %s\n", filepath);
        perror(NULL);
        return -1;
    }

    char line_buffer[kBufferSize];
    if (NULL == fgets(line_buffer, kBufferSize, input_file)) {
        perror("Failed to get the city name from the client file in fgets");
    }
    fclose(input_file);

    line_buffer[strlen(line_buffer) - 1] = '\0';
    strncpy(city_name, line_buffer, MAX_CITY_NAME_SIZE);

    return 0;
}

void print_client_db(Client_DB *client_db) {
    printf("\n\tClient DataBase\n");

    int size = client_db->size;
    for (int i = 0; i != size; i++) {
        Client *client = client_db->clients[i];
        printf("index: %d\tclient_name: %s\tcity_name: %s\n", client->index,
               client->name, client->city_name);
    }

    return;
}
