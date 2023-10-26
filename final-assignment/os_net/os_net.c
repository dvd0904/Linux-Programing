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
        return (OS_SOCKTERR);

    if ((_ip == NULL) || (_ip[0] == '\0')) {
        OS_CloseSocket(ossock);
        return (OS_INVALID);
    }


    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons( _port );
    server.sin_addr.s_addr = inet_addr(_ip);

    if (connect(ossock, (struct sockaddr *)&server, sizeof(server)) < 0) 
    {
        OS_CloseSocket(ossock);
        return (OS_SOCKTERR);
    }
    
    // Set socket maximum size
    if (OS_SetSocketSize(ossock, RECV_SOCK, max_msg_size) < 0) 
    {
        OS_CloseSocket(ossock);
        return (OS_SOCKTERR);
    }
    if (OS_SetSocketSize(ossock, SEND_SOCK, max_msg_size) < 0) 
    {
        OS_CloseSocket(ossock);
        return (OS_SOCKTERR);
    }

    return (ossock);
}



/* OS_Bindport*
 * Bind a specific port (protocol and a ip).
 * If the IP is not set, it is going to use ADDR_ANY
 * Return the socket.
 */
int OS_Bindport(u_int16_t _port, const char *_ip)
{
    int os_sock;
    struct sockaddr_in server;

    int flag = 1;
    if ((os_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
        return (int)(OS_SOCKTERR);

    if (setsockopt(os_sock, SOL_SOCKET, SO_REUSEADDR,
                    (char *)&flag,  sizeof(flag)) < 0) 
    {
        OS_CloseSocket(os_sock);
        return (OS_SOCKTERR);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(_port);

    
    if ((_ip == NULL) || (_ip[0] == '\0')) 
        server.sin_addr.s_addr = htonl(INADDR_ANY);
    else 
        server.sin_addr.s_addr = inet_addr(_ip);

    
    if (bind(os_sock, (struct sockaddr *) &server, sizeof(server)) < 0) 
    {
        OS_CloseSocket(os_sock);
        return (OS_SOCKTERR);
    }

    if (listen(os_sock, BACKLOG) < 0) 
    {
        OS_CloseSocket(os_sock);
        return (OS_SOCKTERR);
    }
    

    return (os_sock);
}


/* Set the maximum buffer size for the socket */
int OS_SetSocketSize(int sock, int mode, int max_msg_size) 
{

    int len;
    socklen_t optlen = sizeof(len);

    if (mode == RECV_SOCK) 
    {
        /* Get current maximum size */
        if (getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&len, &optlen) == -1)
            len = 0;
    
        /* Set maximum message size */
        if (len < max_msg_size) 
        {
            len = max_msg_size;
            if (setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const void *)&len, optlen) < 0) 
                return -1;
            
        }

    } else if (mode == SEND_SOCK) 
    {
        /* Get current maximum size */
        if (getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&len, &optlen) == -1)
            len = 0;
        
        /* Set maximum message size */
        if (len < max_msg_size) 
        {
            len = max_msg_size;
            if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const void *)&len, optlen) < 0) 
                return -1;  
        }
    }

    return 0;
}

/* Accept a TCP connection */
int OS_AcceptTCP(int socket, char *srcip, size_t addrsize)
{
    int clientsocket;
    struct sockaddr_in new_client;
    socklen_t new_client_len;

    memset(&new_client, 0, sizeof(new_client));
    new_client_len = sizeof(new_client);

    if ((clientsocket = accept(socket, (struct sockaddr *) &new_client,
                               &new_client_len)) < 0) {
        return (-1);
    }

    strncpy(srcip, inet_ntoa(new_client.sin_addr), addrsize - 1);
    srcip[addrsize - 1] = '\0';

    return (clientsocket);
}

/* Receive a TCP packet (from an open socket) */
char *OS_RecvTCP(int socket, int sizet)
{
    char *ret;

    ret = (char *) calloc((sizet), sizeof(char));
    if (ret == NULL) 
        return (NULL);

    if (recv(socket, ret, sizet - 1, 0) <= 0) 
    {
        free(ret);
        return (NULL);
    }

    return (ret);
}


/* Send a TCP packet (through an open socket) */
int OS_SendTCP(int socket, const char *msg)
{
    if ((send(socket, msg, strlen(msg), 0)) <= 0) 
        return (OS_SOCKTERR);

    return (0);

}

/* Send a TCP packet of a specific size (through a open socket) */
int OS_SendTCPbySize(int socket, int size, const char *msg)
{
    if ((send(socket, msg, size, 0)) < size) 
        return (OS_SOCKTERR);
    
    return (0);
}

int OS_SetRecvTimeout(int socket, long seconds, long useconds)
{
    struct timeval tv = { seconds, useconds };
    return setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const void *)&tv, sizeof(tv));
}


int OS_CloseSocket(int socket)
{
    return (close(socket));
}


