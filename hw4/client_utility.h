/**
 * This file contains declarations of some utility functions for the
 * client side of twitterTrend
 *
 * Author: Jingxiang Li
 * Date: Thu 03 Dec 2015 09:41:03 PM CST
 */

#ifndef CLIENT_UTILITY_H
#define CLIENT_UTILITY_H

/**
 * change the currect working directory to the directory holding
 * the input file
 * @param  input_file_path path to the input file
 * @return                 0 on success; otherwise -1
 */
int change_wd(char *input_file_path);

/**
 * compile the output file for the client, named "client_name.result"
 * @param  client_name name of the client (file)
 * @param  city_name   name of the city
 * @param  keywords    keywords
 * @return             0 on success; -1 otherwise
 */
int compile_output(char *client_name, char *city_name, char *keywords);

#endif  // CLIENT_UTILITY_H
