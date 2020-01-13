// server.c


#include <stdio.h>
#include "communication.h"

void gamesetup (int* n_players,
                int* n_computer,
                int* n_client)
{
    printf ("==================================");
    printf ("========= KNIFFEL-Server =========");
    printf ("==================================\n\n");
    printf ("Anzahl der Spieler eingeben: ");
    scanf ("%d", n_players);
    printf ("\nAnzahl der Computergegner eingeben: ");
    scanf ("%d", n_computer);
    *n_client = *n_players - *n_computer;
    printf("\n\nEin Spiel mit %d, davon %d menschliche und %d Computer Spieler", *n_players, *n_client, *n_computer);
}

int connect_to_clients (struct sockaddr_in* client_add,
                        int* client_socket,
                        int n_client,
                        int server_socket)
{

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
    return 0;

}


int extract_action (char* m)
{
    return (int) *m - '0';
    
}

void answer_to_action (int action,
                       int client_socket)
{
    switch (action)
    {
        case 1: printf("send Aktion 1 to %d\n", client_socket); break;
        case 2: printf("send Aktion 2 to %d\n", client_socket); break;
        default: printf("ungueltige Eingabe\n"); break;
    }
    return;
}

// Aktionen, die der Client zum Server sendet, der Server muss darauf antworten
void wuerfel ();
void get_table ();




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

    int n_players = 0;
    int n_computer = 0;
    int n_client = 0;
    gamesetup (&n_players, &n_computer, &n_client);

    struct sockaddr_in* client_add = (struct sockaddr_in*) malloc (n_client * sizeof (struct sockaddr_in));
    if (client_add == NULL)
    {
        printf ("client_add failed");
    }
    int* client_socket = (int *) malloc (n_client * sizeof (int));
    if (client_socket == NULL)
    {
        printf ("client_socket failed");
    }

    if (connect_to_clients(client_add, client_socket, n_client, server_socket) != 0)
    {
        printf ("connection to clients failed");
        return -1;
    }

    for (int i = 0; i < n_client; i++)
    {
        printf ("client %2d of %d: %d\n", i + 1, n_client, client_socket [i]);
    }

    
    // TODO: GAME LOGIK

    

    // receive messages from clients:
    int read_size;
    char client_message [2000];

    while( (read_size = recv (client_socket [0], client_message , 2000 , 0)) > 0 )
    {
        extract_action (client_message);
        printf ("message: ");
        printf (client_message);
        printf (" ");
        int action =  extract_action (client_message);
        answer_to_action (action, client_socket[0]);

        memset (client_message, 0, sizeof(client_message));
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