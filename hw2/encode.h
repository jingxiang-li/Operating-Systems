/**
 * This file contains declarations of functions for encoding a file
 *
 * Author: Jingxiang Li
 * Date: Fri 23 Oct 2015 01:13:13 AM CDT
 */

#ifndef ENCODE_H
#define ENCODE_H

#include <stdio.h>

/**
 * encode a given file
 * @param  filepath path to the file
 * @return          0 on success, otherwise -1.
 */
int encode_file(char *filepath);

#endif  // ENCODE_H
