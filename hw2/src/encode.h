#ifndef ENCODE_H
#define ENCODE_H

#include <stdio.h>

/**
 * encode a given file
 * @param  filepath path to the file
 * @return          0 on success, otherwise -1.
 */
int encode_file(char *filepath);


int encode_file_report(char *filepath, FILE *report_file);

#endif  // ENCODE_H
