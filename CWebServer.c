#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define APP_MAX_BUFFER 1024
#define PORT 8080

int main()
{
    int server_fd; // listening socket
    int client_fd; // connected client socket

    // Create a new socket address object
    struct sockaddr_in address;
    int address_len = sizeof(address);

    // Application-level buffer (NOT Send/Receive kernel buffer)
    char buffer[APP_MAX_BUFFER] = {0};

    // Create socket (kernel object)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Specify Address information (IP Version, IP Address, Port, etc.)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to IP:PORT
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Start listening (creates SYN & accept queues)
    if (listen(server_fd, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // 4. Server loop
    while (1)
    {
        printf("Waiting for connection...\n");

        // Accept blocks until a connection is ready
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&address_len);
        if (client_fd < 0)
        {
            perror("accept");
            continue;
        }

        // Read HTTP request (blocking)
        read(client_fd, buffer, APP_MAX_BUFFER);
        printf("Request:\n%s\n", buffer);

        // Write HTTP response
        const char *response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n\r\n"
            "Hello world!\n";

        write(client_fd, response, strlen(response));

        // Close connection
        close(client_fd);
    }

    return 0;
}
