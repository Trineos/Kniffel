// server.c


#include <stdio.h>
#include "communication.h"
#include "kniffel.h"
#include <string.h>
#include <time.h>

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


    srand(time(NULL));


    int n_players = 0;
    int n_computer = 0;
    int n_client = 0;
    gamesetup (&n_players, &n_computer, &n_client);

    struct sockaddr_in* client_add = (struct sockaddr_in*) malloc (n_client * sizeof (struct sockaddr_in));
    int* client_socket = (int *) malloc (n_client * sizeof (int));
    
    if (client_add == NULL)
        printf ("client_add failed");
    if (client_socket == NULL)
        printf ("client_socket failed");
    if (connect_to_clients(client_add, client_socket, n_client, server_socket) != 0)
        printf ("connection to clients failed");

    for (int i = 0; i < n_client; i++)
    {
        printf ("client %2d of %d: %d\n", i + 1, n_client, client_socket [i]);
    }

    
    // TODO: GAME LOGIK
    int dice [5] = {0, 0, 0, 0, 0};
    int b [5] = {TRUE, TRUE, TRUE, TRUE, TRUE};

    int read_size;
    char client_message [1000];
    char server_message [1000];
    memset (server_message, 0, sizeof (server_message));
    memset (client_message, 0, sizeof(client_message));
    
    // communication: first server sends message to client, after that, the client responds
    strcpy (server_message, "Auswahl: 1) Wuerfeln, 2) Tabelle 3) Anleitung");
    if (send_to (client_socket [0], server_message, strlen (server_message)))
        return -1;

    // now the communication between the server and the currently active client starts
    while( (read_size = recv (client_socket [0], client_message , 1000 , 0)) > 0 )
    {
        printf (client_message);
        int action =  extract_action (client_message);
        switch (action)
        {
        case 1: // Spieler möchte würfeln
            wuerfel (dice, b, 0);
            sprintf (server_message, "%d %d %d %d %d", dice [0], dice [1], dice [2], dice [3], dice [4]);
            break;

        case 2: // Spieler möchte die Tabelle sehen
            sprintf (server_message, "          | Spieler 1 | Spieler 2 \n1er       | %9d | %9d\n2er       | %9d | %9d", 1,2,3,4);
            break;

        case 3: // Spieler möchte die Tabelle sehen
            strcpy (server_message, " 3 mal Moeglichkeit zu wuerfeln, man kann Wuerfel rausnehmen, im Anschluss auswaehlen");
            break;
        default:
        break;
        }

        send_to (client_socket [0], server_message, sizeof (server_message));


        // resets buffer client_message and server_message
        memset (server_message, 0, sizeof (server_message));
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