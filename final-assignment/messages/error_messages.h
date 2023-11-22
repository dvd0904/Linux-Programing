
#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H


#define FORK_ERROR          "Could not fork due to [(%d)-(%s)]."
#define MEM_ERROR           "Could not acquire memory due to [(%d)-(%s)]."
#define OPEN_ERROR          "Could not open file due to [(%d)-(%s)]."
#define WRITE_ERROR         "Could not write file due to [(%d)-(%s)]."
#define READ_ERROR          "Could not read from file due to [(%d)-(%s)]."
#define SEEK_ERROR          "Could not set position in file due to [(%d)-(%s)]."
#define CLOSE_ERROR         "Could not close file due to [(%d)-(%s)]."
#define MUTEX_ERROR         "Unable to set pthread mutex."
#define WAITPID_ERROR       "Error during waitpid()-call due to [(%d)-(%s)]."
#define THREAD_ERROR        "Unable to create new pthread."
#define SIG_ERROR           "Unable to catch signal."
#define PIPE_ERROR          "Could not create pipe due to [(%d)-(%s)]."
#define SELECT_ERROR        "Error slecect due to [(%d)-(%s)]."
#define RECV_ERROR          "Error recv due to [(%d)-(%s)]."
#define CHMOD_ERROR         "Could not chmod object '%s' due to [(%d)-(%s)]."

#define PORT_ERROR          "Invalid port number: '%d'."
#define BIND_ERROR          "Unable to Bind port '%d' due to [(%d)-(%s)]."
#define ACCEPT_ERROR        "Couldn't accept TCP connections: %s (%d)."
#define CONN_ERROR          "Unable to connect to server at '%s:%d': %s (%d)."
#define SEND_ERROR          "Unable to send message to server due to [(%d)-(%s)].."
#define SIGNAL_RECV         "SIGNAL [(%d)-(%s)] Received."

#endif