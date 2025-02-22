#include "parse.h"

void parse(int sockfd, int messageNum, char *tag) {
    char buffer[BUFFER_LEN];
    char *header = NULL;
    headerValue_t headerValue;
    headerValue.from = NULL;
    headerValue.to = NULL;
    headerValue.date = NULL;
    headerValue.subject = NULL;

    // Fetches all headers at one time
    sprintf(buffer, "%s FETCH %d BODY.PEEK[HEADER.FIELDS (FROM TO DATE SUBJECT)]\r\n", tag, messageNum);
    send(sockfd, buffer, strlen(buffer), 0);
    header = fetchMessage(sockfd);
    char *crlf = "\r\n";
    char *token = strtok(header, crlf);

    // Assign values to represents each header field
    int headerField = 0;

    while (token != NULL) {
        if (strncasecmp(token, FROM_LABEL, strlen(FROM_LABEL)) == 0) {
            headerField = 1;
            char *from_value = getValue(token, FROM_LABEL);
            headerValue.from = from_value;
        } else if (strncasecmp(token, TO_LABEL, strlen(TO_LABEL)) == 0) {
            headerField = 2;
            char *to_value = getValue(token, TO_LABEL);
            headerValue.to = to_value;
        } else if (strncasecmp(token, DATE_LABEL, strlen(DATE_LABEL)) == 0) {
            headerField = 3;
            char *date_value = getValue(token, DATE_LABEL);
            headerValue.date = date_value;
        } else if (strncasecmp(token, SUBJECT_LABEL, strlen(SUBJECT_LABEL)) == 0) {
            headerField = 4;
            char *subject_value = getValue(token, SUBJECT_LABEL);
            headerValue.subject = subject_value;
        } else if (isspace(*token)) {
            // Folded, unfold and append to current string. HeaderField is latest appended field
            switch(headerField) {
                case 1:
                    headerValue.from = realloc(headerValue.from, strlen(headerValue.from) + strlen(token));
                    strcat(headerValue.from, token);
                    break;
                case 2:
                    headerValue.to = realloc(headerValue.to, strlen(headerValue.to) + strlen(token));
                    strcat(headerValue.to, token);
                    break;
                case 3:
                    headerValue.date = realloc(headerValue.date, strlen(headerValue.date) + strlen(token));
                    strcat(headerValue.date, token);
                    break;
                case 4:
                    headerValue.subject = realloc(headerValue.subject, strlen(headerValue.subject) + strlen(token));
                    strcat(headerValue.subject, token);
                    break;
                default:
                    printf("Not appended");
                    break;
            }
        }
        token = strtok(NULL, crlf);
    }

    int subjectIsEmpty = -1;
    int toIsEmpty = -1;

    // If subject is not given print <No subject>
    if (headerValue.subject == NULL) {  
        headerValue.subject = "Subject: <No subject>";
    } else {
        subjectIsEmpty = 1;
    }
    
    // If no to value, print nothing
    if (headerValue.to == NULL) {
        headerValue.to = "To:";
    } else {
        toIsEmpty = 1;
    }

    // Print all header values
    fprintf(stdout, "%s\n", headerValue.from);
    fprintf(stdout, "%s\n", headerValue.to);
    fprintf(stdout, "%s\n", headerValue.date);
    fprintf(stdout, "%s\n", headerValue.subject);

    // Free allocated memory
    free(headerValue.from);
    free(headerValue.date);

    if (toIsEmpty == 1) {
        free(headerValue.to);
    }
    if (subjectIsEmpty == 1) {
        free(headerValue.subject);
    }

    free(header);
}

// Removes labels such as "From: " and returns just the value after
char *getValue(char *token, char* label) {
    // Allocate memory for values
    char *value = malloc(strlen(token) + 1);
    value[0] = '\0';

    // Skip past label in token
    token += strlen(label);

    
    strcat(value, label);
    strcat(value, token);
    return value;
}