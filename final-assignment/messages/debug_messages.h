#ifndef DEBUG_MESSAGES_H
#define DEBUG_MESSAGES_H

/* Connection manager */

#define SENSOR_CONNECTION_OPEN      "A sensor with ID '%d' has opened a new connection."
#define SENSOR_CONNECTION_CLOSE     "A sensor with ID '%d' has closed a new connection."

/* Data manager */

#define SENSOR_TOO_COLD             "The sensor node with ID '%d' reports it's too cold (running temperature = <%d>)."
#define SENSOR_TOO_HOT              "The sensor node with ID '%d' reports it's too cold (running temperature = <%d>)."
#define SENSOR_INVALID              "Received sensor data with invalid sensor ID '%d'"

/* Storage manager */

#define DB_NEW_CONNECTION           "Connection to SQL server established."
#define DB_NEW_TABLE                "New table '%s' created."
#define DB_LOST_CONNECTION          "Connection to SQL server lost."
#define DB_UNABLE_CONNECTION        "Unable to connect to SQL server lost."


#endif /* DEBUG_MESSAGES_H */