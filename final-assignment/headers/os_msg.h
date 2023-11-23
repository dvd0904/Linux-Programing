#ifndef MSG_H
#define MSG_H


typedef struct msg_t
{
    char ts[64];
    double temp;
    int senID;
} msg_t;

/**
 * @brief Construct a message
 * @param msg message
 * @param senID sensor ID
 * @param temp temperature value
*/
void getMsg(char **msg, int senID, double temp);

/**
 * @brief Read a json file room
 * @return an array of all sensor ID in all room
*/
int *read_room();

/**
 * @brief Parse a message from sensor node
 * @param msg message to parse
 * @param senIDs an array of sensor ID
 * @return a struct contain all information of message
*/
// msg_t *msg_parse(char *msg, int *senIDs);
int msg_parse(msg_t **msg_parsed, char *msg, int *senIDs);

#endif