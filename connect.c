#include "connect.h"

int establishConnection(const char *serverName) {
    int sockfd, s;
    struct addrinfo hints, *servinfo, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Unspec to use both IPV6 and IPV4
    hints.ai_socktype = SOCK_STREAM;

    // Get address info of server name
    s = getaddrinfo(serverName, IMAP_PORT, &hints, &servinfo);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for (rp = servinfo; rp != NULL; rp = rp->ai_next) {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1)
            continue;

        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break; // connection success

        close(sockfd);
    }

    // Check for successfull connection
    if (rp == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        exit(EXIT_FAILURE);
    }

    // Free allocated memory
    freeaddrinfo(servinfo);
    return sockfd;
}