/**
 * This file contains declarations of functions for decoding a file
 *
 * Author: Jingxiang Li
 * Date: Fri 23 Oct 2015 01:13:13 AM CDT
 */

#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>

/**
 * decode a given file
 * @param  filepath path to the file
 * @return          0 on success, otherwise -1.
 */
int decode_file(char *filepath);

#endif  // DECODE_H
