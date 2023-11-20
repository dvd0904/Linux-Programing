#ifndef DEBUG_MESSAGES_H
#define DEBUG_MESSAGES_H

/* Connection manager */

#define SENSOR_CONNECTION_OPEN      "Client with ID '%d' has opened a new connection."
#define SENSOR_CONNECTION_CLOSE     "Client with ID '%d' has closed a new connection."

/* Data manager */

#define SENSOR_COLD                 "%s [WARNING]: The sensor node with ID '%d' reports it's cold (avg temperature = <%d>)."
#define SENSOR_HOT                  "%s [WARNING]: The sensor node with ID '%d' reports it's hot (avg temperature = <%d>)."
#define SENSOR_TEMP                 "%s [INFO]: The sensor node with ID '%d' (avg temperature = <%d>)."
#define SENSOR_INVALID              "%s Received sensor data with invalid sensor ID '%d'"

/* Storage manager */

#define DB_NEW_CONNECTION           "Connection to SQL server established."
#define DB_NEW_TABLE                "New table '%s' created."
#define DB_LOST_CONNECTION          "Connection to SQL server lost."
#define DB_UNABLE_CONNECTION        "Unable to connect to SQL server lost."


#endif /* DEBUG_MESSAGES_H */