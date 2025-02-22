#ifndef CONNECT_H
#define CONNECT_H

#define _POSIX_C_SOURCE 200112L
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#define IMAP_PORT "143"
#define BUFFER_LEN 2048

// Function to establish connection with the given server name
// Inspiration taken from practical 8
int establishConnection(const char *serverName);


#endif