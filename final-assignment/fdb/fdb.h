
#include "shared.h"
#include <sqlite3.h>

typedef enum fdb_stmt 
{
    FDB_STMT_DATA_INSERT,
    FDB_STMT_DATA_GET_SID,
    FDB_STMT_DATA_GET_TEMP_ASC,
    FDB_STMT_SIZE
} fdb_stmt;

typedef struct fdb_t
{
    sqlite3 *db;
    sqlite3_stmt *stmt[FDB_STMT_SIZE];
} fdb_t;

extern char *schema_sensor_sql;

/**
 * @brief Init a database
 * @param db database connection object
 * @return a database struct
 */
fdb_t *fdb_init(sqlite3 *db);

/**
 * @brief Free a database
 * @param fdb database structure
 */
void fdb_destroy(fdb_t *fdb);

/**
 * @brief Create a database file
 * @return 0 if success otherwise -1
 */
int fdb_create_sensor_db();

/**
 * @brief Establish connection to databse
 * @return database structure
 */
fdb_t *fdb_open_sensor_db();

/**
 * @brief Reset sqlite3 statement
 * @param fdb database structure
 * @param index index of sqlite3 statement
 * @return 0 if success otherwise -1
 */
int fdb_stmt_cache(fdb_t *fdb, int index);

/**
 * @brief Insert data to database
 * @param fdb database structure
 * @param SID sensor ID
 * @param temp temperature
 * @param ts timestamp
 * @return 0 if success otherwise -1
 */
int fdb_data_insert(fdb_t *fdb, int SID, int temp, const char *ts);
