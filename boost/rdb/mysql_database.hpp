#ifndef __MYSQL_DATABASE_H_
#define __MYSQL_DATABASE_H_

namespace {
  const int SQL_NULL_DATA = -1;
}

#include <name_of/name_of.h>

#include <memory>
#include <exception>



using std::unique_ptr;



#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>


#include <boost/rdb/core.hpp>
#include <boost/rdb/core/datetime.hpp>
#include <boost/rdb/dynamic.hpp>
#include <boost/rdb/sql/alias.hpp>
#include <boost/rdb/sql/table.hpp>

#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/accumulate.hpp>
#include <boost/fusion/include/push_back.hpp>
#include <boost/fusion/include/zip_view.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/iter_fold.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/erase.hpp>
#include <boost/fusion/include/advance.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/int.hpp>

#include <boost/lexical_cast.hpp>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <deque>
#include <stdexcept>

#include <iostream>
using std::cout;
using std::endl;

#include "name_of/name_of.h"

namespace boost { namespace rdb {

  //template<class SqlType, class Value, class Tag>
  //struct sql_type_adapter;

  //struct dynamic_value_mismatch : std::exception {
  //  virtual const char* what() const throw();
  //};

} }

namespace boost { namespace rdb { namespace mysql {

    using connection_typ         = unique_ptr< ::sql::Connection >;
    using statement_typ          = unique_ptr< ::sql::Statement >;
    using prepared_statement_typ = unique_ptr< ::sql::PreparedStatement >;
    using result_set_typ         = unique_ptr< ::sql::ResultSet >;

    namespace mpl                    = boost::mpl;

  template <class T>
  struct alias_expr
  {
      using type = T;
  };

  template <class Expr, class Name>
  struct alias_expr< ::boost::rdb::sql::sql_column_alias<Expr, Name> >
  {
      using type = Expr;
  };

  template <class ExprList, class Expr>
  struct expr_pos
  {
      using found_type = typename fusion::result_of::find<ExprList, typename alias_expr<Expr>::type >::type;

      using type = typename fusion::result_of::distance<typename  fusion::result_of::begin<ExprList>::type, found_type >::type;

      static constexpr bool found = !boost::is_same< found_type, typename fusion::result_of::end<ExprList>::type >::value;
  };
#if 0
    template <int CurPos, int SearchPos, class OldV, class NewV, class NewVal >
    OldV nullable_replace_imp( const OldV & , const NewV& new_seq, const NewVal&, mpl::bool_<true> )
    {
        //static_name_of < mpl::int_<CurPos> > asas;
        return new_seq;
    }

    template <class Seq, class OldVal, class NewVal>
    //typename fusion::result_of::push_back<Seq, OldVal>::type
    auto pusher_to_vector( const Seq& seq, const OldVal& old_val, const NewVal& , mpl::bool_<false> )
        -> decltype(fusion::push_back( seq, old_val ))
    {
        return fusion::push_back( seq, old_val );
    }

    template <class Seq, class OldVal, class NewVal>
    //typename fusion::result_of::push_back<Seq, NewVal>::type
    auto pusher_to_vector( const Seq& seq, const OldVal& , const NewVal& new_val, mpl::bool_<true> )
        -> decltype(fusion::push_back( seq, new_val ))
    {
        return fusion::push_back( seq, new_val );
    }


    template <int CurPos, int SearchPos, class OldV, class NewV, class NewVal >
    OldV nullable_replace_imp( const OldV & old_seq, const NewV& new_seq, const NewVal& new_val, mpl::bool_<false>  )
    {        
        using tt = typename mpl::equal_to<mpl::int_<CurPos>, typename fusion::result_of::size<OldV>::type >::type;
        using found = typename mpl::equal_to< mpl::int_<CurPos>, mpl::int_<SearchPos> >::type;

        auto vv = pusher_to_vector( new_seq, fusion::at_c<CurPos>(old_seq), new_val,  found());

        return nullable_replace_imp<CurPos + 1, SearchPos>(old_seq, vv, new_val, tt() );
    }




    template <int Pos, class V, class NewVal>
    V nullable_replace( const V & seq, const NewVal& new_val )
    {
        //static_name_of<  mpl::int_<Pos> > aaa;

        return nullable_replace_imp<0, Pos>( seq, fusion::vector<>(), new_val,
                                             typename mpl::equal_to< mpl::int_<Pos>, typename fusion::result_of::size<V>::type >::type() );

    }

#endif

    template <class Nullable>
    struct nullable_setter
    {
        nullable_setter( Nullable& n ):nullabl(n){}

        template <class Col, class CppTyp, class SqlTyp>
        void operator()(const boost::rdb::sql::set_clause<Col, ::boost::rdb::core::literal<CppTyp, SqlTyp>>& s ) const
        {            
            nullabl.ref< boost::rdb::sql::expression<Col> >() = s.expr_.value_;
            //static_name_of <Expr> asas;
        }

        template <class Col, class CppTyp>
        void operator()(const boost::rdb::sql::set_clause<Col, ::boost::rdb::core::literal<CppTyp, ::boost::rdb::core::datetime>>& s ) const
        {
            nullabl.ref< boost::rdb::sql::expression<Col> >() =
                    ::boost::posix_time::time_from_string(  s.expr_.value_ );
        }

        Nullable & nullabl;
    };

  template<class Seq, class ExprList>
  struct nullable {
      using expr_list = ExprList;

    Seq values_;
    typedef std::bitset<fusion::result_of::size<Seq>::value> status_vector_type;
    typedef Seq value_vector_type;
    status_vector_type status_;
    bool is_null(int pos) const { return !status_[pos]; }
    template<int I> bool is_null() const { return !status_[I]; }
    void set_null(int pos, bool to_null) { status_[pos] = !to_null; }
    template<int I> typename fusion::result_of::at_c<const Seq, I>::type get() const {
      return fusion::at_c<I>(values_);
    }
    template<int I> typename fusion::result_of::at_c<Seq, I>::type ref() {
      return fusion::at_c<I>(values_);
    }

    template<class Key>
    typename fusion::result_of::at<
        Seq,
        typename expr_pos<ExprList, Key>::type
    >::type& ref()
    {
        static_assert(expr_pos<ExprList, Key>::found, "Cant find column in result!");

        return fusion::at<typename expr_pos<ExprList, Key>::type>(values_);
    }

    const Seq& values() const { return values_; }
    Seq& values() { return values_; }
    const status_vector_type& status() const { return status_; }
    status_vector_type& status() { return status_; }
    nullable& operator =(const Seq& values) { values_ = values; return *this; }



    template <class Expr>
    typename fusion::result_of::at<
        const Seq,
        typename expr_pos<ExprList, Expr>::type
    >::type
    operator[]( const Expr& ) const
    {
        static_assert(expr_pos<ExprList, Expr>::found, "Cant find column in result!");

        return fusion::at< typename expr_pos<ExprList, Expr>::type >( values_ );
    }

    template <class Expr>
    typename std::remove_const <
        typename std::remove_reference <
            typename fusion::result_of::at<
                const Seq,
                typename expr_pos<ExprList, Expr>::type
            >::type
        >::type
    >::type &
    operator[]( const Expr& )
    {
        static_assert(expr_pos<ExprList, Expr>::found, "Cant find column in result!");
        return  fusion::at< typename expr_pos<ExprList, Expr>::type >( values_ );
    }

    template <class ... Args>
    void set( const fusion::vector<Args ...>& v )
    {
        fusion::for_each( v, nullable_setter< nullable<Seq, ExprList> >(*this) );
    }



    template <int N>
    using type_of_c = fusion::result_of::at_c<Seq, N>;

    using size = typename fusion::result_of::size<Seq>::type;
  };


    template <class Seq, class ExprList, class Key>
    nullable <
        typename fusion::result_of::as_vector <
            typename fusion::result_of::erase <
                Seq,
                typename fusion::result_of::advance<
                    typename fusion::result_of::begin<Seq>::type,
                    typename expr_pos<ExprList, Key>::type
                >::type
            >::type
        >::type,
        typename fusion::result_of::as_vector <
            typename fusion::result_of::erase <
                ExprList,
                typename fusion::result_of::advance<
                    typename fusion::result_of::begin<ExprList>::type,
                    typename expr_pos<ExprList, Key>::type
                >::type
            >::type
        >::type
    >
    erase_nullable( const nullable<Seq, ExprList>& nullab, const Key& )
    {
        static_assert(expr_pos<ExprList, Key>::found, "Cant find column in result!");

        auto values = fusion::as_vector (
                        fusion::erase( nullab.values_,
                            fusion::advance<typename expr_pos<ExprList, Key>::type>(
                                fusion::begin(nullab.values_))));

        using keys = typename fusion::result_of::as_vector <
                        typename fusion::result_of::erase <
                            ExprList,
                            typename fusion::result_of::advance<
                                typename fusion::result_of::begin<ExprList>::type,
                                typename expr_pos<ExprList, Key>::type
                            >::type
                        >::type
                    >::type;

        nullable< decltype(values), keys > ret;
        ret.values_ = values;
        return std::move(ret);
    }


  template<class Row>
  struct print_row_element {

    print_row_element(std::ostream& os, const Row& r) : os_(os), r_(r), bit_(0) { }

    std::ostream& os_;
    const Row& r_;
    mutable int bit_;

    template<class T>
    void operator ()(const T& value) const {
      if (bit_)
        os_ << " ";
      if (r_.is_null(bit_++))
        os_ << "null";
      else
        os_ << value;
    }
  };

  template<class Seq, class ExprList>
  std::ostream& operator <<(std::ostream& os, const nullable<Seq, ExprList>& r) {
    os << "(";
    fusion::for_each(r.values(), print_row_element< nullable<Seq, ExprList> >(os, r));
    os << ")";
    return os;
  }

  extern std::ostream* trace_stream;

  struct mysql_tag { };

  /*struct odbc_error : std::exception {
    odbc_error(SQLSMALLINT handle_type, SQLHANDLE handle, long rc);
    virtual const char* what() const throw();
    long rc;
    SQLCHAR stat[10]; // Status SQL
    SQLINTEGER err;
    char msg[200];
  };*/

  /*inline bool sql_fail(long rc) {
    return rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO;
  }*/

  /*inline void sql_check(SQLSMALLINT handle_type, SQLHANDLE handle, long rc) {
    if (sql_fail(rc)) {
      odbc_error ex(handle_type, handle, rc);
      //TR << "ODBC Exception: " << ex.what() << endl;
      throw ex;
    }
  }*/

  namespace detail {
    template<class CliType>
    struct bind;
  }

  template<class SqlType, class CliType>
  struct can_bind : is_same<SqlType, typename CliType::rdb_type> {
  };

  class dynamic_value : public dynamic::abstract_dynamic_value {
  public:
    dynamic_value(int type, int length) : abstract_dynamic_value(type, length) { }
    virtual void bind_result(statement_typ& stmt, int i) = 0;
    virtual void bind_parameter(statement_typ& stmt, int i) = 0;
  };

  typedef std::vector< intrusive_ptr<dynamic_value> > dynamic_values;

  template<class RdbType, class CliType, class CppType = CliType>
  class simple_numeric_type {
  public:
    simple_numeric_type() : value_(CliType()), length_(SQL_NULL_DATA) { }

    template<typename T>
    simple_numeric_type(const T& value) : value_(static_cast<T>(value)), length_(sizeof(value_)) { }

    template<typename T>
    simple_numeric_type& operator =(const T& value) {
      value_ = static_cast<T>(value);
      length_ = sizeof(value_);
      return *this;
    }

    void set_null() { length_ = SQL_NULL_DATA; }
    bool is_null() const { return length_ == SQL_NULL_DATA; }
    const CliType& value() const { return value_; }
    CliType& value() { return value_; }
    CppType cpp_value() const { return value_; }

    typedef RdbType rdb_type;
    typedef CppType cpp_type;

  //private:
    CliType value_;
    int length_;
  };

  typedef simple_numeric_type<core::integer, int> integer;

  namespace detail {

    template<>
    struct bind<integer> {
      static void result(result_set_typ& result_, int i, integer& var) {
          var = result_->getInt(i);
      }

      static void parameter(statement_typ & , int , integer& ) {
      }
    };
  }

  template<class CliType>
  class generic_dynamic_value : public dynamic_value {
  public:

    generic_dynamic_value(const CliType& var) :
      dynamic_value(CliType::rdb_type::id, CliType::rdb_type::length),
                                     var_(const_cast<CliType&>(var)) { }

    virtual void bind_result(statement_typ & stmt, int i) override {
      //detail::bind<CliType>::result(hstmt, i, var_);
    }

    virtual void bind_parameter(statement_typ stmt, int i) override {
      //detail::bind<CliType>::parameter(hstmt, i, var_);
    }

  private:
    CliType& var_;
  };

  template<class RdbType, class CliType>
  inline intrusive_ptr<dynamic_value> make_dynamic(simple_numeric_type<RdbType, CliType>& lvalue) {
    return new generic_dynamic_value< simple_numeric_type<RdbType, CliType> >(lvalue);
  }

  typedef simple_numeric_type<core::float_, double> float_;

  namespace detail {
    template<>
    struct bind<float_> {
      static void result(result_set_typ& result_, int i, float_& var) {
//        sql_check(SQL_HANDLE_STMT, hstmt, SQLBindCol(hstmt, i, SQL_C_DOUBLE,
//          &var.value_, 0, &var.length_));
        var = result_->getDouble(i);
      }

      static void parameter(statement_typ & stmt, int i, float_& var) {
//        sql_check(SQL_HANDLE_STMT, hstmt, SQLBindParameter(hstmt, i, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0,
//          (SQLPOINTER) &var.value_, 0, (SQLINTEGER*) &var.length_));
      }
    };
  }

  typedef simple_numeric_type<core::boolean, bool> boolean;
  namespace detail {
  template<>
  struct bind<boolean> {
        static void result(result_set_typ& result_, int i, boolean& var) {
            var = result_->getBoolean(i);
        }

        static void parameter(statement_typ & , int , boolean& ) {
        }
    };
  }

  template<size_t N>
  class varchar {
  public:
    BOOST_STATIC_CONSTANT(size_t, size = N);

    typedef core::varchar<N> rdb_type;
    typedef std::string cpp_type;

    varchar() /*: length_(0)*/ {
    }

    varchar(const char* str) /*: length_(0)*/ {
      *this = str;
    }

    varchar(const std::string& str)/* : length_(0)*/ {
      *this = str;
    }

    operator std::string() const { return value_;/*return std::string(chars_, chars_ + length()); */ }
    std::string cpp_value() const {  return value_;/* return std::string(chars_, chars_ + length());  */}
    const char* value() const { return value_.c_str();/* return reinterpret_cast<const char*>(chars_); */}
    size_t length() const { return value_.length(); } //length_; }

    template<int Length>
    varchar& operator =(const char (&str)[Length]) {
      BOOST_STATIC_ASSERT(Length <= N);
      /*const char* src = str;
      char* dest = chars_;
      while (*dest++ = *src++);
      length_ = dest - chars_ - 1; */
      value_ = str;
      return *this;
    }

    varchar& operator =(const char* src)
    {
/*      char * dest = chars_;
      char * last = chars_ + N;
      while (*src) {
        if (dest == last)
          throw std::overflow_error("1: overflow in varchar");
        *dest++ = *src++;
      }
      *dest = 0;
      length_ = dest - chars_;*/
      value_ = src;
      return *this;
    }

    varchar& operator =(const std::string& src)
    {
      /*if (src.length() > N)
        throw std::range_error("2: overflow in varchar. Received length: " +
                               boost::lexical_cast<std::string>(src.length())  + " value: " + src);
      *std::copy(src.begin(), src.end(), chars_) = 0;
      length_ = src.length();*/
        value_ = src;
      return *this;
    }

    void set_null() { value_ = std::string(); /* length_ = SQL_NULL_DATA; */}
    bool is_null() const { return value_.empty(); }

 private:
    //int length_;
    //char chars_[N + 1];
    std::string value_;

    template<class SqlType, class Value, class Tag> friend struct sql_type_adapter;
  };

  template<size_t N>
  inline intrusive_ptr<dynamic_value> make_dynamic(varchar<N>& lvalue) {
    return new generic_dynamic_value< varchar<N> >(lvalue);
  }

  namespace detail {

    template<size_t N>
    struct bind< varchar<N> > {
      static void result(result_set_typ& result_, int i, varchar<N>& var) {
//        sql_check(SQL_HANDLE_STMT, hstmt, SQLBindCol(hstmt, i, SQL_C_CHAR,
//          var.chars_, N + 1, &var.length_));
          var = result_->getString(i);
      }

      static void parameter(statement_typ & hstmt, int i, varchar<N>& var) {
//        sql_check(SQL_HANDLE_STMT, hstmt, SQLBindParameter(hstmt, i, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, N, 0,
//          (SQLPOINTER) var.chars_, 0, (SQLINTEGER*) &var.length_));
      }
    };
  }

  template<size_t N>
  class dynamic_varchar_value : public dynamic_value {
  public:

    dynamic_varchar_value(int type, int length, varchar<N>& var) :
                                        dynamic_value(type, length), var_(var) { }

    virtual void bind_result(result_set_typ & result_, int i) override {
//      sql_check(SQL_HANDLE_STMT, hstmt, SQLBindCol(hstmt, i, SQL_C_CHAR,
//        var_.chars_, N + 1, &var_.length_));
        var_ = result_->getString(i);
    }


    virtual void bind_parameter(statement_typ& hstmt, int i) override {
//      sql_check(SQL_HANDLE_STMT, hstmt, SQLBindParameter(hstmt, i, SQL_PARAM_INPUT, SQL_C_CHAR,
//         SQL_CHAR, N, 0,  &var_.chars_, 0, &var_.length_));
    }

  private:
    varchar<N>& var_;
  };

} } }

namespace boost { namespace rdb { namespace core {

  template<size_t N>
  struct cli_type<varchar<N>, mysql::mysql_tag> {
    typedef mysql::varchar<N> type;
  };

  template<>
  struct cli_type<integer, mysql::mysql_tag> {
    typedef mysql::integer type;
  };

  template<>
  struct cli_type<float_, mysql::mysql_tag> {
    typedef mysql::float_ type;
  };

  template<>
  struct cli_type<boolean, mysql::mysql_tag> {
    typedef mysql::boolean type;
  };

  template<>
  struct cli_type<dynamic::dynamic_expressions, mysql::mysql_tag> {
    typedef mysql::dynamic_values type;
  };

} } }

namespace boost { namespace rdb { namespace mysql {

  struct on_type { };
  const on_type on = on_type();

  struct off_type { };
  const off_type off = off_type();

  template<class Specific>
  class generic_database {
    Specific& spec() { return static_cast<Specific&>(*this); }

  public:
  };

  class mysql_database;


  template<size_t N>
  std::ostream& operator <<(std::ostream& os, const varchar<N>& str) {
    if (str.is_null())
      os << "<null>";
    else
      os.write(str.value(), str.length());
    return os;
  }

  template<class Row>
  struct read_row {
    read_row(Row& row) : row_(row), i_(0) { }
    Row& row_;
    mutable int i_;

    template<class CliType, class Value>
    void operator ()(const fusion::vector2<CliType, Value&>& value) const {

      using namespace fusion;

      row_.set_null(i_, at_c<0>(value).is_null());

      if (!at_c<0>(value).is_null())
        at_c<1>(value) = at_c<0>(value).cpp_value();

      ++i_;
    }
  };

  template<class Select>
  class prepared_select_statement;

  template<class Statement>
  class prepared_statement;

  template<class ExprList, bool Own>
  class result_set;



    class mysql_database  {
    public:


    mysql_database() { }
    mysql_database(mysql_database&& other)
    {
        std::swap( connection_, other.connection_ );
        std::swap( host_, other.host_ );
        std::swap( database_, other.database_);
        std::swap( user_, other.user_ );
        std::swap( password_, other.password_ );
    }

    mysql_database& operator=(mysql_database&& other)
    {
        connection_ = std::move(other.connection_);
        host_       = std::move(other.host_ );
        database_   = std::move(other.database_);
        user_       = std::move(other.user_ );
        password_   = std::move(other.password_ );

        return *this;
    }

    ~mysql_database();



    mysql_database(const std::string& h, const std::string& db, const std::string& u, const std::string& p) {
      open(h, db, u, p);
    }

    void open(const std::string& h, const std::string& db, const std::string& u, const std::string& p);
    void open();
    void close();

    void setHost(const std::string& h){host_ = h;}
    void setDatabase(const std::string& db){database_ = db;}
    void setUser(const std::string& u){user_ = u;}
    void setPassword(const std::string& p){password_ = p;}

    std::string host() const{return host_;}
    std::string database() const{return database_;}
    std::string user() const{return user_;}
    std::string password() const{return password_;}

    bool isClosed() {
        if (!connection_) return true;
        return connection_->isClosed();
    }

    connection_typ & connection() {return connection_;}

    template<class Tag, class Stat>
    struct discriminate {

      typedef typename Stat::result execute_return_type;

      static execute_return_type execute(mysql_database& db, const Stat& stat) {
          statement_typ stmt;
          return db.exec_str(stmt, as_string(stat));
      }

      typedef prepared_statement<Stat> prepare_return_type;

      static prepare_return_type prepare(mysql_database& db, const Stat& st) {
        //HSTMT hstmt;
        //sql_check(SQL_HANDLE_DBC, db.hdbc_, SQLAllocStmt(db.hdbc_, &hstmt));
          statement_typ stmt ;//= db.connection()->c
        try {
          db.prepare_str(stmt, as_string(st));
          return prepare_return_type(st, stmt);
        } catch (...) {
          //SQLFreeHandle(SQL_HANDLE_STMT, &hstmt);
          throw;
        }
      }
    };

    template<class Select>
    struct discriminate<core::tabular_result_tag, Select> {

      typedef result_set<typename core::statement_result_type<Select>::type, false> execute_return_type;

      static execute_return_type execute(mysql_database& db, const Select& select) {
          statement_typ stmt;
        try {
          db.exec_str(stmt, as_string(select));
          return execute_return_type(std::move(stmt), exprs(select));
        }
        catch( ... ) {
            throw;
        }
      }

      typedef prepared_select_statement<Select> prepare_return_type;

      static prepare_return_type prepare(mysql_database& db, const Select& select) {
        //HSTMT hstmt;
        statement_typ stmt;
        //sql_check(SQL_HANDLE_DBC, db.hdbc_, SQLAllocStmt(db.hdbc_, &hstmt));
        db.prepare_str(stmt, as_string(select));
        return prepare_return_type(select, stmt);
      }
    };


    // why doesn't the line below work ?
    // BOOST_CONCEPT_REQUIRES(((Statement<Stat>)), (typename discriminate<typename Stat::tag, Stat>::execute_return_type))
    template<class Stat>
    typename discriminate<typename Stat::tag, Stat>::execute_return_type
    execute(const Stat& st) {
      // error "tag is not a member..." probably means that you tried to execute a statement that is not complete, e.g. `insert(t)`
      return discriminate<typename Stat::tag, Stat>::execute(*this, st);
    }

    template<class Stat>
    typename discriminate<typename Stat::tag, Stat>::prepare_return_type
    prepare(const Stat& st) {
      // error "tag is not a member..." probably means that you tried to prepare a statement that is not complete, e.g. `insert(t)`
      return discriminate<typename Stat::tag, Stat>::prepare(*this, st);
    }

    void prepare_str(statement_typ & , const std::string& sql);

    void exec_str(statement_typ &, const std::string& sql);
    //void exec_str(const std::string& sql) { return exec_str(statement_, sql); }

/*
    void set_autocommit(on_type) {
      SQLSetConnectOption(hdbc_, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_ON);
    }

    void set_autocommit(off_type) {
      SQLSetConnectOption(hdbc_, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
    }

    bool is_txn_capable() const {
      SQLSMALLINT res;
      SQLGetInfo(hdbc_, SQL_TXN_CAPABLE, &res, sizeof res, NULL);
      return res != SQL_TC_NONE;
    }

    void commit() {
      SQLTransact(henv_, hdbc_, SQL_COMMIT);
    }

    void rollback() {
      SQLTransact(henv_, hdbc_, SQL_ROLLBACK);
    }
*/
  private:
    std::string host_, database_ , user_, password_;



    connection_typ connection_;
    //statement_typ  statement_;
  };

  struct parameter_binder {
    parameter_binder(statement_typ & stmt) : stmt_(stmt)/*, i_(1)*/ { }
    statement_typ & stmt_;
    mutable int i_ = 0;

    template<class T1, class T2>
    void operator ()(const fusion::vector<T1&, T2&>& zip) const {
      detail::bind<T2>::parameter(stmt_, i_, fusion::at_c<1>(zip));
      ++i_;
    }

    void operator ()(const fusion::vector<const dynamic::dynamic_placeholders&, dynamic_values&>& zip) const;
  };

  template<class Tag>
  struct make_cli_result_vector {
    typedef make_cli_result_vector Self;

    template<typename Sig>
    struct result;

    template<class CliVector, class Expr>
    struct result<Self(const CliVector&, Expr)> {
      typedef typename fusion::result_of::push_back<
        CliVector,
        typename core::cli_type<
          typename remove_reference<Expr>::type::rdb_type,
          Tag
        >::type
      >::type type;
    };
  };

  template<class CliType>
  struct compact_result {
    typedef typename CliType::cpp_type type;
  };

  template<>
  struct compact_result<dynamic_values> {
    typedef dynamic_values type;
  };

  struct make_compact_result_vector {
    typedef make_compact_result_vector Self;

    template<typename Sig>
    struct result;

    template<class CliType, class Vector>
    struct result<Self(const Vector&, CliType)> {
      typedef typename fusion::result_of::push_back<
        Vector,
        typename compact_result<typename remove_reference<CliType>::type>::type
      >::type type;
    };
  };

  template<class Tag>
  struct make_cli_param_vector {
    typedef make_cli_param_vector Self;

    template<typename Sig>
    struct result;

    template<class Placeholder, class CliVector>
    struct result<Self(Placeholder, const CliVector&)> {
      typedef typename fusion::result_of::push_back<
        CliVector,
        typename core::cli_type<
          typename remove_reference<Placeholder>::type::rdb_type,
          Tag
        >::type
      >::type type;
    };
  };

  template<class Statement>
  class prepared_statement {

    typedef typename fusion::result_of::as_vector<typename Statement::placeholder_vector>::type placeholder_vector;
    placeholder_vector placeholders_;

  public:
    prepared_statement(const Statement& st, statement_typ & stmt) :
      placeholders_(st.placeholders()), stmt_(stmt) {
    }

    ~prepared_statement() {
    //  SQLCloseCursor(hstmt_);
    //  SQLFreeHandle(SQL_HANDLE_STMT, hstmt_);
    }

    template<class Params>
    void executev(const Params& params) {
      typedef typename fusion::result_of::as_vector<
        typename fusion::result_of::accumulate<
          typename Statement::placeholder_vector,
          fusion::vector<>,
          make_cli_param_vector<mysql_tag>
        >::type
      >::type cli_param_vector;
      cli_param_vector cli_params(params);
      typedef fusion::vector<const placeholder_vector&, cli_param_vector&> zip;
      fusion::for_each(fusion::zip_view<zip>(zip(placeholders_, cli_params)),
        parameter_binder(stmt_));
      //sql_check(SQL_HANDLE_STMT, hstmt_, SQLExecute(hstmt_));
      //stmt_->execute();
    }

    void execute() {
      //sql_check(SQL_HANDLE_STMT, hstmt_, SQLExecute(hstmt_));
        //stmt_->execute();
    }

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/odbc/detail/execute.hpp>
    #include BOOST_PP_ITERATE()

    template<class Params>
    void bind_parameters_(Params& params) {
      typedef fusion::vector<const placeholder_vector&, Params&> zip;
      fusion::for_each(fusion::zip_view<zip>(zip(placeholders_, params)),
        parameter_binder(stmt_));
    }

    //void bind_parameters_(param_ref_vector& params) {
    //  typedef fusion::vector<const placeholder_vector&, param_ref_vector&> zip;
    //  fusion::for_each(fusion::zip_view<zip>(zip(placeholders_, params)),
    //    parameter_binder(hstmt_));
    //}

    #define BOOST_RDB_ADD_REF(z, n, type) BOOST_PP_COMMA_IF(n) type##n&
    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/odbc/detail/bind_parameters.hpp>
    #include BOOST_PP_ITERATE()

  protected:
    statement_typ & stmt_;
  };

  template<>
  struct can_bind<dynamic::dynamic_expressions, dynamic_values> : mpl::true_ {
  };

  struct results_binder {
    results_binder(result_set_typ & r) : result_(r), i_(1) { }
    result_set_typ& result_;
    mutable int i_;

    template<class Expr, class CliType>
    void operator ()(const fusion::vector2<const Expr&, CliType&>& zip) const {
      BOOST_MPL_ASSERT((can_bind<typename Expr::rdb_type, CliType>));

      //cout << "bind: "<< i_ << name_of<decltype(fusion::at_c<1>(zip))>::value() << endl;

        //static_name_of<Expr> aaa;

      detail::bind<CliType>::result(result_, i_, fusion::at_c<1>(zip));
      ++i_;
    }

    void operator ()(const fusion::vector<const dynamic::dynamic_expressions&, dynamic_values&>& zip) const;
  };

  template<class Select>
  class prepared_select_statement : public prepared_statement<Select> {

  public:
    typedef typename core::statement_result_type<Select>::type select_list;
    typedef prepared_statement<Select> base;

    prepared_select_statement(const Select& select, statement_typ& stmt) :
      base(select, stmt), exprs_(exprs(select)) { }

    select_list exprs_;

    result_set<select_list, false> execute() {
      //sql_check(SQL_HANDLE_STMT, this->hstmt_, SQLExecute(this->hstmt_));
      return result_set<select_list, false>(this->stmt_, exprs_);
    }

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/odbc/detail/execute_select.hpp>
    #include BOOST_PP_ITERATE()

    template<class Results>
    prepared_select_statement& bind_results_(const Results& results) {
      typedef fusion::vector<const select_list&, Results&> zip;
      fusion::for_each(fusion::zip_view<zip>(zip(exprs_, const_cast<Results&>(results))),
        results_binder(this->hstmt_));
      return *this;
    }

    #define BOOST_RDB_ADD_REF(z, n, type) BOOST_PP_COMMA_IF(n) type##n&
    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/odbc/detail/bind_results.hpp>
    #include BOOST_PP_ITERATE()
  };

  namespace detail {

    struct is_dynamic_expression_lambda {
      template<class Expr>
      struct apply : is_same<Expr, dynamic::dynamic_expressions> { };
    };

    template<class ExprList>
    struct contains_only_static_expressions : fusion::result_of::equal_to<
      typename fusion::result_of::find_if<ExprList, is_dynamic_expression_lambda>::type,
      typename fusion::result_of::end<ExprList>::type
    > {
    };

  }

  template<class ExprList, bool Own>
  class result_set {

      //static_name_of<ExprList>  aaaa;

    statement_typ  stmt_;
    result_set_typ result_;

    const ExprList& exprs_;
    //bool bound_;

    //static_name_of<ExprList> aaa;

  public:


    typedef typename fusion::result_of::as_vector<
      typename fusion::result_of::accumulate<
        ExprList,
        fusion::vector<>,
        make_cli_result_vector<mysql_tag>
      >::type
    >::type result_vector;

    result_vector results_;

    typedef nullable<
      typename fusion::result_of::as_vector<
        typename fusion::result_of::accumulate<
          result_vector,
          fusion::vector<>,
          make_compact_result_vector
        >::type
      >::type,
      typename fusion::result_of::as_vector<ExprList>::type
    > compact_result_vector;

    typedef std::deque< compact_result_vector > Container;

    typedef typename Container::value_type value_type;

    /*result_set() : exprs_(ExprList())
    {

    }*/

    result_set(statement_typ && stmt, const ExprList& exprs)
        : stmt_( std::move(stmt) ),
          exprs_(exprs)
    {
        if ( stmt_ )
            result_.reset( stmt_->getResultSet() );
    }

    result_set(result_set&& other) :stmt_( std::move(other.stmt_)),
                                    result_( std::move(other.result_)),
                                    exprs_( std::move(other.exprs_))
    {
        //stmt_.swap(other.stmt_);
        //result_.reset( stmt_->getResultSet() );
    }

    /*result_set& operator= (result_set&& other)
    {
        stmt_   = std::move(other.stmt_);
        result_ = std::move(other.result_);
        exprs_  = std::move(other.exprs_);
        return *this;
    }*/

    void bind() {

      //if (!bound_) {
        typedef fusion::vector<const ExprList&, result_vector&> zip;
        fusion::for_each(fusion::zip_view<zip>(zip(exprs_, results_)),
          results_binder(this->result_));


        //bound_ = true;
      //}
    }

    ~result_set() {

      //SQLCloseCursor(hstmt_);

      //if (Own)
        //SQLFreeHandle(SQL_HANDLE_STMT, hstmt_);
    }

    bool fetch(value_type& row) {        
        if (!result_ || !result_->next()) return false;

        bind();
        fetch2(row);

        return true;
    }

    bool fetch2(value_type& row) {

      // error here indicates that at least one select expression is dynamic
      // this is not supported - explicitly bind results instead
      BOOST_MPL_ASSERT((detail::contains_only_static_expressions<ExprList>));

      //long rc = SQLFetch(hstmt_);

      //if (rc == SQL_NO_DATA) {
      //  return false;
      //}

      //if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
      //  throw odbc_error(SQL_HANDLE_STMT, hstmt_, rc);

      typedef fusion::vector<const result_vector&, typename value_type::value_vector_type&> zip;

      fusion::for_each(fusion::zip_view<zip>(zip(results_, row.values())),
        read_row<value_type>(row));

      return true;
    }

    bool fetch() {
      //long rc = SQLFetch(hstmt_);

      //if (rc == SQL_NO_DATA) {
      //  return false;
      //}

      //if (rc != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
      //  throw odbc_error(SQL_HANDLE_STMT, hstmt_, rc);

      return true;
    }

    Container all() {

      // error here indicates that at least one select expression is dynamic
      // this is not supported - explicitly bind results instead
      BOOST_MPL_ASSERT((detail::contains_only_static_expressions<ExprList>));

      //bind();

      Container results;
      value_type row;

      while (fetch(row)) {
        results.push_back(row);
      }

      return results;
    }

    template<class OtherContainer>
    void all(OtherContainer& results) {
      value_type row;

      while (fetch(row)) {
        results.push_back(row);
      }
    }
  };

  template<class ExprList, bool Own>
  std::ostream& operator <<(std::ostream& os, const result_set<ExprList, Own>& results) {
    const char* sep = "";
    os << "(";
    typename result_set<ExprList, Own>::value_type row;
    while (const_cast<result_set<ExprList, Own>&>(results).fetch(row)) {
      os << sep;
      os << row;
      sep = " ";
    }
    return os << ")";
  }
 
	template <class Table>
	struct  table_result_set {
        using type = typename result_set< typename ::boost::rdb::sql::table_columns<Table>::type, false>::Container;
	};
	
} } }





#endif
