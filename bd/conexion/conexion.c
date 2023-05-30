#include "../sqlite3.h"
#include "conexion.h"
#include <stdio.h>

sqlite3* startConn(){
    sqlite3* db;
    int rc = sqlite3_open("../bd/fama.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

int closeConn(sqlite3* db) {
    return sqlite3_close_v2(db);
}
}
