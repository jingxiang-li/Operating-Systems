/**
 * This file contains the main function for the server side of twitterTrend
 *
 * Author: Jingxiang Li
 * Date: Thu 03 Dec 2015 05:02:13 PM CST
 */

#include "./client_db.h"
#include "./message.h"
#include "./utility.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
