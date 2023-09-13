#ifndef OSNET_H
#define OSNET_H


typedef struct tcpsock
{
    int sd;
    char *ip;
    unsigned short port;
} tcpsock_t;


typedef struct tcpsock_el 
{
    tcpsock_t *sock_ptr;
    int sd;
    sensor_id_t sensor;
    char * last_active;
} tcpsock_el_t;

/* OS_Bindport*
 * Bind a specific port (protocol and a ip).
 * If the IP is not set, it is going to use ADDR_ANY
 * Return the socket.
 */
int OS_Bindporttcp(u_int16_t _port, const char *_ip);

/* OS_Connect
 * Connect to a TCP socket
 */
int OS_ConnectTCP(u_int16_t _port, const char *_ip);


/* OS_RecvTCP
 * Receive a TCP packet
 */
int OS_AcceptTCP(tcpsock_t *server_sock, tcpsock_t **client, size_t addrsize);
char *OS_RecvTCP(int socket, int sizet);
int OS_RecvTCPBuffer(int socket, char *buffer, int sizet) __attribute__((nonnull));

/* OS_SendTCP
 * Send a TCP packet (in a open socket)
 */
int OS_SendTCP(int socket, const char *msg) __attribute__((nonnull));
int OS_SendTCPbySize(int socket, int size, const char *msg) __attribute__((nonnull));

/* Set the maximum buffer size for the socket */
int OS_SetSocketSize(int sock, int mode, int max_msg_size);

/* Set the receiving timeout for a socket
 * Returns 0 on success, else -1
 */
int OS_SetRecvTimeout(int socket, long seconds, long useconds);

/* Close a network socket
 * Returns 0 on success, else -1 or SOCKET_ERROR
 */
int OS_CloseSocket(int socket);

int OS_GetSocketSd(int *socket, int *poll_fd);

#endif