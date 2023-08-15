#ifndef DEBUG_MESSAGES_H
#define DEBUG_MESSAGES_H

/* Connection manager */

#define SENSOR_CONNECTION_OPEN      "(1001): A sensor with ID '%d' has opened a new connection."
#define SENSOR_CONNECTION_CLOSE     "(1002): A sensor with ID '%d' has closed a new connection."

/* Data manager */

#define SENSOR_TOO_COLD             "(1003): The sensor node with ID '%d' reports it's too cold (running temperature = <%d>)."
#define SENSOR_TOO_HOT              "(1004): The sensor node with ID '%d' reports it's too cold (running temperature = <%d>)."
#define SENSOR_INVALID              "(1005): Received sensor data with invalid sensor ID '%d'"

/* Storage manager */

#define DB_NEW_CONNECTION           "(1006): Connection to SQL server established."
#define DB_NEW_TABLE                "(1007): New table '%s' created."
#define DB_LOST_CONNECTION          "(1008): Connection to SQL server lost."
#define DB_UNABLE_CONNECTION        "(1009): Unable to connect to SQL server lost."


#endif /* DEBUG_MESSAGES_H */