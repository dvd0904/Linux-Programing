#ifndef DEBUG_MESSAGES_H
#define DEBUG_MESSAGES_H

/* Connection manager */

#define SENSOR_CONNECTION_OPEN      "(1000): A sensor with ID '%d' has opened a new connection."
#define SENSOR_CONNECTION_CLOSE     "(1001): A sensor with ID '%d' has closed a new connection."

/* Data manager */

#define SENSOR_TOO_COLD             "(1002): The sensor node with ID '%d' reports it's too cold (running temperature = <%d>)."
#define SENSOR_TOO_HOT              "(1002): The sensor node with ID '%d' reports it's too cold (running temperature = <%d>)."
#define SENSOR_INVALID              "(1003): Received sensor data with invalid sensor ID '%d'"

/* Storage manager */

#define DB_NEW_CONNECTION           "(1004): Connection to SQL server established."
#define DB_NEW_TABLE                "(1005): New table '%s' created."
#define DB_LOST_CONNECTION          "(100): Connection to SQL server lost."
#define DB_UNABLE_CONNECTION        "(100): Unable to connect to SQL server lost."


#endif /* DEBUG_MESSAGES_H */