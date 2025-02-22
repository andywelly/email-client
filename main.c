#define _POSIX_C_SOURCE 200112L

#define IMAP_PORT "143"
#define DEFAULT_FOLDER "INBOX" // Default folder given in spec

#include "login.h"
#include "connect.h"
#include "retrieve.h"
#include "parse.h"
#include "serverCommunication.h"
#include "mime.h"
#include "list.h"


int main(int argc, char const *argv[]) {
    const char *username = NULL;
    const char *password = NULL;
    const char *folder = NULL;
    int messageNum = 0;
    int numEmpty = -1; // Checks if messageNum is empty or not
    const char *serverName = NULL;
    const char *command = NULL;
    int commandFound = 0;

    // Minimum number of required arguments is 7, not including optional arguments
    if (argc < 7) {
        fprintf(stderr, "Insufficient Num Arguments\n");
        exit(EXIT_FAILURE);
    }


    // Taken from project 1
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            folder = argv[++i];
        } else if (strcmp(argv[i], "-u") == 0) {
            username = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0) {
            password = argv[++i];
        } else if (strcmp(argv[i], "-n") == 0) {
            numEmpty = 1;
            // Validate messageNum
            const char *num = argv[++i];
            int isValid = 1;
            for (const char *p = num; *p != '\0'; ++p) {
                if (!isdigit(*p)) {
                    isValid = 0;
                    break;
                }
            }
            // If input is valid, convert to integer
            if (isValid) {
                messageNum = atoi(num);
            } else {
                fprintf(stdout, "Message not found\n");
                exit(3);
            }
            // Given allowed commands
        } else if (strcmp(argv[i], "retrieve") == 0 || 
                    strcmp(argv[i], "parse") == 0 || 
                    strcmp(argv[i], "mime") == 0 || 
                    strcmp(argv[i], "list") == 0) 
                {
                    command = argv[i];
                    commandFound = 1;
                }
        else if (commandFound == 1) {
            serverName = argv[i];
        }
    }
    
    // Username, password, command, server name required arguments
    if (username == NULL) {
        fprintf(stderr, "Missing Username\n");
        exit(EXIT_FAILURE);
    }
    if (password == NULL) {
        fprintf(stderr, "Missing Password\n");
        exit(EXIT_FAILURE);
    }
    if (command == NULL) {
        fprintf(stderr, "Missing Command\n");
        exit(EXIT_FAILURE);
    }
    if (serverName == NULL) {
        fprintf(stderr, "Missing Server Name\n");
        exit(EXIT_FAILURE);
    }

    // If folder is not given, set to default folder "inbox"
    if (folder == NULL) {
        folder = DEFAULT_FOLDER;
    }
    // Create Socket and establish connection with server
    int sockfd = establishConnection(serverName);
    
    // Complete initial handshake
    char handshake[BUFFER_LEN];
    int n_bytes;
    n_bytes = recv(sockfd, handshake, BUFFER_LEN, 0);
    if (n_bytes < 0) {
        fprintf(stderr, "recv error\n");
        exit(EXIT_FAILURE);
    }
    handshake[n_bytes] = '\0';

    // Complete user login
    userEmails_t user = login(sockfd, username, password, folder, messageNum);

    // If messageNum given is after the user's last email throw error and no number given in cli
    if ((messageNum > user.latestEmail || messageNum <= 0) && numEmpty != -1) {
        fprintf(stdout, "Message not found\n");
        exit(3);
    }

    // If number is not given, set messageNum as the users last email
    if (numEmpty == -1) {
        messageNum = user.latestEmail;
    }

    // Complete the command specified
    if (command != NULL) {
        switch (command[0]) {
            case 'r': // retrieve
                retrieve(sockfd, messageNum, user.tag);
                break;
            case 'p': // parse
                parse(sockfd, messageNum, user.tag);
                break;
            case 'm': // mime
                mime(sockfd, messageNum, user.tag);
                break;
            case 'l': // list
                list(sockfd, user.latestEmail, user.tag);
                break;
            default:
                fprintf(stderr, "Invalid Command\n");
                exit(EXIT_FAILURE);
                break;
        }
    }

    // Closes socket and frees allocated user tag
    close(sockfd);
    free(user.tag);
    return 0;
}