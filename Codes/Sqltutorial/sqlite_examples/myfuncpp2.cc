/*
  Very simple C++ program.

   Compile:
    g++ -o myfuncpp myfuncpp.cc  -Wall -W -O2 -Wl,-R/usr/local/lib -lsqlite3

   Note sqlite3 shared library, by default, installs in /usr/local/lib. 
   The compile command above will directly link the full path of 
   this library into this program.


*/
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>

#include <sqlite3.h>
#include <assert.h>



class mextract {
public:

    std::vector<std::string> indexv;
    mextract(std::string d=" \n")
    {
        delims=d;
    }

    void setdelims(std::string d)
    {
        delims=d;
    }
    void setterm(std::string t="")
    {
        terminator=t;
    }



    void strip(std::string s ) {
      v.clear();
        bidx=s.find_first_not_of(delims);
        while(bidx != std::string::npos) {
            eidx=s.find_first_of(delims,bidx);
            if (eidx == std::string::npos) {
                eidx = s.length();
            }

            v.push_back(s.substr(bidx,(eidx-bidx)));
            bidx=s.find_first_not_of(delims,eidx);
        }
	index();
    }

  std::string I(unsigned int i)
  {
    if ( i < indexv.size() && indexv.size() > 0  )
      return indexv[i];
    else
      if ( indexv.size() > 0 )
	return indexv[ indexv.size()-1];
      else
	return "";

  }

  std::string F()
  {
    std::string s="";
    std::string d="";
    for(std::vector<std::string>::iterator i = indexv.begin(); i < indexv.end(); ++i)
      {
	s=s + d + *i;
	d=",";
      }
    return s;
  }

  std::string NF()
  {
    std::string s="(";
    std::string d="";
    for(std::vector<std::string>::iterator i = indexv.begin(); i < indexv.end(); ++i)
      {
	s=s + d + *i;
	d=",";
      }
    s+=")";
    return s;
  }



private:
  void index()
  {
    int rcount=0;
    
    std::string t="";
    indexv.clear();
    
    for(std::vector<std::string>::iterator i = v.begin(); i < v.end(); ++i)
      {
      for(std::string::iterator j = i->begin(); j < i->end(); ++j)
	{
	while( *j == '(' && j < i->end() )
	  {
	   rcount++;
	   if ( rcount > 1)
	     t+=*j;
           j++;
          }	  
        while( *j != ')' && j < i->end() ) {
	  t+=*j;
	  j++;
	}
        while( *j == ')' && j < i->end() ) {
          if ( rcount > 1)
	    t+=*j;
	  rcount--;
	  j++;
	}

	if ( rcount <= 1)
	  {
	    indexv.push_back(t);
	    t="";
	  } else { t+=","; }
	}
      }
  }


    std::string::size_type bidx,eidx;
    std::string delims,terminator;

    std::vector<std::string> v;

};












static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  NotUsed=0;
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

/*
** Implementation of the sign() function
*/
static void msignFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  assert( argc==1 );
  switch( sqlite3_value_type(argv[0]) ){
    case SQLITE_INTEGER: {
      long long int iVal = sqlite3_value_int64(argv[0]);
      iVal = ( iVal > 0) ? 1 : ( iVal < 0 ) ? -1 : 0;
      sqlite3_result_int64(context, iVal);
      break;
    }
    case SQLITE_NULL: {
      sqlite3_result_null(context);
      break;
    }
    default: {
      double rVal = sqlite3_value_double(argv[0]);
      rVal = ( rVal > 0) ? 1 : ( rVal < 0 ) ? -1 : 0;
      sqlite3_result_double(context, rVal);
      break;
    }
  }
}




static void IFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  assert( argc==2 );
  char *buf=NULL;
  std::stringstream s;
  std::string ts;
  mextract e;
  e.setdelims(", ");
  s.str("");

  s << sqlite3_value_text(argv[0]);
  e.strip(s.str());

  s.str(e.I(sqlite3_value_int64(argv[1])));
  buf = (char *) malloc (sizeof(char)*(s.str().size()+2));
  if (buf == NULL)
    fprintf(stderr,"malloc error in SNFunc, buf\n");
  snprintf(buf,s.str().size()+1,"%s",s.str().c_str());
  sqlite3_result_text(context,buf,s.str().size()+1,free );

}

static void FFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  assert( argc==1 );
  char *buf=NULL;
  std::stringstream s;
  std::string ts;
  mextract e;
  e.setdelims(", ");
  s.str("");

  s << sqlite3_value_text(argv[0]);
  e.strip(s.str());

  s.str(e.F());
  buf = (char *) malloc (sizeof(char)*(s.str().size()+2));
  if (buf == NULL)
    fprintf(stderr,"malloc error in SNFunc, buf\n");
  snprintf(buf,s.str().size()+1,"%s",s.str().c_str());
  sqlite3_result_text(context,buf,s.str().size()+1,free );

}






static void SFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  std::stringstream s;
  std::string d;
  double sum=0;
  char *buf=NULL;
  int i;
  int tflag=0;

  s.str("");

  s << "(";    
  d="";
  for(i=0; i < argc; i++)
    {
  switch( sqlite3_value_type(argv[i]) ){
    case SQLITE_INTEGER: {
      sum+=(double) sqlite3_value_int64(argv[i]);
      s << d << sum;
      d=",";
      break;
    }
    case SQLITE_FLOAT: {
      sum+= sqlite3_value_double(argv[i]);
      s << d << sum;
      d=",";
      break;
    }
    case SQLITE_BLOB: {
      sum+= 0;
      s << d << sum;
      d=",";
      break;
    }
    case SQLITE_NULL: {
      s << d << "()";
      d=",";
      break;
    }
    default: {
      s << sqlite3_value_text(argv[i]);
      s << d <<  sum;
      d=",";
      tflag=1;
      break;
     }
  }
   

  if( 1 == 1)
    {

    sum=0;
    s << ")";
    mextract e;
    e.setdelims(", ");
    e.strip(s.str());
    

    std::stringstream ts;
    ts.str("");
    ts << e.F();
    e.setdelims("(),");
    e.strip(ts.str());
    s.str(e.F());
    ts.str("");
    double md;
    for(std::vector<std::string>::iterator ii=e.indexv.begin(); ii < e.indexv.end(); ++ii)
      {
        ts << *ii  ;
        ts >> md;
	sum+=md;
      }
   sqlite3_result_double(context,sum );
    }else {
      

   s << ")";
   buf = (char *) malloc (sizeof(char)*(s.str().size()+2));
   if (buf == NULL)
     fprintf(stderr,"malloc error in SNFunc, buf\n");
   snprintf(buf,s.str().size()+1,"%s",s.str().c_str());
   sqlite3_result_text(context,buf,s.str().size()+1,free );
    }
}
}

static void SNFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  std::stringstream s;
  char *buf=NULL;
  int i;

  s.str("");

  s << "(";    
  for(i=0; i < argc; i++)
    {
  switch( sqlite3_value_type(argv[i]) ){
    case SQLITE_INTEGER: {
      s << "(" << i+1 << "," << sqlite3_value_int64(argv[i]) << ")";
      break;
    }
    case SQLITE_NULL: {
      s << "()";
      break;
    }
    default: {
      s << "(" << i+1 << "," << sqlite3_value_double(argv[i]) << ")";
      break;
     }
    }

    }

  s << ")";
  buf = (char *) malloc (sizeof(char)*(s.str().size()+2));
  if (buf == NULL)
    fprintf(stderr,"malloc error in SNFunc, buf\n");
  snprintf(buf,s.str().size()+1,"%s",s.str().c_str());
  sqlite3_result_text(context,buf,s.str().size()+1,free );

}


static void TFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  std::stringstream s;
  std::string d;
  char *buf=NULL;
  long i;
  long j;
  double di;
  double dj;
  double dk;
  s.str("");

  if (argc == 1)
    {
      s << "(";
      d = "";
     j= (int) sqlite3_value_int64(argv[0]);
     for(i=0; i < j; ++i)
       {
	 s << d << i;
       d=",";
       }

    }


  if (argc == 2)
    {
      s << "(";
      d = "";
      if ( sqlite3_value_int64(argv[0]) < sqlite3_value_int64(argv[1]))
	{
                i=  sqlite3_value_int64(argv[0]);
                j=  sqlite3_value_int64(argv[1]);
	} else 	  {
                i=  sqlite3_value_int64(argv[1]);
                j=  sqlite3_value_int64(argv[0]);
	 }
     for( ; i <= j; ++i)
       {
       s << d << i;
       d=",";
       }

    }


  if (argc == 3)
    {
      s << "(";
      d = "";

      di=  sqlite3_value_double(argv[0]);
      dj=  sqlite3_value_double(argv[1]);
      dk=  sqlite3_value_double(argv[2]);       

      if ( di < dj && dk > 0 )
	for( ; di <= dj; di+=dk)
        {
         s << d << di;
         d=",";
        }

      if ( di > dj && dk < 0 )
	for( ; di >= dj; di+=dk)
        {
         s << d << di;
         d=",";
        }

      if (  dk == 0 )
        {
         s << di;
        }

    }
  


  s << ")";
  buf = (char *) malloc (sizeof(char)*(s.str().size()+2));
  if (buf == NULL)
    fprintf(stderr,"malloc error in SNFunc, buf\n");
  snprintf(buf,s.str().size()+1,"%s",s.str().c_str());
  sqlite3_result_text(context,buf,s.str().size()+1,free );

}








/*
** An instance of the following structure holds the context of a
** sum() or avg() aggregate computation.
*/
typedef struct SCtx SCtx;
struct SCtx {
  double sum;     /* Sum of terms */
  int cnt;        /* Number of elements summed */
};

std::stringstream s0;
/*
** Routines used to compute the sum or average.
*/
static void SStep(sqlite3_context *context, int argc, sqlite3_value **argv){
  SCtx *p;
  int i;
  if( argc<1 ) return;
  p = (SCtx *) sqlite3_aggregate_context(context, sizeof(*p));
  if( p->cnt == 0)
    s0.str("");

  s0 << "(";
    

    p->sum += sqlite3_value_double(argv[0]);
    p->cnt++;
    s0 <<  p->sum ;

    if (p->cnt == 1)
      {
	for(i=1; i< argc; ++i) {
          p->cnt++;
	  p->sum+=sqlite3_value_double(argv[i]);
          s0 <<  "," << p->sum ;
	  }

      }

}
static void SFinalize(sqlite3_context *context){
  SCtx *p;
  char *buf=NULL;


  s0 << ")";
  buf = (char *) malloc (sizeof(char)*(s0.str().size()+2));
  if (buf == NULL)
    fprintf(stderr,"malloc error in SNFinalize, buf\n");
  
  p = (SCtx *) sqlite3_aggregate_context(context, sizeof(*p));
  snprintf(buf,s0.str().size()+1,"%s",s0.str().c_str());
  sqlite3_result_text(context,buf,s0.str().size()+1,free );

}






/*
** An instance of the following structure holds the context of a
** sum() or avg() aggregate computation.
*/
typedef struct SumCtx SumCtx;
struct SumCtx {
  double sum;     /* Sum of terms */
  int cnt;        /* Number of elements summed */
};

std::stringstream ss;
/*
** Routines used to compute the sum or average.
*/
static void SNStep(sqlite3_context *context, int argc, sqlite3_value **argv){
  SumCtx *p;
  int i;
  if( argc<1 ) return;
  p = (SumCtx *) sqlite3_aggregate_context(context, sizeof(*p));
  if( p->cnt == 0)
    ss.str("");


    p->sum += sqlite3_value_double(argv[0]);
    p->cnt++;
    ss << "(" << p->cnt << "," << p->sum << ")";

    if (p->cnt == 1)
      {
	for(i=1; i< argc; ++i) {
          p->cnt++;
	  p->sum+=sqlite3_value_double(argv[i]);
          ss << "(" << p->cnt << "," << p->sum << ")";
	  }

      }

}
static void SNFinalize(sqlite3_context *context){
  SumCtx *p;
  char *buf=NULL;


  
  buf = (char *) malloc (sizeof(char)*(ss.str().size()+2));
  if (buf == NULL)
    fprintf(stderr,"malloc error in SNFinalize, buf\n");
  
  p = (SumCtx *) sqlite3_aggregate_context(context, sizeof(*p));
  snprintf(buf,ss.str().size()+1,"%s",ss.str().c_str());
  sqlite3_result_text(context,buf,ss.str().size()+1,free );

}








/*
** An instance of the following structure holds the context of a
** list() from aggregate
*/
typedef struct listCtx listCtx;
struct listCtx {
  int cnt;        /* Number of elements in list */
};

std::stringstream ss2;
/*
** 
*/
static void listStep(sqlite3_context *context, int argc, sqlite3_value **argv){
  listCtx *p;
  int i;

  ss2.str("");
  ss2 << "(";

  if( argc<1 ) 
     return;


  p = (listCtx *) sqlite3_aggregate_context(context, sizeof(*p));

  p->cnt++;   
  ss2 << "(" <<  p->cnt<< "," << sqlite3_value_text(argv[0]) << ")";

  if (p->cnt == 1)
   {
    for(i=1; i< argc; ++i) {
     p->cnt++;
     ss2 << ",(" <<  p->cnt << "," << sqlite3_value_text(argv[i]) << ")";
      }
    }
}
static void listFinalize(sqlite3_context *context){

  char *buf=NULL;
  ss2 << ")";
  
  buf = (char *) malloc (sizeof(char)*(ss2.str().size()+2));
  if (buf == NULL)
    fprintf(stderr,"malloc error in listFinalize, buf\n");
  

  snprintf(buf,ss2.str().size()+1,"%s",ss2.str().c_str());
  sqlite3_result_text(context,buf,ss2.str().size()+2,free );
  ss2.clear();

}





int main(int argc, char **argv){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  if( argc!=3 ){
    fprintf(stderr, "Usage: %s DATABASE 'select S(1,2,3,4,5,6)'\n", argv[0]);
    exit(1);
  }
  rc = sqlite3_open(argv[1], &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(1);
  }

  if (sqlite3_create_function(db, "msign", 1, SQLITE_UTF8, NULL, &msignFunc, NULL,
			      NULL) != 0)
    fprintf(stderr,"Problem with msign\n");



  if (sqlite3_create_function(db, "T", -1, SQLITE_UTF8, NULL, &TFunc, NULL,
			      NULL) != 0)
    fprintf(stderr,"Problem with T using just TFunc \n");


  if (sqlite3_create_function(db, "I", 2, SQLITE_UTF8, NULL, &IFunc, NULL,
			      NULL) != 0)
    fprintf(stderr,"Problem with I using just IFunc \n");


  if (sqlite3_create_function(db, "F", 1, SQLITE_UTF8, NULL, &FFunc, NULL,
			      NULL) != 0)
    fprintf(stderr,"Problem with F using just FFunc \n");




  /*
   *  With 2 or more arguments call the simple function ssum. Simple functions
   *  can be used within an expression. Aggregate functions can only be used
   *  in a select.
   */
  
  if (sqlite3_create_function(db, "S", -1, SQLITE_UTF8, NULL, &SFunc, NULL,
			      NULL) != 0)
    fprintf(stderr,"Problem with S using just SFunc \n");
  


  /*
   *  With one argument call the aggregate function.
   */

  if (sqlite3_create_function(db, "S", 1, SQLITE_UTF8, NULL, NULL, &SStep,
			      &SFinalize) != 0)
    fprintf(stderr,"Problem with S using SStep and SFinalize\n");









  /*
   *  With 2 or more arguments call the simple function ssum. Simple functions
   *  can be used within an expression. Aggregate functions can only be used
   *  in a select.
   */
  
  if (sqlite3_create_function(db, "SN", -1, SQLITE_UTF8, NULL, &SNFunc, NULL,
			      NULL) != 0)
    fprintf(stderr,"Problem with ssum using just SNFunc \n");
  


  /*
   *  With one argument call the aggregate function.
   */

  if (sqlite3_create_function(db, "SN", 1, SQLITE_UTF8, NULL, NULL, &SNStep,
			      &SNFinalize) != 0)
    fprintf(stderr,"Problem with ssum using SNStep and SNFinalize\n");
  

  if (sqlite3_create_function(db, "L", -1, SQLITE_UTF8, NULL, NULL, &listStep,
			      &listFinalize) != 0)
    fprintf(stderr,"Problem with list using listStep and listFinalize\n");


  rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
  }
  sqlite3_close(db);
  return 0;
}

