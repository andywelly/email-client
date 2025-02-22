
#include "login.h"

#define TAG_SIZE 10

userEmails_t login(int sockfd, const char *username, const char *password, const char *folder, int messageNum) {
    userEmails_t user;
    char buffer[BUFFER_LEN];
    char recv_buffer[BUFFER_LEN];
    int numbytes;

    user.tag = (char*)malloc(TAG_SIZE * sizeof(char));
    assert(user.tag);

    // Generate random tag
    srand(time(NULL));
    char random_char = 'A' + rand() % 26;
    int random_num = rand() % 10000;
    sprintf(user.tag, "%c%04d", random_char, random_num);

    // Send login command to server
    sprintf(buffer, "%s LOGIN \"%s\" \"%s\"\r\n", user.tag, username, password);
    if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
        fprintf(stderr, "send error\n");
        exit(EXIT_FAILURE);
    }

    // Recieve response from server
    numbytes = recv(sockfd, recv_buffer, sizeof(recv_buffer) - 1, 0);
    if (numbytes < 0) {
        perror("recv error");
        exit(EXIT_FAILURE);
    }
    recv_buffer[numbytes] = '\0';

    // Check if "OK" is substring of server response
    if (strstr(recv_buffer, "OK") != NULL) { // Login success
        // Send which folder to read from
        sprintf(buffer, "%s SELECT \"%s\"\r\n", user.tag, folder);
        if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
            fprintf(stdout, "send error\n");
            exit(EXIT_FAILURE);
        }

        // Recieve folder
        recv(sockfd, recv_buffer, sizeof(recv_buffer) - 1, 0);
        if (strstr(recv_buffer, "OK") != NULL) {

            // Found folder
            // Get latest email sequence
            char *token = strtok(recv_buffer, " EXISTS");

            while (token != NULL) {
                int latestEmail = atoi(token);
                if (latestEmail != 0) {
                    user.latestEmail = latestEmail;
                    break;
                }
                token = strtok(NULL, " EXISTS");
            }
        } else {
            fprintf(stdout, "Folder not found\n");
            exit(3);
        }

    } else { // Login fails
        fprintf(stdout, "Login failure\n");
        exit(3);
    }

    // Return tag and the latest email position
    return user;
}