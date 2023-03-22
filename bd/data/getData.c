#include "./sqlite/sqlite3.h"
#include "conexion.h"
#include <stdio.h>
#include "getData.h"
#include <string.h>
#include <stdlib.h>
#include "struct.h"

char * getCliente (char * dni){
    int rc;
    char *err_msg = 0;
    sqlite3_stmt *res;
    char * nombre;
    nombre = (char *) malloc (15* sizeof(char));

    char * sql = "SELECT *FROM CLIENTE WHERE " dni = ?"";

    rc = sqlite3_prepare_v2(db, sql, -1, &res,0);

    if(rc == SQLITE_OK){
        sqlite3_bind_text(res, 1, dni, sizeof(*dni), SQLITE_STATIC);
    }
    else{
        fprintf(stderr,"Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }

    int step = sqlite3_step(res);

    if(step == SQLITE_ROW){
        strcpy(nombre, (const char *) sqlite3_column_text(res,0));
    }

    else{
        printf("No results foudn")
    }
    return nombre;
}

char * getAdministrador (char * dni){
    int rc;
    char *err_msg = 0;
    sqlite3_stmt *res;
    char * nombre;
    nombre = (char *) malloc (15* sizeof(char));

    char * sql = "SELECT *FROM ADMINISTRADOR WHERE " dni = ?"";

    rc = sqlite3_prepare_v2(db, sql, -1, &res,0);

    if(rc == SQLITE_OK){
        sqlite3_bind_text(res, 1, dni, sizeof(*dni), SQLITE_STATIC);
    }
    else{
        fprintf(stderr,"Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }

    int step = sqlite3_step(res);

    if(step == SQLITE_ROW){
        strcpy(nombre, (const char *) sqlite3_column_text(res,0));
    }

    else{
        printf("No results foudn")
    }
    return nombre;
}