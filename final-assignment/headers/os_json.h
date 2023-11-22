#ifndef JSON_FILE_H
#define JSON_FILE_H

#define JSON_MAX_FSIZE 536870912

#include "cJSON/cJSON.h"

/**
 * @brief Read a json file
 * @param path path to json file
 * @param retry try to read json file after clear comment
 * @return an object of cJSON
*/
cJSON * json_fread(const char * path, char retry);

/**
 * @brief Wrtie to a json file
 * @param path path to json file
 * @param item an object of cJSON
 * @return 0 if success otherwise -1
*/
int json_fwrite(const char * path, const cJSON * item);

/**
 * @brief Clear comment in json file
 * @param json buffer in json file
*/
void json_strip(char * json);


#endif
