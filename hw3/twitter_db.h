/**
 * This file contains declarations for a twitter database data structure
 * which should be parsed from the text file "TwitterDB.txt"
 *
 * Author: Jingxiang Li
 * Date: Tue 03 Nov 2015 12:40:42 AM CST
 */

#ifndef TWITTER_DB_H
#define TWITTER_DB_H

#define MAX_DB_SIZE 64
#define MAX_CITY_NAME_SIZE 32
#define MAX_KEYWORDS_SIZE 128

/**
 * definition of DB_ENTRY_t which is the entry for the twitter database
 */
typedef struct DB_ENTRY_t {
    char city_name[MAX_CITY_NAME_SIZE];
    char keywords[MAX_KEYWORDS_SIZE];
} DB_ENTRY;

/**
 * definition of the DB_t which is the twitter database
 */
typedef struct DB_t {
    DB_ENTRY *db_entries[MAX_DB_SIZE];
    int size;
} DB;

/**
 * initialize a DB instance
 * @param  filepath path to the db file
 * @return          pointer to the DB instanc on success; otherwise NULL.
 */
DB *create_db(const char *filepath);

/**
 * free the DB instance
 * @param  db pointer to the DB instance
 * @return    0 on success; otherwise -1.
 */
int free_db(DB *db);

/**
 * return the keywords for corresponding city_name
 * @param  db        pointer to a DB instance
 * @param  city_name name of the city to be matched in the DB
 * @return           pointer to the keywords string on success;
 * NULL otherwise
 */
char *get_keywords(DB *db, char *city_name);

/**
 * parse a line from the db file to two strings: city_name and keywords
 * @param  line      the line to be parsed, '\0' terminated. The format of
 * the line should be "city_name,keyword1,keyword2,keyword3"
 * @param  city_name address of string city_name to be modified
 * @param  keywords address of string keywords to be modified
 * @return           0 on success; -1 otherwise.
 */
int parse_line(const char *line, char *city_name, char *keywords);

#endif  // TWITTER_DB_H
