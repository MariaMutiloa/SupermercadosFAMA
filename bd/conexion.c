#include "./sqlite/sqlite3.h"
#include "conexion.h"
#include <stdio>

sqlite3 *db;

int startConn(){
	int rc = sqlite3_open ("./fama.db", &db);
	if(rc != SQLITE_OK){
		fprintf(stderr, "Cannot open database %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return 0;
	}

	return 1;
}