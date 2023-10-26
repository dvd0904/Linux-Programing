
#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H


/* SYSTEM ERRORS */
#define FORK_ERROR          "(1101): Could not fork due to [(%d)-(%s)]."
#define MEM_ERROR           "(1102): Could not acquire memory due to [(%d)-(%s)]."
#define FOPEN_ERROR         "(1103): Could not open file due to [(%d)-(%s)]."
#define FWRITE_ERROR        "(1104): Could not write file due to [(%d)-(%s)]."
#define FREAD_ERROR         "(1105): Could not read from file due to [(%d)-(%s)]."
#define FSEEK_ERROR         "(1115): Could not set position in file due to [(%d)-(%s)]."
#define FCLOSE_ERROR        "(1105): Could not close file due to [(%d)-(%s)]."
#define MUTEX_ERROR         "(1106): Unable to set pthread mutex."
#define WAITPID_ERROR       "(1107): Error during waitpid()-call due to [(%d)-(%s)]."
#define THREAD_ERROR        "(1108): Unable to create new pthread."
#define SIG_ERROR           "(1109): Unable to catch signal."
#define PIPE_ERROR          "(1110): Could not create pipe due to [(%d)-(%s)]."
#define READ_PIPE_ERROR     "(1111): Error reading from pipe due to [(%d)-(%s)]."
#define SELECT_ERROR        "(1112): Error slecect due to [(%d)-(%s)]."
#define RECV_ERROR          "(1114): Error recv due to [(%d)-(%s)]."

/* COMMON ERRORS */
#define PORT_ERROR      "(1201): Invalid port number: '%d'."
#define BIND_ERROR      "(1202): Unable to Bind port '%d' due to [(%d)-(%s)]."
#define ACCEPT_ERROR    "(1203): Couldn't accept TCP connections: %s (%d)."
#define CONN_ERROR      "(1204): Unable to connect to server at '%s:%d': %s (%d)."
#define SEND_ERROR      "(1204): Unable to send message to server due to [(%d)-(%s)].."
#define SIGNAL_RECV     "(1206): SIGNAL [(%d)-(%s)] Received. Exit Cleaning..."

#endif