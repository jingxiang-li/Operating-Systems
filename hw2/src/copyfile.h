#ifndef COPYFILE_H
#define COPYFILE_H

/**
 * copy file from source to dest
 * @param  source path to the source file
 * @param  dest   path to the destination file
 * @return        0 on success, otherwise -1
 */
int copy(char *source, char *dest);

#endif //COPYFILE_H
