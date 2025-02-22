#ifndef LOGIN_H
#define LOGIN_H

#define _POSIX_C_SOURCE 200112L
#include <time.h>
#include "serverCommunication.h"

// Stores the tag and the amount of email they have in inbox
typedef struct {
    char *tag;
    int latestEmail;
} userEmails_t;

// Function to login to server and returns struct containing tag and users latest email
userEmails_t login(int sockfd, const char *username, const char *password, const char *folder, int messageNum);

#endif