/**
 * This file contains implementations for twitter_db.h
 *
 * Author: Jingxiang Li
 * Date: Mon 02 Nov 2015 11:54:11 PM CST
 */

#include "./twitter_db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int kBufferSize = 256;

DB *create_db(const char *filepath) {
    if (NULL == filepath) {
        fprintf(
            stderr,
            "Failed to create database, illegal access to a NULL pointer\n");
        return NULL;
    }

    FILE *db_file = fopen(filepath, "r");
    if (NULL == db_file) {
        fprintf(stderr, "Failed to open the DB file %s\n", filepath);
        perror(NULL);
        return NULL;
    }

    DB *db = (DB *)malloc(sizeof(DB));
    if (NULL == db) {
        perror("Failed to allocate space for DB");
        return NULL;
    }
    memset(db->db_entries, 0, MAX_DB_SIZE);
    db->size = 0;

    // parse the DB file line by line
    char line_buffer[kBufferSize];

    while (NULL != fgets(line_buffer, kBufferSize, db_file)) {
        if (strcmp(line_buffer, "\n") == 0) continue;

        DB_ENTRY **db_entry = &db->db_entries[db->size];
        *db_entry = (DB_ENTRY *)malloc(sizeof(DB_ENTRY));
        if (NULL == *db_entry) {
            perror("Failed to allocate space for db_entry");
            return NULL;
        }

        int parse_result = parse_line(line_buffer, (*db_entry)->city_name,
                                      (*db_entry)->keywords);
        if (-1 == parse_result) {
            fprintf(stderr, "Failed to parse line %s\n", line_buffer);
            return NULL;
        }

        (db->size)++;
    }

    fclose(db_file);
    return db;
}

int free_db(DB *db) {
    if (NULL == db) {
        fprintf(stderr,
                "Failed to free the db, illegal access to a NULL pointer\n");
        return -1;
    }

    for (int i = 0; i != db->size; i++) {
        free(db->db_entries[i]);
    }

    free(db);

    return 0;
}

char *get_keywords(DB *db, char *city_name) {
    if (NULL == db) {
        fprintf(stderr,
                "Failed to access the db, illegal access to a NULL pointer\n");
        return NULL;
    }

    if (NULL == city_name) {
        fprintf(
            stderr,
            "Failed to read the city_name, illegal access to a NULL pointer\n");
        return NULL;
    }

    char *ret_keywords = (char *)malloc(MAX_KEYWORDS_SIZE * sizeof(char));

    for (int i = 0; i != db->size; i++) {
        DB_ENTRY *db_entry = db->db_entries[i];
        if (0 == strcmp(city_name, db_entry->city_name)) {
            strncpy(ret_keywords, db_entry->keywords, MAX_KEYWORDS_SIZE);
            return ret_keywords;
        }
    }

    fprintf(stderr, "Failed to match the city_name %s\n", city_name);
    return NULL;
}

int parse_line(const char *line, char *city_name, char *keywords) {
    if (NULL == line || NULL == city_name || NULL == keywords) {
        fprintf(stderr,
                "Illegal access to either line, city_name or keywords in "
                "function parse_line\n");
        return -1;
    }

    char line_copy[kBufferSize];
    strncpy(line_copy, line, kBufferSize);

    int size_of_line = strlen(line_copy);

    int i;
    for (i = 0; i != size_of_line; i++) {
        if (line_copy[i] == ',') break;
    }

    line_copy[i] = '\0';
    // remove the '\n' after the line
    line_copy[size_of_line - 1] = '\0';

    strncpy(city_name, line_copy, MAX_CITY_NAME_SIZE);
    strncpy(keywords, line_copy + i + 1, MAX_KEYWORDS_SIZE);
    return 0;
}

void print_db(DB *db) {
    printf("\n\tTwitter Keywords DataBase\n");

    int size = db->size;
    for (int i = 0; i != size; i++) {
        DB_ENTRY *entry = db->db_entries[i];
        printf("index: %d\tcity_name: %s\tkeywords: %s\n", i, entry->city_name,
               entry->keywords);
    }
    return;
}
