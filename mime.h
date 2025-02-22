#ifndef MIME_H
#define MIME_H

#define _POSIX_C_SOURCE 200112L
#include "serverCommunication.h"

#define BOUNDARY_PREFIX "boundary="

// Given headers that are present after the boundary
#define TRANSFER_HEADER "Content-Transfer-Encoding: quoted-printable"
#define CONTENT_TYPE_HEADER "Content-Type: text/plain;"
#define CHARSET_HEADER "charset=UTF-8"

// Retrieves MIME messages from email server and prints them as plain ASCII
void mime(int sockfd, int messageNum, char* tag);

// Helper function to get the boundary of the email without the BOUNDARY_PREFIX
char *get_boundary(char* email);

#endif