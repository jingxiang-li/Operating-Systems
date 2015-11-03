/**
 * This file contains declarations of some utility functions for twitterTrend
 *
 * Author: Jingxiang Li
 * Date: Tue 03 Nov 2015 04:34:02 PM CST
 */

#ifndef UTILITY_H
#define UTILITY_H

/**
 * change the currect working directory to the directory holding
 * the input file
 * @param  input_file_path path to the input file
 * @return                 0 on success; otherwise -1
 */
int change_wd (char *input_file_path);

#endif  // UTILITY_H
