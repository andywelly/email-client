
#include "mime.h"

void mime(int sockfd, int messageNum, char* tag) {
    
    // Send fetch message to server
    char sendBuffer[BUFFER_LEN];
    sprintf(sendBuffer, "%s FETCH %d BODY.PEEK[]\r\n", tag, messageNum);
    if (send(sockfd, sendBuffer, strlen(sendBuffer), 0) < 0) {
        fprintf(stderr, "send error\n");
        exit(EXIT_FAILURE);
    }
    
    // Recieve full email
    char *email = fetchMessage(sockfd);
    
    // Parse the boundary from the email
    char *boundary = get_boundary(email);
    
    // Add "--" to the boundary as this specifies start of the email
    char* full_boundary = malloc(strlen(boundary) + 3);
    assert(full_boundary);
    strcpy(full_boundary, "--");
    strcat(full_boundary, boundary);
    
    // First occurence of full_boundary is where the message starts
    char *message_start = strstr(email, full_boundary);

    // Skip past message start
    message_start += strlen(full_boundary);

    // Second occurence of full_boundary is where the messsage ends
    char *message_end = strstr(message_start, full_boundary);

    // Null terminate end of message
    if (message_end)
        *(message_end - 2) = '\0';

    // Find the occurences of given headers
    char *transfer = strstr(message_start, TRANSFER_HEADER);
    char *content_type = strstr(message_start, CONTENT_TYPE_HEADER);
    char *charset = strstr(message_start, CHARSET_HEADER);

    // If any headers and not found, a matching error thrown
    if (transfer == NULL || content_type == NULL || charset == NULL) {
        fprintf(stdout, "Matching Error");
        exit(4);
    } else { // All headers are found
        // Skip over all the headers, start from the end of the last header
        if (transfer > charset) {
            message_start = transfer;
            message_start += strlen(TRANSFER_HEADER) + 4;
        } else {
            message_start = charset;
            message_start += strlen(CHARSET_HEADER) + 4;
        }
    }

    printf("%s", message_start);

    // Free allocated memory
    free(email);
    free(boundary);
    free(full_boundary);
}


// Helper function to get the boundary of the email without the BOUNDARY_PREFIX
char *get_boundary(char* email) {
    // Find occurence of "boundary:" in email
    char *start = strstr(email, BOUNDARY_PREFIX);

    if (!start)
        return NULL;

    // Skip over "boundary:"
    start += strlen(BOUNDARY_PREFIX);

    char *end;
    // If boundary starts with " also ends with "
    if (*start == '"') {
        start ++;
        end = strchr(start, '"');
    } else { // Boundary end denoated by newline
        end = strchr(start, '\n');
    }

    // Allocate memory for boundary
    char *boundary = malloc(end - start + 3);
    assert(boundary);
    strncpy(boundary, start, end - start);
    boundary[end - start] = '\0';

    return boundary;

}