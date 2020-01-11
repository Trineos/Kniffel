// client.c
#include "communication.h"

int main(){

    // Initialize Winsock
    WSADATA wsaData;
    if (initialize_winsocket (& wsaData) != 0)
        return -1;

    // Create client socket and connect to server socket
    int client_socket;
    struct sockaddr_in server_addr;
    if (make_client_connection (PORT, &client_socket, &server_addr, "127.0.0.1") != 0)
        return -1;


    // sending messages to the server
    char message[1000];
    while(1)
    {
        printf("write your message : ");
        gets(message);

        //Send some data
        if( send(client_socket , message , strlen(message) , 0) < 0)
        {
            puts("send error");
            return 1;
        }
    }

    // close client socket
    if (closesocket (client_socket) != 0)
    {
            printf("failed to close client socket\n");
            return -1;
    }

    return 0;
}