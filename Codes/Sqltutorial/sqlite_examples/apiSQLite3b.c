/* Copyright (GPL) mmc Mike Chirico mchirico@users.sourceforge.net
http://prdownloads.sourceforge.net/cpearls/sqlite_examples.tar.gz?download

 Compile:
  gcc -o apiSQLite3b apiSQLite3b.c -lsqlite3

 Usage:

      $ ./apiSQLite3b test3.db "create table mytest (a INTEGER PRIMARY KEY, b INT, c FLOAT, e TEXT)"
      $ ./apiSQLite3b test3.db "insert into  mytest (b,c,e) values (1,1.0,'first value')"
      $ ./apiSQLite3b test3.db "insert into  mytest (b,c,e) values (2,2.0,'second value')"
      $ ./apiSQLite3b test3.db "select * from   mytest"

*/

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sqlite3.h>
#define STMLEN 1024

//sqlite3_column_count(sqlite3_stmt *pStmt);
int print_col(sqlite3_stmt * pTableInfo, int col)
{

	switch (sqlite3_column_type(pTableInfo, col)) {
	case SQLITE_INTEGER:
		printf("%d ", sqlite3_column_int(pTableInfo, col));
		break;
	case SQLITE_FLOAT:
		printf("%f ", sqlite3_column_double(pTableInfo, col));
		break;
	case SQLITE_TEXT:
		printf("%s ", sqlite3_column_text(pTableInfo, col));
		break;
	case SQLITE_BLOB:	//printf("%s",sqlite3_column_blob(pTableInfo, col));
		break;
	case SQLITE_NULL:
		printf("Null ");
		break;
	default:
		printf(" *Cannot determine SQLITE TYPE* col=%d ", col);
	}

	return 0;
}

int main(int argc, char **argv)
{
	sqlite3 *db;
	sqlite3_stmt *plineInfo = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	//  char *zErrMsg = 0;
	int rc, i;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s DATABASE < SQL-STATEMENT(S)\n",
			argv[0]);
		exit(1);
	}

	if (sqlite3_open(argv[1], &db) != SQLITE_OK) {
		fprintf(stderr, "Can't open database: \n");
		sqlite3_close(db);
		exit(1);
	}







	while ((read = getline(&line, &len, stdin)) != -1) {

		rc = sqlite3_prepare(db, line, -1, &plineInfo, 0);
		if (rc == SQLITE_OK && plineInfo != NULL ) {
			while ((rc =
				sqlite3_step(plineInfo)) == SQLITE_ROW) {
				//
				for (i = 0;
				     i < sqlite3_column_count(plineInfo);
				     ++i)
					print_col(plineInfo, i);

				printf("\n");

			}
			rc = sqlite3_finalize(plineInfo);
		}


		fprintf(stderr, "Total Changes %d\n",
			sqlite3_total_changes(db));
	}
	if (line) {
		free(line);
	}
	sqlite3_close(db);
	return 0;
}
