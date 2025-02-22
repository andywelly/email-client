#ifndef SERVERCOMMUNICATION_H
#define SERVERCOMMUNICATION_H

#define _POSIX_C_SOURCE 200112L
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <assert.h>

#define BUFFER_LEN 2048

// Function to recieve message from server, used after command is sent
char *fetchMessage(int sockfd);

// Own implementation of strdup function as not available in C standard library
char* strdup(const char* str);

#endif