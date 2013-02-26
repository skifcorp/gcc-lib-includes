//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#ifndef BOOST_RDB_SQL_COMMON_HPP
#define BOOST_RDB_SQL_COMMON_HPP

#include <sstream>
#include <string>

#include <boost/rdb/sql/detail/comma_output.hpp>

#include <boost/format.hpp>
#include <boost/ref.hpp>
#include <boost/typeof/typeof.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/if.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/static_assert.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_convertible.hpp>

#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/accumulate.hpp>
#include <boost/fusion/include/push_back.hpp>
#include <boost/fusion/include/join.hpp>
#include <boost/fusion/include/replace_if.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/zip_view.hpp>

#include <boost/concept_check.hpp>
#include <boost/concept/requires.hpp>
#include <boost/noncopyable.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#include <boost/rdb/core.hpp>
#include <boost/rdb/detail/static_map.hpp>

#include <boost/rdb/sql/common_output.hpp>

#define BOOST_RDB_PP_WITH(z, n, t) ::template with<t##n>::type
#define BOOST_RDB_PP_CALL(z, n, t) (t##n)
#define BOOST_RDB_PP_EXPRESSION(z, n, t) BOOST_PP_COMMA_IF(n) const expression<T##t##n>& t##n
#define BOOST_RDB_PP_REFERENCE(z, n, t) BOOST_PP_COMMA_IF(n) t##n&
#define BOOST_RDB_MAKE_EXPRESSION(z, n, t) BOOST_PP_COMMA_IF(n) make_expression(t##n)
#define BOOST_RDB_RESULT_OF_MAKE_EXPRESSION(z, n, t) \
  BOOST_PP_COMMA_IF(n) typename result_of::make_expression<this_type, t##n>::type
#define BOOST_PP_RDB_CONCEPT_ASSERT(z, n, t) \
  BOOST_CONCEPT_ASSERT((BOOST_PP_TUPLE_ELEM(2, 0, t)<BOOST_PP_TUPLE_ELEM(2, 1, t) ## n>));
#ifdef _MSC_VER
#pragma message("find a solution for g++")
#define BOOST_PP_RDB_MPL_ASSERT(z, n, t) \
  BOOST_MPL_ASSERT((BOOST_PP_TUPLE_ELEM(2, 0, t)<BOOST_PP_CAT(BOOST_PP_TUPLE_ELEM(2, 1, t), n)>));
#else
#define BOOST_PP_RDB_MPL_ASSERT(z, n, t) \
  ;
#endif



namespace boost { namespace rdb { namespace sql {

  struct base_of_select_statement {};

  namespace detail {
    typedef fusion::list<> empty;
    
    struct none { };
  }

  namespace precedence_level {
    enum level {
      logical_or,
      logical_and,
      logical_not,
      compare,
      add,
      multiply,
      lowest = logical_or,
      highest = multiply
    };
  }
  
  template<class T>
  struct precedence_of {
    BOOST_STATIC_CONSTANT(int, value = T::precedence);
  };
  
  template<typename T, class SqlType>
  struct precedence_of< core::literal<T, SqlType> > {
    BOOST_STATIC_CONSTANT(int, value = precedence_level::highest);
  };
  
  template<class Condition, typename Tag, class Enable = void>
  struct tag_if {
  };
  
  template<class Condition, typename Tag>
  struct tag_if<Condition, Tag, typename enable_if<Condition>::type> {
    typedef Tag tag;
  };

  struct sql2003 {
    struct select;
    struct distinct;
    struct all;
    struct exprs;
    struct from;
    struct where;
    struct insert;
    struct insert_select;
    struct update;
    struct cols;
    struct values;
    struct set;
    struct last_insert_id;
  };
  
  template<class Dialect, class State, class Data, class Subdialect>
  struct select_statement;

  template<class Dialect, class State, class New>
  struct allow : mpl::false_ { };
  
  #define BOOST_RDB_ALLOW(Dialect, State, New) \
    template<> struct allow<Dialect, Dialect::State, Dialect::New> : mpl::true_ { }
  
  BOOST_RDB_ALLOW(sql2003, select, exprs);
  BOOST_RDB_ALLOW(sql2003, exprs, from);
  BOOST_RDB_ALLOW(sql2003, from, where);

  struct assign_output : detail::comma_output {
    assign_output(std::ostream& os) : comma_output(os) { }

    template<typename First, typename Second>
    void operator ()(const fusion::vector<First, Second>& p) const {
      std::ostream& os = this->item();
      using namespace fusion;
      at_c<0>(p).str(os);
      os_ << " = ";
      at_c<1>(p).str(os);
    }
  };

  template<class ExprList>
  void str(std::ostream& os, const ct::map_entry<sql2003::exprs, ExprList>& p) {
    os << " ";
    fusion::for_each(p.value, detail::comma_output(os));
  }

  inline void str(std::ostream& os, const ct::map_entry<sql2003::distinct, int>& ) {
    os << " distinct";
  }

  inline void str(std::ostream& os, const ct::map_entry<sql2003::all, int>& ) {
    os << " all";
  }



  struct print_nothing
  {
    static void print(std::ostream & )
    {

    }
  };

  template <char symbol>
  struct print_symbol
  {
    static void print(std::ostream & os)
    {
       os << symbol;
    }
  };

  template<class TableList>
  void str(std::ostream& os, const ct::map_entry<sql2003::from, TableList>& p) {
      constexpr bool need_braces = boost::is_convertible < typename mpl::at<TableList, mpl::int_<0>>::type, base_of_select_statement >::value;

      using left_brace  = typename mpl::if_<mpl::bool_<need_braces>, print_symbol<'('>, print_nothing >::type;
      using right_brace = typename mpl::if_<mpl::bool_<need_braces>, print_symbol<')'>, print_nothing >::type;

      os << " from ";
      left_brace::print(os);

      fusion::for_each(p.value, detail::comma_output(os));

      right_brace::print(os);
  }

  template<class Predicate>
  void str(std::ostream& os, const ct::map_entry<sql2003::where, Predicate>& p) {
    os << " where ";
    p.value.str(os);
  }

  template<class St>
  struct Statement
  {
    const St& st;
    std::ostream& stream;

    BOOST_CONCEPT_USAGE(Statement) {
      st.str(stream);
      st.placeholders();
    }
  };

  template<class St>
  struct SelectStatement : Statement<St> {
    BOOST_CONCEPT_USAGE(SelectStatement) {
    }
  };

  struct comparison {
    typedef core::boolean rdb_type;
    BOOST_STATIC_CONSTANT(int, precedence = precedence_level::compare);
  };

  template<class Statement>
  std::string as_string(const Statement& statement) {
    std::ostringstream os;
    statement.str(os);
    return os.str();
  }

  struct extract_sql_kind {

    template<typename Sig>
    struct result;

    template<typename Self, typename Expr>
    struct result<Self(Expr)> {
      typedef typename boost::remove_reference<Expr>::type::rdb_type type;
    };
  };

  struct str_clause {
    str_clause(std::ostream& os) : os_(os) { }
    std::ostream& os_;

    template<typename Clause>
    void operator ()(const Clause& clause) const {
      str(os_, clause);
    }
  };

  template<class Expr1, class Expr2>
  struct is_sql_compatible : mpl::or_<
    is_same<
      typename remove_reference<Expr1>::type::rdb_type::kind,
      typename remove_reference<Expr2>::type::rdb_type::kind
    >,
    is_same<typename remove_reference<Expr1>::type::rdb_type::kind, core::universal>,
    is_same<typename remove_reference<Expr2>::type::rdb_type::kind, core::universal>
  > {
  };

  template<class Iter1, class Last, class Iter2, class End>
  struct _sql_lists_compatible : mpl::and_<
    is_sql_compatible<
      typename fusion::result_of::deref<Iter1>::type,
      typename fusion::result_of::deref<Iter2>::type
    >,
    _sql_lists_compatible<
      typename fusion::result_of::next<Iter1>::type,
      Last,
      typename fusion::result_of::next<Iter2>::type,
      typename fusion::result_of::equal_to<
        typename fusion::result_of::next<Iter1>::type,
        Last
      >::type
    >
  > {
  };

  //template<class Iter1, class Last, class Iter2>
  //struct _sql_lists_compatible<Iter1, Last, Iter2, 
  //  typename fusion::result_of::equal_to<Last, Last>::type
  //> : mpl::true_ {
  //};

  //template<class ExprList1, class ExprList2>
  //struct sql_lists_compatible : _sql_lists_compatible<
  //  typename fusion::result_of::begin<ExprList1>::type,
  //  typename fusion::result_of::end<ExprList1>::type,
  //  typename fusion::result_of::begin<ExprList2>::type,
  //  typename fusion::result_of::equal_to<
  //    typename fusion::result_of::begin<ExprList1>::type,
  //    typename fusion::result_of::end<ExprList1>::type
  //  >::type
  //> {
  //};

  // This is /not/ the placeholder type. It's just the marks' type, i.e.
  // the type of `_`, `_1`, etc. Placeholders are typed things, marks are not.
  // The actual type of the placeholder is determined from the context in which
  // it is used.
  template<int N>
  struct placeholder_mark {
    typedef core::placeholder_type rdb_type;
    typedef fusion::vector<> placeholder_vector; // not really used; exists to please mpl::if_ which is not lazy
    placeholder_vector placeholders() const { return fusion::make_vector(); }
    BOOST_STATIC_CONSTANT(int, precedence = precedence_level::highest);
    void str(std::ostream& os) const {
      os << "?";
    }
  };
  
  template<class Col, class Expr>
  struct set_clause {
    set_clause(const Col& col, const Expr& expr) : col_(col), expr_(expr) {
      BOOST_MPL_ASSERT((is_sql_compatible<Col, Expr>));
    }
    
    Col col_;
    Expr expr_;
    
    void str(std::ostream& os) const {
      col_.str(os);
      os << " = ";
      expr_.str(os);
    }
    
    template<class Expr2>
    struct placeholders_for {
      typedef typename Expr2::placeholder_vector placeholder_vector;
      static placeholder_vector make(const set_clause& update) {
        return update.expr_.placeholders();
      }
    };
    
    template<int N>
    struct placeholders_for< placeholder_mark<N> > {
      typedef fusion::vector< core::placeholder<typename Col::rdb_type> > placeholder_vector;
      static placeholder_vector make(const set_clause& update) {
        return fusion::make_vector(core::placeholder<typename Col::rdb_type>());
      }
    };
    
    typedef typename placeholders_for<Expr>::placeholder_vector placeholder_vector;
    
    placeholder_vector placeholders() const {
      return placeholders_for<Expr>::make(*this);
    }
  };
  
  template<class T>
  struct is_update_container : mpl::false_ {
  };
  
  template<class Col, class Expr>
  struct is_update_container< set_clause<Col, Expr> > : mpl::true_ {
  };

  struct no_tag;
  
  template<class T>
  struct tag_of {
    typedef no_tag type;
  };
  
  template<class T>
  struct is_column_container : mpl::false_ {
  };
  
} } }

#endif // BOOST_RDB_SQL_COMMON_HPP
