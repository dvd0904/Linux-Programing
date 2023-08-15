#include <errno.h>
#include "shared.h"
#include "ass_net.h"



#define RECV_SOCK 0
#define SEND_SOCK 1

/* Prototypes */
static int OS_Bindport(u_int16_t _port, unsigned int _proto, const char *_ip);
static int OS_Connect(u_int16_t _port, unsigned int protocol, const char *_ip);

static int OS_Bindport(u_int16_t _port, unsigned int _proto, const char *_ip)
{
    int os_sock;
    struct sockaddr_in server;

    if (_proto == IPPROTO_TCP) 
    {
        int flag = 1;
        if ((os_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
            return (int)(OS_SOCKTERR);
        

        if (setsockopt(os_sock, SOL_SOCKET, SO_REUSEADDR,
                       (char *)&flag,  sizeof(flag)) < 0) 
        {
            OS_CloseSocket(os_sock);
            return (OS_SOCKTERR);
        }
    } 
    else 
        return (OS_INVALID);

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

    if (_proto == IPPROTO_TCP) 
    {
        if (listen(os_sock, BACKLOG) < 0) 
        {
            OS_CloseSocket(os_sock);
            return (OS_SOCKTERR);
        }
    }

    return (os_sock);
}

/* OS_Bindport*
 * Bind a specific port (protocol and a ip).
 * If the IP is not set, it is going to use ADDR_ANY
 * Return the socket.
 */
int OS_Bindporttcp(u_int16_t _port, const char *_ip)
{
    return (OS_Bindport(_port, IPPROTO_TCP, _ip));
}

/* Open a TCP/UDP client socket */
static int OS_Connect(u_int16_t _port, unsigned int protocol, const char *_ip)
{
    int ossock;
    int max_msg_size = OS_MAXSTR + 512;
    struct sockaddr_in server;


    if ((ossock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) 
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

/* Open a TCP socket */
int OS_ConnectTCP(u_int16_t _port, const char *_ip)
{
    return (OS_Connect(_port, IPPROTO_TCP, _ip));
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
    struct sockaddr_in _nc;
    socklen_t _ncl;

    memset(&_nc, 0, sizeof(_nc));
    _ncl = sizeof(_nc);

    if ((clientsocket = accept(socket, (struct sockaddr *) &_nc,
                               &_ncl)) < 0) {
        return (-1);
    }

    strncpy(srcip, inet_ntoa(_nc.sin_addr), addrsize - 1);
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

/* Receive a TCP packet (from an open socket)
   Returns the number of bytes received,
   or -1 if an error occurred */
int OS_RecvTCPBuffer(int socket, char *buffer, int sizet)
{
    int retsize;

    if ((retsize = recv(socket, buffer, sizet - 1, 0)) > 0) {
        buffer[retsize] = '\0';
    }
    return (retsize);
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

int OS_CloseSocket(int socket)
{
    return (close(socket));
}