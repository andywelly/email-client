#include "retrieve.h"

void retrieve(int sockfd, int messageNum, char *tag) {
    
    // Send fetch command to server
    char sendBuffer[BUFFER_LEN];
    sprintf(sendBuffer, "%s FETCH %d BODY.PEEK[]\r\n", tag, messageNum);
    if (send(sockfd, sendBuffer, strlen(sendBuffer), 0) < 0) {
        fprintf(stderr, "send error\n");
        exit(EXIT_FAILURE);
    }

    // Get response from server
    char *rawEmail = fetchMessage(sockfd);

    // Print email response
    fprintf(stdout, "%s", rawEmail);

    // Free allocated memory
    free(rawEmail);
}
