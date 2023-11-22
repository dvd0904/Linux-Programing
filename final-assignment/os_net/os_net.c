#include <errno.h>
#include "shared.h"
#include "os_net.h"

#define RECV_SOCK 0
#define SEND_SOCK 1


/* Open a TCP socket */
int OS_Connect(u_int16_t _port, const char *_ip)
{
    int ossock;
    int max_msg_size = OS_MAXSTR + 512;
    struct sockaddr_in server;


    if ((ossock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
        return (-1);

    if ((_ip == NULL) || (_ip[0] == '\0')) {
        OS_CloseSocket(ossock);
        return (-1);
    }


    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(_port);
    server.sin_addr.s_addr = inet_addr(_ip);

    if (connect(ossock, (struct sockaddr *)&server, sizeof(server)) < 0) 
    {
        OS_CloseSocket(ossock);
        return (-1);
    }

    return (ossock);
}

int OS_BindPort(u_int16_t _port)
{
    int os_sock;
    struct sockaddr_in server;

    int flag = 1;
    if ((os_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
        return -1;

    if (setsockopt(os_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&flag,  sizeof(flag)) < 0) 
    {
        OS_CloseSocket(os_sock);
        return -1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(_port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(os_sock, (struct sockaddr *) &server, sizeof(server)) < 0) 
    {
        OS_CloseSocket(os_sock);
        return -1;
    }

    if (listen(os_sock, BACKLOG) < 0) 
    {
        OS_CloseSocket(os_sock);
        return -1;
    }
    

    return os_sock;
}

int OS_AcceptTCP(int socket, char *srcip, size_t addrsize)
{
    int clientsocket;
    struct sockaddr_in new_client;
    socklen_t new_client_len;

    memset(&new_client, 0, sizeof(new_client));
    new_client_len = sizeof(new_client);

    if ((clientsocket = accept(socket, (struct sockaddr *) &new_client, &new_client_len)) < 0) {
        return (-1);
    }

    strncpy(srcip, inet_ntoa(new_client.sin_addr), addrsize - 1);
    srcip[addrsize - 1] = '\0';

    return (clientsocket);
}

char *OS_RecvTCP(int socket, int sizet)
{
    char *ret = NULL;
    ret = (char *)calloc((sizet), sizeof(char));
    
    if (ret == NULL) 
        return (NULL);

    if (recv(socket, ret, sizet - 1, 0) <= 0) 
    {
        free(ret);
        return (NULL);
    }

    return (ret);
}

int OS_SendTCP(int socket, const char *msg)
{
    if ((send(socket, msg, strlen(msg), 0)) <= 0) 
        return (-1);

    return (0);

}

int OS_CloseSocket(int socket)
{
    return (close(socket));
}


