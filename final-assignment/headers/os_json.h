#ifndef JSON_FILE_H
#define JSON_FILE_H

#define JSON_MAX_FSIZE 536870912

#include "cJSON/cJSON.h"

cJSON * json_fread(const char * path, char retry);
int json_fwrite(const char * path, const cJSON * item);

// Clear C/C++ style comments from a JSON string
void json_strip(char * json);


#endif
