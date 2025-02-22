#ifndef PARSE_H
#define PARSE_H

#define _POSIX_C_SOURCE 200112L
#include <strings.h>
#include <ctype.h>
#include "serverCommunication.h"

#define HEADER_FIELDS 4
#define FROM_LABEL "From: "
#define TO_LABEL "To: "
#define DATE_LABEL "Date: "
#define SUBJECT_LABEL "Subject: "

// Struct to store header values
typedef struct {
    char *from;
    char *to;
    char *date;
    char *subject;
} headerValue_t;

// Function to parse information from email
void parse(int sockfd, int messageNum, char *tag);

// Removes labels such as "From: " and returns just the value after
char *getValue(char *token, char* label);

#endif