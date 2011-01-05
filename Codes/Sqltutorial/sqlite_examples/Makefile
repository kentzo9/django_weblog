# Version
VERSION = 0.0.1

# Compiler flags
MSQLFLAGS = -Wall -W -O2 -s -pipe -lpthread -L/usr/local/lib/mysql -I/usr/local/include/mysql      -lmysqlclient
DLLFLAGS = -Wall -W -O2 -s -pipe -lpthread -L/usr/local/lib/mysql -I/usr/local/include/mysql     -lmysqlclient      -ldl
SL3FLAGS = -Wall -W -O2 -s -pipe -Wl,-R/usr/local/lib -lsqlite3 -I/usr/local/include -L/usr/local/lib
SL3MFLAG = -Wall -W -O2 -s -pipe -Wl,-R/usr/local/lib -lsqlite3 -lm
SLFLAGS = -Wall -W -O2 -s -pipe -Wl,-R/usr/local/lib -lsqlite -I/usr/local/include -L/usr/local/lib
CFLAGS = -Wall -W -O2 -s -pipe 
sqliteLIB := $(shell ls /usr/local/lib/libsqlite.so)
sqlite3LIB := $(shell ls /usr/local/lib/libsqlite3.so)
# all assumes sqlite and sqlite3 are installed  
#
all: apiSQLite3 myfuncpp simplesqlite3cpp simplesqlite3 sqlite3apilist eatblob apiSQLite3b



# If only sqlite is installed: make sqlite
sqlite: apiSQLite

# If only sqlite3 is installed:  make sqlite3
sqlite3: apiSQLite3 apiSQLite3b eatblob myfuncpp simplesqlite3cpp simplesqlite3 

apiSQLite3: apiSQLite3.c
	gcc -o $@ $(SL3FLAGS)  $<





apiSQLite3b: apiSQLite3b.c
	gcc -o $@ $(SL3FLAGS)  $<



apiSQLite: apiSQLite.c
	gcc -o $@ $(SLFLAGS)  $<



eatblob: eatblob.c
	gcc -o $@ $(SL3FLAGS)  $<



myfuncpp: myfuncpp.cc
	g++ -o $@ $(SL3FLAGS)  $<



simplesqlite3cpp: simplesqlite3cpp.cc
	g++ -o $@ $(SL3FLAGS)  $<



simplesqlite3: simplesqlite3.c
	gcc -o $@ $(SL3FLAGS)  $<



simplesqlite3cpp2: simplesqlite3cpp2.cc
	g++ -o $@ $(SL3FLAGS)  $<




sqlite3apilist: sqlite3apilist.cc
	g++ -o $@ $(SL3FLAGS)  $<





test:
	./apiSQLite test.db "create table test (a INTEGER PRIMARY KEY, b int)"
	./apiSQLite test.db "insert into test (b) values (1)"
	./apiSQLite test.db "insert into test (b) values (2)"
	./apiSQLite test.db "insert into test (b) values (3)"
	./apiSQLite test.db "select * from test"


test3:
	./apiSQLite3b test3.db < script0



clean:	
	rm -f apiSQLite3 apiSQLite3b apiSQLite test3.db test.db eatblob myfuncpp simplesqlite3cpp simplesqlite3 simplesqlite3cpp2 sqlite3api_list


