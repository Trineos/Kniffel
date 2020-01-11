// communication.h


#include <winsock.h>
#include <stdio.h>

#define PORT 6666

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_


int make_server_socket (int port,
                        int* server_sock,
                        struct sockaddr_in* server);

int initialize_winsocket (WSADATA* wsaData);

int make_client_connection (int port,
                            int* client_socket,
                            struct sockaddr_in* server_addr,
                            char* ip_address);
#endif
