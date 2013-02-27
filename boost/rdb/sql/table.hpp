//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#ifndef BOOST_RDB_TABLE_HPP
#define BOOST_RDB_TABLE_HPP

#include <boost/rdb/sql/common.hpp>
#include <boost/rdb/sql/expression.hpp>

#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/as_vector.hpp>

#include <boost/rdb/detail/static_map.hpp>

#include <fusion_tools/as_vvector.h>

#include <type_traits>

namespace boost { namespace rdb { namespace sql {
    namespace mpl = boost::mpl;
    namespace fusion = boost::fusion;
    namespace ct = ::boost::rdb::ct;

  template<class Table, class SqlType, class Base>
  struct column : Base {
    enum { precedence = precedence_level::highest };
    typedef SqlType rdb_type;
    static void str_type(std::ostream& os) { SqlType::str(os); }
    
    template<class T>
    //set_clause<column, typename result_of::make_expression<column, T>::type>
    set_clause
        <   column,
            typename boost::remove_const
            <
                typename boost::remove_reference
                <
                    typename core::make_literal<SqlType, T>::type
                >::type
            >::type
        >
    operator =(const T& expr) const {
      return set_clause<column, typename result_of::make_expression<column, T>::type>(
        *this, expression<column>::make_expression(expr));
    }
    
    template<class Expr>
    set_clause<column, Expr>
    operator =(const expression<Expr>& expr) const {
      return set_clause<column, Expr>(*this, expr.unwrap());
    }
    
    template<class T>
    set_clause<column, typename result_of::make_expression<column, T>::type>
    operator <<(const T& expr) const {
      return set_clause<column, typename result_of::make_expression<column, T>::type>(
        *this, expression<column>::make_expression(expr));
    }

    void str(std::ostream& os) const {
      if (this->table_->has_alias())
        os << this->table_->alias() << '.' << Base::name();
      else
        os << Base::name();
    }
  };
  
  template<class Table, class SqlType, class Base>
  struct is_column_container< column<Table, SqlType, Base> > : mpl::true_ {
  };

  template<class Col>
  struct ColumnContainer
  {
    Col col;
    std::ostream& os;

    BOOST_CONCEPT_USAGE(ColumnContainer) {
      BOOST_MPL_ASSERT((is_column_container<Col>));
      col.str(os);
    }
  };

  template<class T, class Enable = void>
  struct is_table_container : mpl::false_ {
  };

  template<class T>
  struct is_table_container<T, typename T::table_container_tag> : mpl::true_ {
  };

  namespace detail {

    struct any_table : boost::noncopyable {
      any_table() { }
      any_table(const std::string& alias) : alias_(alias) { }
      any_table(const any_table& ) = delete;
      std::string alias_;
      const std::string& alias() const { return alias_; }
      bool has_alias() const { return !alias_.empty(); }
      typedef fusion::vector<> placeholder_vector;
      placeholder_vector placeholders() const { return fusion::make_vector(); }
      typedef void table_container_tag;
    };
    
    struct any_column /*: boost::noncopyable*/ {
      const any_table* table_;
      BOOST_STATIC_CONSTANT(int, precedence = precedence_level::highest);

      void initialize(const any_table* table) {
        table_ = table;
      }

      typedef fusion::vector<> placeholder_vector;
      placeholder_vector placeholders() const { return fusion::make_vector(); }
    };
    
    template<class Table>
    struct initialize_columns {
      initialize_columns(Table* pt) : pt(pt) { }
      template<typename T> void operator ()(T) {
        T::initialize(pt);
      }
      Table* pt;
    };
  
    struct detect_column_type_from_column_member
    {
        template <class T>
        struct apply {
            using type =  typename std::add_pointer<
                              typename std::remove_reference<
                                  typename T::column_type
                              >::type
                          >::type;
        };
    };

    template<typename T>
    struct singleton {
      static T _;
    };

    template<class T>
    T singleton<T>::_;
    

    template<class Base, bool IsSelfQualified>
    struct table_;


    template<class Base>
    struct table_<Base, false> : Base, any_table {
        table_() { }

        table_(const std::string& alias) : any_table(alias) { }

        // following function name chosen because it won't conflict with column names :-P
        static const char* table() { return Base::name(); }

        void str(std::ostream& os) const {
            if (has_alias())
                os << alias_;
            else
                os << Base::name();
        }
    };





    template<class Base>
    struct table_<Base, true> : Base, any_table {

        table_() : any_table(Base::name()) { }
      
        void str(std::ostream& os) const {
            if (has_alias())
                os << alias_;
            else
                os << Base::name();
        }
    };
  }
  
  struct table_keys
  {
      struct as;
  };

  inline void str(std::ostream& os, const ct::map_entry<table_keys::as, std::string>& as_name)
  {
      os << " as " << as_name.value;
  }

  #define BOOST_RDB_BEGIN_TABLE(NAME)  \
  struct NAME##_base { static const char* name() { return #NAME; } }; \
  template<int Alias, class Data = ::boost::rdb::ct::map0 > \
  struct NAME##_ : ::boost::rdb::sql::detail::table_<NAME##_base, Alias == -1>, ::boost::rdb::sql::detail::singleton< NAME##_<Alias> > {  \
    using BaseTableClass = ::boost::rdb::sql::detail::table_<NAME##_base, Alias == -1>; \
    typedef NAME##_<Alias, Data> this_table;  \
    typedef NAME##_<1> _1; typedef NAME##_<2> _2; typedef NAME##_<3> _3;  \
    NAME##_(const Data& d = ::boost::rdb::ct::map0()):data_(d) { initialize(); }  \
    NAME##_(const std::string& alias) : ::boost::rdb::sql::detail::table_<NAME##_base, Alias == -1>(alias) { initialize(); }  \
    NAME##_(const this_table& other):BaseTableClass(), data_(other.data_) { initialize(); }  \
    typedef NAME##_<-1> qualified;  \
    typedef ::boost::mpl::vector0<>

  #define BOOST_RDB_END_TABLE(NAME)  \
    column_members; \
    using column_pointers_type = typename ::tools::result_of::as_vvector2< \
                                        typename mpl::transform< \
                                            column_members, \
                                            ::boost::rdb::sql::detail::detect_column_type_from_column_member \
                                        >::type \
                                    >::type ; \
    column_pointers_type all; \
    void initialize() { \
      ::boost::mpl::for_each<typename this_table::column_members>(::boost::rdb::sql::detail::initialize_columns<this_table>(this)); \
    } \
    void str(std::ostream& os) const { \
        os << (BaseTableClass::has_alias() ? BaseTableClass::alias_ : #NAME); \
        data_.for_each(::boost::rdb::sql::str_clause(os)); \
    } \
    template <class Key, class NewData> \
    struct transition { \
        using type = NAME##_<0, \
            ::boost::rdb::ct::map<Key, NewData> \
        >; \
    }; \
    Data data_; \
    typename transition< ::boost::rdb::sql::table_keys::as, std::string>::type as(const std::string& s) \
    { \
        return typename transition< ::boost::rdb::sql::table_keys::as, std::string>::type(  ::boost::rdb::ct::map< \
                ::boost::rdb::sql::table_keys::as, std::string>( s,  this->data_ ) ); \
    } \
  }; \
  typedef NAME##_<0> NAME;

  #define BOOST_RDB_COLUMN(NAME, rdb_type) \
  members_before_##NAME;  \
  enum { NAME##_index = boost::mpl::size<members_before_##NAME>::value }; \
  struct NAME##_base : ::boost::rdb::sql::detail::any_column { static const char* name() { return #NAME; } }; \
  typedef ::boost::rdb::sql::expression< ::boost::rdb::sql::column<this_table, ::boost::rdb::core::rdb_type, NAME##_base> > NAME##_type;  \
  NAME##_type NAME;  \
  struct NAME##_member {  \
    using column_type = NAME##_type; \
    typedef std::string type;  \
    static NAME##_type& ref(this_table& obj) { return obj.NAME; }  \
    static const NAME##_type& ref(const this_table& obj) { return obj.NAME; }  \
    static void initialize(this_table* table) \
    { \
        table->NAME.initialize(table); \
        fusion::at_c<NAME##_index>(table->all) = &table->NAME; \
    }  \
  };  \
  typedef typename ::boost::mpl::push_back<members_before_##NAME, NAME##_member>::type

  template<typename Table>
  struct table_column_output : detail::comma_output {
    table_column_output(std::ostream& os, const Table& table) : comma_output(os), table_(table) { }
    
    template<typename Column> void operator ()(Column) {
      std::ostream& os = item();
      os << Column::ref(table_).name() << " ";
      Column::ref(table_).str_type(os);
    }
    
    const Table& table_;
  };

  struct create_table_statement_tag : core::statement_tag { };

  template<typename Table>
  struct create_table_statement {
    typedef create_table_statement_tag tag;
    typedef void result;
    void str(std::ostream& os) const {
      //os << "create table if not exists " << Table::table() << "(";
        std::stringstream sss;
        table.str(sss);
        std::cout << "create table!!! " << sss.str() << std::endl;

        os << "create table if not exists ";
        table.str(os);
        os<< " (";
      boost::mpl::for_each<typename Table::column_members>(table_column_output<Table>(os, Table::_));
      os << ")";
    }
    std::string str() const { return as_string(*this); }

    create_table_statement(const Table& t ):table(t) {}
    const Table & table;
  };

  template<class Table>
  create_table_statement<Table> create_table(const Table& t) {
    return create_table_statement<Table>(t);
  }

  struct drop_table_statement_tag : core::statement_tag { };

  template<typename Table>
  struct drop_table_statement {
    typedef drop_table_statement_tag tag;
    typedef void result;
    void str(std::ostream& os) const {
      os << "drop table " << Table::table();
    }
  };

  template<class Table>
  drop_table_statement<Table> drop_table(const Table&) {
    return drop_table_statement<Table>();
  }

  
   template <class ThisTable>
    struct columns_type_extractor
    {
        using Self = columns_type_extractor;

        template <class T>
        struct result;

        template <class Placeholder, class Vector>
        struct result<Self( const Vector&, Placeholder )>
        {
            using col_type = typename std::remove_const<
                    typename std::remove_reference<
                        decltype(Placeholder::ref(ThisTable()))
                    >::type
            > ::type ;

            using seq_type = typename fusion::result_of::push_back<Vector, col_type>::type;

            using type = typename fusion::result_of::as_vector<seq_type>::type;
        };
    };

    template <class Table>
    using table_columns = typename fusion::result_of::accumulate<
            typename Table::column_members,
            fusion::result_of::make_vector<>::type,
            columns_type_extractor<typename Table::this_table>
    >;  
} } }

#endif
