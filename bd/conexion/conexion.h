#ifndef CONEXION_H
#define CONEXION_H
#include "../sqlite3.h"
sqlite3* startConn();
int closeConn(sqlite3* db);

#endif