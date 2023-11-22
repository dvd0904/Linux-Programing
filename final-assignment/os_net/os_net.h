#ifndef OS_NET_H
#define OS_NET_H

/**
 * @brief Bind a specific port.
 * @param _port port to bind
 * @return socket descriptor
 */
int OS_BindPort(u_int16_t _port);

/**
 * @brief Connect to a TCP socket.
 * @param _port port to connect
 * @param _ip ip address to connect
 * @return socket descriptor
 */
int OS_Connect(u_int16_t _port, const char *_ip);

/**
 * @brief Accept a TCP connection.
 * @param socket socket file descriptor
 * @param srcip ip address of incomming connection
 * @param addrsize address size
 * @return socket descriptor of an incomming connection
 */
int OS_AcceptTCP(int socket, char *srcip, size_t addrsize);

/**
 * @brief Receive a TCP packet.
 * @param socket socket file descriptor
 * @param sizet number of bytes to read
 * @return a string represent for packet content
 */
char *OS_RecvTCP(int socket, int sizet);

/**
 * @brief Send a TCP packet.
 * @param socket socket file descriptor
 * @param msg message to send
 * @return 0 if success otherwise -1
 */
int OS_SendTCP(int socket, const char *msg);

/**
 * @brief Close a network socket.
 * @param socket socket file descriptor
 * @return 0 if success otherwise -1 or SOCKET_ERROR
 */
int OS_CloseSocket(int socket);


#endif