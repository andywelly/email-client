#include "list.h"

void list(int sockfd, int numOfMessage, char *tag) {
    char buffer[BUFFER_LEN];
    char recv_buffer[BUFFER_LEN];
    char *subject;
    

    for (int i = 1; i <= numOfMessage; i++) {
        // Send command
        sprintf(buffer, "%s FETCH %d BODY.PEEK[HEADER.FIELDS (SUBJECT)]\r\n", tag, i);
        send(sockfd, buffer, strlen(buffer), 0);
        
        // Recv command
        ssize_t bytes_received = recv(sockfd, recv_buffer, sizeof(recv_buffer) - 1, 0);
        if (bytes_received < 0) {
            perror("recv error");
            exit(EXIT_FAILURE);
        }
        recv_buffer[bytes_received] = '\0';

        subject = NULL;
        char *crlf = "\r\n";
        char *token = strtok(recv_buffer, crlf);

        // While still recieving keep looping
        while (token != NULL) {
            if (strncasecmp(token, SUBJECT_LABEL, strlen(SUBJECT_LABEL)) == 0) {
                char *subject_value = getValue(token, SUBJECT_LABEL);

                // Skip "Subject" prefix
                subject = strdup(subject_value + strlen(SUBJECT_LABEL));
                assert(subject);
                free(subject_value);

            } else if (isspace(*token) && subject != NULL) {
                // Folded, unfold and append to current string. HeaderField is latest appended field
                subject = realloc(subject, strlen(subject) + strlen(token) + 1);
                assert(subject);
                strcat(subject, token);
            }

            token = strtok(NULL, crlf);
        }
        
        if (subject == NULL) {
            subject = strdup("<No subject>");
        }

        fprintf(stdout, "%d: %s\n", i, subject);

        // Free allocated memory
        free(subject);
    }
}
