
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

fdb_t *fdb_init(sqlite3 *db);

void fdb_destroy(fdb_t *db);

int fdb_create_file(const char *path, const char *source);

int fdb_create_sensor_db();

int fdb_stmt_cache(fdb_t *fdb, int index);

int fdb_data_insert(fdb_t *fdb, int SID, int temp, const char *ts);

fdb_t *fdb_open_sensor_db();