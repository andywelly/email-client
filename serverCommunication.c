#include "serverCommunication.h"

char *fetchMessage(int sockfd) {
    char *msgBuffer = NULL;
    int buffBytes = 0;
    int numbytes = 0;
    int msgSize = 0;
    int totalRead = 0;

    // Peek at message so it remains unread, get message size and next recv reads whole message
    char peekBuffer[BUFFER_LEN];
    buffBytes = recv(sockfd, peekBuffer, sizeof(peekBuffer) - 1, MSG_PEEK);
    if (buffBytes < 0) {
        perror("recv error");
        exit(EXIT_FAILURE);
    }
    peekBuffer[buffBytes] = '\0';

    // Extract the number within {} to get message size
    sscanf(peekBuffer, "%*[^{]{%d}", &msgSize);

    // Remove fetch line
    char *first_line = strchr(peekBuffer, '\n');
    if (first_line != NULL) {
        msgSize += (int) (first_line - peekBuffer);
    }

    // Create message buffer the exact size of the message
    msgBuffer = (char *) malloc((msgSize + 2) * sizeof(char));
    assert(msgBuffer);
    
    // Receive actual message in multiple parts
    while (totalRead < msgSize) {
        numbytes = recv(sockfd, msgBuffer + totalRead, msgSize - totalRead, 0);
        if (numbytes < 0) {
            fprintf(stderr, "recv error");
            free(msgBuffer);
            exit(EXIT_FAILURE);
        }
        totalRead += numbytes;
    }

    // Newline then terminate string
    msgBuffer[msgSize] = '\n';
    msgBuffer[msgSize + 1] = '\0';

    // Message starts after first newline character
    char *message = strchr(msgBuffer, '\n');
    if (message == NULL) {
        fprintf(stdout, "Message not found\n");
        free(msgBuffer);
        exit(3);
    }

    // Remove leading newline
    if (*message == '\n')
        message ++;

    // Duplicate message to be able to free msgBuffer
    char *result = NULL;
    if (message)
        result = strdup(message);

    free(msgBuffer);


    return(result);
}

// Own implementation of strdup function as not available in C standard library
char *strdup(const char *str) {
    char *new_str = malloc(strlen(str) + 1);
    assert(new_str);
    strcpy(new_str, str);
    return new_str;
}