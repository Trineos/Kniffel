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


    // receive send_mess from server and then send answer to the server
    char send_mess [1000];
    char rec_mess [1000];
    
    while(1)
    {
        // receive data
        recv (client_socket, rec_mess , 2000 , 0);
        printf(rec_mess);
        printf("\n\n Auswahl: ");
        gets(send_mess);

        //Send some data
        if (send_to (client_socket, send_mess, strlen(send_mess)) < 0)
            return -1;
    }

    // close client socket
    if (closesocket (client_socket) != 0)
    {
            printf("failed to close client socket\n");
            return -1;
    }

    return 0;
}