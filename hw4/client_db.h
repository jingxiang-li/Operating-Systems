/**
 * This file contains declarations for a client database which should be
 * parsed from a input text file
 *
 * Author: Jingxiang Li
 * Date: Tue 03 Nov 2015 11:46:36 AM CST
 */

#ifndef CLIENT_DB_H
#define CLIENT_DB_H

#ifndef MAX_DB_SIZE
#define MAX_DB_SIZE 64
#endif

#ifndef MAX_CITY_NAME_SIZE
#define MAX_CITY_NAME_SIZE 32
#endif

#ifndef MAX_CLIENT_NAME_SIZE
#define MAX_CLIENT_NAME_SIZE 32
#endif

/**
 * definition of the Client_t, which is the entry for the client database
 */
typedef struct Client_t {
    int index;
    char city_name[MAX_CITY_NAME_SIZE];
} Client;

/**
 * definition of the Client_DB_t which is the client database
 */
typedef struct Client_DB_t {
    Client *clients[MAX_DB_SIZE];
    int size;
} Client_DB;

/**
 * initialize a Client_DB instance
 * @param  filepath path to the input file
 * @return          a pointer to the new Client_DB instance on success;
 * otherwise NULL
 */
Client_DB *create_client_db(const char *filepath);

/**
 * free a Client_DB instance
 * @param  client_db pointer to a Client_DB instance
 * @return           0 on success; -1 otherwise
 */
int free_client_db(Client_DB *client_db);

/**
 * get a Client specified by the index from a Client_DB instance
 * @param  client_db pointer to a Client_DB instance
 * @param  index     index of the Client
 * @return           0 on success; -1 otherwise
 */
Client *get_client(Client_DB *client_db, int index);

/**
 * print the client database to stdout
 * @param client_db pointer to the client database instance
 */
void print_client_db(Client_DB *client_db);

#endif  // CLIENT_DB_H
