#ifndef ASS_NET_H
#define ASS_NET_H


#define DEFAULT_PORT 1515

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
int OS_AcceptTCP(int socket, char *srcip, size_t addrsize) __attribute__((nonnull));
char *OS_RecvTCP(int socket, int sizet);
int OS_RecvTCPBuffer(int socket, char *buffer, int sizet) __attribute__((nonnull));

/* OS_SendTCP
 * Send a TCP packet (in a open socket)
 */
int OS_SendTCP(int socket, const char *msg) __attribute__((nonnull));
int OS_SendTCPbySize(int socket, int size, const char *msg) __attribute__((nonnull));

/* Set the maximum buffer size for the socket */
int OS_SetSocketSize(int sock, int mode, int max_msg_size);


/* Close a network socket
 * Returns 0 on success, else -1 or SOCKET_ERROR
 */
int OS_CloseSocket(int socket);


#endif