/*
  Very simple C program.

   Compile:
     gcc -o simplesqlite3 simplesqlite3.c  -Wall -W -O2 -Wl,-R/usr/local/lib -lsqlite3
  
   Note sqlite3 shared library, by default, installs in /usr/local/lib. 
   The compile command above will directly link the full path of 
   this library into this program.

*/



#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>



int main(int argc, char **argv){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc,i;

  char **result;
  int nrow;
  int ncol;

  if( argc!=3 ){
    fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
    exit(1);
  }
  rc = sqlite3_open(argv[1], &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(1);
  }
  //rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);

  rc = sqlite3_get_table(
			db,              /* An open database */
			"select * from stuff",       /* SQL to be executed */
			&result,       /* Result written to a char *[]  that this points to */
			&nrow,             /* Number of result rows written here */
			&ncol,          /* Number of result columns written here */
			&zErrMsg          /* Error msg written here */
			);

  printf("nrow=%d ncol=%d\n",nrow,ncol);
  for(i=0 ; i < nrow+ncol; ++i)
      printf("%s ",result[i]);



  sqlite3_free_table(result);
  






  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
  }
  sqlite3_close(db);
  return 0;
}

