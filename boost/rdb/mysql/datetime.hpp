#ifndef BOOST_MYSQL_DATETIME_HPP
#define BOOST_MYSQL_DATETIME_HPP

#include <boost/rdb/mysql_database.hpp>
#include <boost/rdb/core/datetime.hpp>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>

#include <chrono>
#include <ctime>


namespace boost { namespace rdb { namespace mysql {

  class datetime {
  public:
    datetime() : length_(SQL_NULL_DATA) { }
    
    datetime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int fraction = 0) {
      value_.tm_year = year;
      value_.tm_mon = month;
      value_.tm_mday = day;
      value_.tm_hour = hour;
      value_.tm_min = minute;
      value_.tm_sec = second;
      value_.tm_isdst = fraction;

      length_ = sizeof(value_);
    }
    
    void set_null() { length_ = SQL_NULL_DATA; }
    bool is_null() const { return length_ == SQL_NULL_DATA; }

    const std::tm& value() const { return value_; }
    std::tm& value() { return value_; }
    
    boost::posix_time::ptime cpp_value() const {
      using namespace boost::gregorian;
      using namespace boost::posix_time;
      return ptime(date(value_.tm_year, value_.tm_mon, value_.tm_mday),
        time_duration(value_.tm_hour, value_.tm_min, value_.tm_sec, value_.tm_isdst));
    }
    
    typedef core::datetime rdb_type;
    typedef boost::posix_time::ptime cpp_type;
  
  //private:
    //SQL_TIMESTAMP_STRUCT value_;
    //std::chrono::time_point<std::chrono::system_clock> value_;
    std::tm value_;
    int length_;
  };
  
  std::ostream& operator <<(std::ostream& os, const datetime& t);

  template<size_t N>
  struct can_bind< core::datetime, mysql::varchar<N> > : mpl::true_ {
  };

  namespace detail {
    
    template<>
    struct bind<datetime> {
      static void result(result_set_typ& result_, int i, datetime& var) {
        //sql_check(SQL_HANDLE_STMT, hstmt, SQLBindCol(hstmt, i, SQL_C_TIMESTAMP,
        //  &var.value_, 0, &var.length_));
          time_t t = static_cast<time_t>(result_->getInt(i));
          var.value() = *std::gmtime( &t );
      }
    
      static void parameter(result_set_typ& result_, int i, datetime& var) {
      }
    };
  }

} } }

namespace boost { namespace rdb { namespace core {

  template<>
  struct cli_type<datetime, mysql::mysql_tag> {
    typedef mysql::datetime type;
  };

  template<>
  struct literal<mysql::datetime, datetime> : any_literal  {
    typedef datetime rdb_type;
    literal(const mysql::datetime& value) : value_(value) { }
    void str(std::ostream& os) const;
    mysql::datetime value_;
  };
  
  template<>
  struct make_literal<datetime, mysql::datetime> {
    typedef literal<mysql::datetime, datetime> type;
    static type value(const mysql::datetime& val) { return type(val); }
  };

} } }

#endif
