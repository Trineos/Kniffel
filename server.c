// server.c


#include <stdio.h>
#include "communication.h"

int main ()
{
    // Initialize Winsock
    WSADATA wsaData;
    if (initialize_winsocket (& wsaData) != 0 )
        return -1;

    // create server socket
    int server_socket;
    struct sockaddr_in server;
    if (make_server_socket(PORT, &server_socket, &server) != 0)
        return -1;


    // Wait for connections
    int n_client = 1;
    struct sockaddr_in* client_add = (struct sockaddr_in*) malloc (n_client * sizeof (struct sockaddr_in));
    int* client_socket = (int *) malloc (n_client * sizeof (int));
    listen (server_socket, n_client);
    int c = sizeof (struct sockaddr_in);
    
    // accept connection from n_client clients:
    for (int i = 0; i < n_client; i++)
    {
        printf ("waiting for incoming connection on port %d \t (%d | %d ) \n", PORT, i, n_client);    
        client_socket [i] = accept (server_socket, (struct sockaddr *) client_add + i, &c);
        if (client_socket [i] < 0)
        {
            printf ("accept failed\n");
            return -1;
        }
        else
        {
            printf ("connection to client %d accepted\n", client_socket [i]);
        }
    }
    printf ("%d of %d connections accepted\n", n_client, n_client);
    for (int i = 0; i < n_client; i++)
    {
        printf ("client %2d of %d: %d\n", i + 1, n_client, client_socket [i]);
    }

    // receive messages from clients:
    int read_size;
    char client_message [2000];

    while( (read_size = recv (client_socket [0], client_message , 100 , 0)) > 0 )
    {
        printf(client_message);
        printf("\n");
    }


    // close all sockets
    closesocket (server_socket);
    for (int i = 0; i < n_client; i++)
    {
        if (closesocket (client_socket [i]) != 0)
        {
            printf ("failed to close client socket %d", client_socket [i]);
        }
    }

    // clean up
    free (client_add); client_add= NULL;
    free (client_socket); client_socket = NULL;

    return 0;
}