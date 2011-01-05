/*

  gcc -o apiSQLite3b apiSQLite3b.c -lsqlite3

*/


#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>


static int callback(void *NotUsed, int argc, char **argv, char **azColName){
      int i;
      int rowpr=argc-1;
      NotUsed=0;

      for(i=0; i<rowpr; i++)
	printf("%s ",azColName[i]);

      printf("%s\n",azColName[rowpr]);

      

      for(i=0; i<rowpr; i++){
	//	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	printf("%s ",  argv[i] ? argv[i] : "NULL");
        
      }
	printf("%s\n",  argv[rowpr] ? argv[rowpr] : "NULL");

      return 0;
}

int main(int argc, char **argv){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  if( argc!=3 ){
    fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
    exit(1);
  }
  if (sqlite3_open(argv[1], &db) != SQLITE_OK)
    {
    fprintf(stderr, "Can't open database: \n");
    sqlite3_close(db);
    exit(1);
  }
  rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
  }
  sqlite3_close(db);
  return 0;
}

