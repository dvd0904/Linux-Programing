#ifndef OSNET_H
#define OSNET_H

/* OS_Bindport*
 * Bind a specific port (protocol and a ip).
 * If the IP is not set, it is going to use ADDR_ANY
 * Return the socket.
 */
int OS_Bindport(u_int16_t _port, const char *_ip);

/* OS_Connect
 * Connect to a TCP socket
 */
int OS_Connect(u_int16_t _port, const char *_ip);

/* Accept a TCP connection */
int OS_AcceptTCP(int socket, char *srcip, size_t addrsize);

/* OS_RecvTCP
 * Receive a TCP packet
 */
char *OS_RecvTCP(int socket, int sizet);

/* OS_SendTCP
 * Send a TCP packet (in a open socket)
 */
int OS_SendTCP(int socket, const char *msg) __attribute__((nonnull));


/* Close a network socket
 * Returns 0 on success, else -1 or SOCKET_ERROR
 */
int OS_CloseSocket(int socket);


#endif