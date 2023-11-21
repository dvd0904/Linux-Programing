#include "shared.h"
#include "cJSON.h"

void getMsg(char **msg, int senID, double temp)
{
    char *timestamp = NULL; 
    cJSON *message = cJSON_CreateObject();
    os_calloc(TIME_LENGTH, sizeof(char), timestamp);
    timestamp = get_timestamp(time(NULL));

    cJSON_AddStringToObject(message, "timestamp", timestamp);
    cJSON_AddNumberToObject(message, "sensorID", senID);
    cJSON_AddNumberToObject(message, "temperature", temp);

    *msg = cJSON_PrintUnformatted(message);
    cJSON_Delete(message);
    os_free(timestamp);

}

int *read_room()
{
    int *senIDs = (int *)malloc(sizeof(int) * 5);
    const char *path = "room.json";
    cJSON *root = NULL, *room_obj = NULL;

    if (root = json_fread(path, 0), !root) 
    {
        if (errno) 
            mdebug("Couldn't load configuration file due to: %s (%d)", strerror(errno), errno);
        else 
            mdebug("Couldn't load configuration file. Maybe format is invalid.");
    }

    room_obj = cJSON_GetObjectItem(root, "roomIDs");
    if(!cJSON_IsArray(room_obj))
    {
        cJSON_Delete(room_obj);
        return NULL;
    }

    for(int i = 0; i < cJSON_GetArraySize(room_obj); i++)
    {
        cJSON *subRID = cJSON_GetArrayItem(room_obj, i);
        if(cJSON_IsObject(subRID))
        {
            cJSON *subSID = cJSON_GetObjectItem(subRID, "sensorID");
            if(cJSON_IsNumber(subSID))
                senIDs[i] = subSID->valueint;
            else 
                cJSON_Delete(subSID);
        }   
        else 
            cJSON_Delete(subRID);
    }

    cJSON_Delete(root);

    return senIDs;
}


msg_t *msg_parse(char *msg)
{
    int *senIDs = read_room();
    msg_t *msg_parsed = (msg_t *)malloc(sizeof(msg_t));
    cJSON *msg_root = cJSON_Parse(msg);
    if(!cJSON_IsObject(msg_root))
        return NULL;

    cJSON *senID = cJSON_GetObjectItem(msg_root, "sensorID");
    if(cJSON_IsNumber(senID))
    {
        if(search(senIDs, senID->valueint, 0, 5))
        {
            msg_parsed->senID = senID->valueint;

            cJSON *ts = cJSON_GetObjectItem(msg_root, "timestamp");
            if(cJSON_IsString(ts))
                strcpy(msg_parsed->ts, ts->valuestring);
            else
            { 
                cJSON_Delete(ts);
                return NULL;
            }

            cJSON *temp = cJSON_GetObjectItem(msg_root, "temperature");
            if(cJSON_IsNumber(temp))
                msg_parsed->temp = temp->valueint;
            else 
            {
                cJSON_Delete(temp);
                return NULL;
            }
        }
        else 
        {
            return NULL;
            cJSON_Delete(senID);
        }
    }

    os_free(senIDs);
    cJSON_Delete(msg_root);

    return msg_parsed;
}