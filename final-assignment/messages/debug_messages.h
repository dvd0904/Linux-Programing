#ifndef DEBUG_MESSAGES_H
#define DEBUG_MESSAGES_H

/* Connection manager */

#define SENSOR_CONNECTION_OPEN      "Client with ID '%d' has opened a new connection."
#define SENSOR_CONNECTION_CLOSE     "Client with ID '%d' has closed a new connection."

/* Data manager */

#define SENSOR_COLD                 "%s [WARNING]: The sensor node with ID '%d' reports it's cold (avg temperature = <%d>)."
#define SENSOR_HOT                  "%s [WARNING]: The sensor node with ID '%d' reports it's hot (avg temperature = <%d>)."
#define SENSOR_TEMP                 "%s [INFO]: The sensor node with ID '%d' (avg temperature = <%d>)."
#define SENSOR_INVALID              "%s [ERROR]: Received sensor data with invalid sensor ID '%d'"

/* Storage manager */

#define DB_CREATE                   "Database '%s' is created."
#define DB_INSERT                   "New data is push into Database."
#define DB_NEW_CONNECTION           "Connection to SQL server established."
#define DB_DISCONNECT               "Database disconnected."
#define DB_UNABLE_CONNECTION        "Unable to connect to SQL server."

#endif /* DEBUG_MESSAGES_H */