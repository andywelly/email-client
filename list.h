#ifndef LIST_H
#define LIST_H

#define _POSIX_C_SOURCE 200112L
#include "parse.h"
#define SUBJECT_LABEL "Subject: "

// Function to print subject lines of emails in folder
void list(int sockfd, int numOfMessage, char *tag);

#endif