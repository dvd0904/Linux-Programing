
#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H


/* SYSTEM ERRORS */
#define FORK_ERROR    "(1101): Could not fork due to [(%d)-(%s)]."
#define MEM_ERROR     "(1102): Could not acquire memory due to [(%d)-(%s)]."
#define FOPEN_ERROR   "(1103): Could not open file '%s' due to [(%d)-(%s)]."
#define FWRITE_ERROR  "(1104): Could not write file '%s' due to [(%d)-(%s)]."
#define FREAD_ERROR   "(1105): Could not read from file '%s' due to [(%d)-(%s)]."
#define MUTEX_ERROR   "(1106): Unable to set pthread mutex."
#define WAITPID_ERROR "(1107): Error during waitpid()-call due to [(%d)-(%s)]."
#define THREAD_ERROR  "(1108): Unable to create new pthread."

/* COMMON ERRORS */
#define PORT_ERROR      "(1201): Invalid port number: '%d'."
#define BIND_ERROR      "(1202): Unable to Bind port '%d' due to [(%d)-(%s)]."
#define ACCEPT_ERROR    "(1203): Couldn't accept TCP connections: %s (%d)."
#define SIGNAL_RECV     "(1204): SIGNAL [(%d)-(%s)] Received. Exit Cleaning..."

#endif