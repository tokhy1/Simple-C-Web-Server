// Windows Web Server for supporting Windows platform.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
// For gcc compilation
#pragma comment(lib, "ws2_32.lib")

#define APP_MAX_BUFFER 1024
#define PORT 8080

int main()
{
    // Create socket, connection file descriptors
    SOCKET server_fd, client_fd;

    // Winsock requires initialization BEFORE using sockets
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create a new socket address object
    struct sockaddr_in address;
    int address_len = sizeof(address);

    // Initialize Application-level buffer (NOT Send/Receive kernel buffer)
    char buffer[APP_MAX_BUFFER] = {0};

    // Create socket (kernel object)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET)
    {
        printf("Socket failed\n");
        WSACleanup();
        return 1;
    }

    // Specify Address information (IP Version, IP Address, Port, etc.)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to IP:PORT
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
    {
        printf("Bind failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Start listening (creates SYN & accept queues)
    if (listen(server_fd, 10) == SOCKET_ERROR)
    {
        printf("Listen failed\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Server loop
    while (1)
    {
        printf("\nWaiting for a connection...\n");

        // Accept blocks until a connection is ready
        client_fd = accept(server_fd, (struct sockaddr *)&address, &address_len);
        if (client_fd == INVALID_SOCKET)
        {
            printf("Accept failed\n");
            continue;
        }

        // Read HTTP request (blocking)
        recv(client_fd, buffer, APP_MAX_BUFFER, 0);
        printf("Request:\n%s\n", buffer);

        // Write HTTP response
        char *http_response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n\r\n"
            "Hello world!\n";

        send(client_fd, http_response, strlen(http_response), 0);

        closesocket(client_fd);
    }

    // Cleanup Winsock when done
    closesocket(server_fd);
    WSACleanup();

    return 0;
}