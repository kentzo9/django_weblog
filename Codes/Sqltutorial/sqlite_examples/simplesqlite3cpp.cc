/* CopyRight GPL (c) Mike Chirico mchirico@users.sourceforge.net 
                     or mchirico@comcast.net


  Very simple C++ program.

   Compile:
    g++ -o simplesqlitecpp3 simplesqlite3cpp.cc  -Wall -W -O2 -Wl,-R/usr/local/lib -lsqlite3

   Note sqlite3 shared library, by default, installs in /usr/local/lib. 
   The compile command above will directly link the full path of 
   this library into this program.


*/
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <sqlite3.h>

class SQLITE3 {
private:
  sqlite3 *db;
  char *zErrMsg;
  int rc;
public:
  SQLITE3(std::string tablename="init.db"): zErrMsg(0) {
    rc = sqlite3_open(tablename.c_str(), &db);
    if( rc ){
     fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
     sqlite3_close(db);
     exit(1);
    }

  }
  static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    NotUsed=0;
    int i;
    for(i=0; i<argc; i++){
     printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
     }
    printf("\n");
    return 0;
   }
  int exe(std::string s_exe) {
     rc = sqlite3_exec(db, s_exe.c_str(),SQLITE3::callback, 0, &zErrMsg);
     if( rc!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
     }
     return rc;
    }
  ~SQLITE3(){
      sqlite3_close(db);
  } 
};

int main(int argc, char** argv)
{

  if( argc!=3 ){
   std::cerr << "Usage: " << argv[0] << " DATABASE SQL-STATEMENT" << std::endl;
   exit(1);
  }
  SQLITE3 sql(argv[1]);
  sql.exe(argv[2]);
}

