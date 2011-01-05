/* Copyright (c) GPL mchirico@users.sourceforge.net

   Example if list using sort on a class
   with user defined comparison "cmp_t".

   compare this with list3b. This is much
   cleaner and does more.

   Downloads:
   http://prdownloads.sourceforge.net/cpearls/simple_but_common_cpp.tar.gz?download

*/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <iterator>
#include <cmath>
#include <sqlite3.h>

#define NUMPTS 5

using namespace std;

class SQLITE3 {
private:
  sqlite3 *db;
  char *zErrMsg;
  char **result;
  int rc;
  int nrow,ncol;
  int db_open;

public:

  std::vector<std::string> vcol_head;
  std::vector<std::string> vdata;


  SQLITE3 (std::string tablename="init.db"): zErrMsg(0), rc(0),db_open(0) {
    rc = sqlite3_open(tablename.c_str(), &db);
    if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
    }
    db_open=1;
  }

  int exe(std::string s_exe) {
      rc = sqlite3_get_table(
			db,              /* An open database */
			s_exe.c_str(),       /* SQL to be executed */
			&result,       /* Result written to a char *[]  that this points to */
			&nrow,             /* Number of result rows written here */
			&ncol,          /* Number of result columns written here */
			&zErrMsg          /* Error msg written here */
			);

      if (vcol_head.size() > 0)
	{ 
         vcol_head.clear();
	}

      if (vdata.size() > 0)
	{ 
         vdata.clear();
	}

     if( rc == SQLITE_OK ){
      for(int i=0; i < ncol; ++i)
	vcol_head.push_back(result[i]); /* First row heading */
      for(int i=0; i < ncol*nrow; ++i)
	vdata.push_back(result[ncol+i]);
     }
     sqlite3_free_table(result);
      return rc;
  }

  ~SQLITE3(){
      sqlite3_close(db);
  } 
};



class t {
  int i;
  int j;

public: 

 t(int ii=0,int jj=0):i(ii),j(jj) {}
  
 void  set_i(int ii) { i=ii; }
  int  get_i() const { return i; }

 void  set_j(int jj) { j=jj; }
  int  get_j() const { return j; }
};


class cmp_t {
  int i;
  int px;
  int py;
  int sizes;

public:
  cmp_t(int ii=0,int xx=0, int yy=0): i(ii),px(xx),py(yy),sizes(6) {}
  int size() { return sizes; }
  bool operator() (const  t*,const  t* y) const;
};

bool cmp_t::operator() (const t* x,const  t* y) const 
{

  switch(i) {
  case 0:
    return x->get_i() < y->get_i();
    break;
  case 1:
    return x->get_j() < y->get_j();
    break;
  case 2:
    return x->get_i() < y->get_j();
    break;
  case 3:
    return x->get_j() < y->get_i();
    break;
  case 4:
    return  ( x->get_i()-x->get_j() ) < ( y->get_i()-y->get_j() ) ;
    break;
  case 5:
    return  ( x->get_j()-x->get_i() ) < ( y->get_j()-y->get_i() ) ;
    break;
  case 6:
    return  sqrt(pow((double)(px-x->get_i()),2)+pow( (double) (py-x->get_j()),2))
                                   <
      sqrt(pow((double)(py-y->get_i()),2)+pow( (double) (py-y->get_j()),2)) ;
    break;
  default:
    return x->get_i() < y->get_i();
  }
}

class within_range: public unary_function<class t*,bool> {
  int px;
  int py;
  double d;
public:
  explicit within_range(int pxx=0, int pyy=0, double dist=1.0): px(pxx),py(pyy),d(dist) {}
  bool operator()(const t* tt) const {
   return sqrt(pow((double)(px-tt->get_i()),2)+pow( (double) (py-tt->get_j()),2)) < d;
  }
};

ostream& operator<<(ostream&s, const t* tt)
{
  return s << '(' << tt->get_i() << ',' << tt->get_j() << ')';
}





int main(void)
{

  list<t*> lst;
  
  vector<t> vpt;





  /*
  if( argc!=3 ){
   std::cerr << "Usage: " << argv[0] << " DATABASE SQL-STATEMENT" << std::endl;
   exit(1);
  }
  */
  //  SQLITE3 sql(argv[1]);
  SQLITE3 sql("data.db");
  sql.exe("select x,y from points");
  if(sql.vcol_head.size() == 0 )
    {
     sql.exe("create table points (x int, y int)");
     sql.exe("create table results (x int, y int)");
     sql.exe("insert into points (x,y) values (3,7)");
     sql.exe("insert into points (x,y) values (5,2)");
     sql.exe("insert into points (x,y) values (6,12)");
     sql.exe("insert into points (x,y) values (-6,-12)");
     sql.exe("select x,y from points");
    }

  if( sql.vcol_head.size() > 0 )
    {

      std::cout << "Heading SIZE:" <<  sql.vcol_head.size() << std::endl;
      std::cout << "Headings" << std::endl;
      copy(sql.vcol_head.begin(),sql.vcol_head.end(),std::ostream_iterator<std::string>(std::cout,"\t")); 
      std::cout << std::endl << std::endl;
      std::cout << "Data" << std::endl;
      copy(sql.vdata.begin(),sql.vdata.end(),std::ostream_iterator<std::string>(std::cout,"\t")); 
      std::cout << std::endl;

      if( 2*vpt.size() < sql.vdata.size()) { 	vpt.resize(sql.vdata.size()/2); }

      int j=0;
      for(unsigned int i=0; i < sql.vdata.size(); i+=2)
	{
          vpt[j].set_i(atoi(sql.vdata[i].c_str()));
          vpt[j].set_j(atoi(sql.vdata[i+1].c_str()));
          j++;
	}
    }


  for(unsigned int i=0; i < vpt.size(); ++i) {
    cout << "vpt["<<i<<"] " << &vpt[i] << endl << endl;
  }



  cout << "vpt[0] " << &vpt[0] << endl << endl;


  for(unsigned int i=0; i<vpt.size(); ++i)
  lst.push_back(&vpt[i]);


  for(int i=0; i<=cmp_t().size(); ++i) {
    lst.sort(cmp_t(i));
    copy(lst.begin(),lst.end(),ostream_iterator<t*>(cout," "));
    cout << endl << endl;
  }


  cout << endl << "** CHANGED vpt[4] ****" << endl;

  vpt[4].set_i(vpt[4].get_i()*10);
  vpt[4].set_j(vpt[4].get_j()*10);

  for(int i=0; i<=cmp_t().size(); ++i) {
    lst.sort(cmp_t(i));
    copy(lst.begin(),lst.end(),ostream_iterator<t*>(cout," "));
    cout << endl << endl;
  }


    lst.sort(cmp_t(6,-30,70));
    copy(lst.begin(),lst.end(),ostream_iterator<t*>(cout," "));
    cout << endl << endl;

    // Find point within the given range
    int my_px=3;
    int my_py=5;
    double range=3;
    list<t*>::iterator p = find_if(lst.begin(),lst.end(),within_range(my_px,my_py,range));
    if ( p != lst.end()) {
         cout << "First point within " << range 
           << " units of (" << my_px << ","
           << my_py << ") is " << *p << endl; 
       } else {
          cout << "NO points within " << range 
           << " units of (" << my_px << ","
           << my_py << ") " << endl;
      }

 
  for( list<t*>::const_iterator p=lst.begin(); p!=lst.end(); ++p)
    { 
    stringstream ss;
    ss << "insert into results (x,y) values (" << (*p)->get_i() << "," << (*p)->get_j() << ")";
    sql.exe(ss.str());
    }




  return 0;













}





