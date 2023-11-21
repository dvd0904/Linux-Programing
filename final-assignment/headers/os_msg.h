#ifndef MSG_H
#define MSG_H


typedef struct msg_t
{
    int senID;
    int temp;
    char ts[64];
} msg_t;

void getMsg(char **msg, int senID, double temp);

int *read_room();

msg_t *msg_parse(char *msg);

#endif