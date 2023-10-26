#include "shared.h"
#include "cJSON.h"

void getMsg(char **msg, int senID, double temp)
{
    char *timestamp = NULL; 
    cJSON *message = cJSON_CreateObject();
    os_calloc(TIME_LENGTH, sizeof(char), timestamp);
    get_timestamp(time(NULL), timestamp);

    cJSON_AddStringToObject(message, "timestamp", timestamp);
    cJSON_AddNumberToObject(message, "sensorID", senID);
    cJSON_AddNumberToObject(message, "temperature", temp);

    *msg = cJSON_PrintUnformatted(message);
    cJSON_Delete(message);
    os_free(timestamp);

}