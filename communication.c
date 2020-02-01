#include "communication.h"

int send_to (int socket, char* m, int size_of_m)
{
    if( send(socket, m, size_of_m , 0) < 0)
    {
        puts("send error");
        return -1;
    }
    return 0;
}

int initialize_winsocket (WSADATA* wsaData)
{
    int iResult = WSAStartup (MAKEWORD (2,2), wsaData);
    if (iResult != 0)
    {
        printf ("WSAStartup failed: %d\n", iResult);
        return -1;
    }
    printf ("Initialization of Winsock successfully\n");
    return 0;

}

// this function creates a socket and binds the socket to a server IP address, port
int make_server_socket (int port,
                        int* server_sock,
                        struct sockaddr_in* server)
{
    // Create server socket
    *server_sock = socket (AF_INET, SOCK_STREAM, 0); // AF_INET : IPv4, SOCK_STREAM: TCP, 0: weitere Parameter für TCP Verbindung
    if (*server_sock < 0)
    {
        printf ("Failed to create socket, error code: %d\n", WSAGetLastError ());
        return -1;
    }
    else
    {
        printf ("Serversocket successfully created: %d\n", *server_sock);
    }
    
    // make socket address
    server -> sin_family = AF_INET;
    server -> sin_addr.s_addr = INADDR_ANY;
    server -> sin_port = htons (port);

   // bind server socket to socket address
    if (bind (*server_sock, (struct sockaddr *) server , sizeof (*server)) < 0)
    {
        printf ("bind failed. Error\n");
        return -1;
    }
    printf ("bind done\n");
    return 0;
}


int make_client_connection (int port,
                            int* client_socket,
                            struct sockaddr_in* server_addr,
                            char* ip_address)
{
    *client_socket = socket (AF_INET, SOCK_STREAM, 0); // AF_INET : IPv4, SOCK_STREAM: TCP, 0: weitere Parameter für TCP Verbindung
    if (*client_socket < 0)
    {
        printf ("Failed to create socket, error code: %d\n", WSAGetLastError ());
        return -1;
    }
    else
    {
        printf ("Client socket successfully created: %d\n", *client_socket);
    }

    // connect to Server
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr  = inet_addr (ip_address);
    server_addr->sin_port = htons (port);
    
    if (connect (*client_socket, (struct sockaddr *) server_addr, sizeof (*server_addr)) < 0)
    {
        printf ("connection error\n");
        return -1;
    }
    return 0;
}
