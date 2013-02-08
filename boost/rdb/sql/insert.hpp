//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#ifndef BOOST_RDB_INSERT_HPP
#define BOOST_RDB_INSERT_HPP

#include <boost/rdb/sql/common.hpp>
#include <boost/rdb/sql/expression.hpp>

namespace boost { namespace rdb { namespace sql {

  template<class Table>
  inline void str(std::ostream& os, const ct::map_entry<sql2003::insert, const Table*>& p) {
    os << "insert into ";
    p.value->str(os);
  }

  template<class ColList>
  inline void str(std::ostream& os, const ct::map_entry<sql2003::cols, ColList>& p) {
    os << " (";
    fusion::for_each(p.value, detail::comma_output(os));
    os << ")";
  }

  template<class ValueList>
  inline void str(std::ostream& os, const ct::map_entry<sql2003::values, ValueList>& p) {
    os << " values (";
    fusion::for_each(p.value, detail::comma_output(os));
    os << ")";
  }

  template<class ExprList>
  inline void str(std::ostream& os, const ct::map_entry<sql2003::insert_select, ExprList>& p) {
    os << " select ";
    fusion::for_each(p.value, detail::comma_output(os));
  }

    template <class Lit>
    inline void str(std::ostream& os, const ct::map_entry<sql2003::last_insert_id, fusion::vector<Lit> >& ) {
        os << "; select last_insert_id() ";
    }


  template<class Data, class HasValues, class HasSelect, class Subdialect, class Tag >
  struct insert_impl {
    insert_impl(const Data& data) : data_(data) { }
    Data data_;
  };
  
  namespace result_of {

    // T = expression or expression list
    // U = something that may contain one or several placeholder marks
    // returns a typed placeholder or a vector of dynamic placeholders
    //template<class T, class U>
    //struct make_placeholder_from_mark;

    //template<class Expr, int N>
    //struct make_placeholder_from_mark< expression<Expr>, expression< placeholder_mark<N> > > {
    //  typedef typename fusion::result_of::vector< core::placeholder<typename Expr::rdb_type> >::type type;
    //};

    //template<class Expr1, class Expr2>
    //struct make_placeholder_from_mark< expression<Expr>, expression< placeholder_mark<N> > > {
    //  typedef fusion::vector<> type;
    //};
  }

  struct extract_insert_values_placeholders {

    template<typename Sig>
    struct result;

    template<class Self, class Col, class Expr, class Placeholders>
    struct result<Self(Placeholders&, fusion::vector2<const Col&, const Expr&> )> {
      typedef typename fusion::result_of::as_vector<
        typename fusion::result_of::join<
          const Placeholders,
          const typename detail::binary_operation_placeholders<Col, Expr>::type
        >::type
      >::type type;
    };

    template<class Col, class Expr, class Placeholders>
    typename result<extract_insert_values_placeholders(Placeholders&, fusion::vector2<const Col&, const Expr&>)>::type
    operator ()(const Placeholders& placeholders, const fusion::vector<const Col&, const Expr&> zip ) const {
      using namespace fusion;
      //BOOST_AUTO(result, as_vector(join(placeholders,
      //  detail::binary_operation_placeholders<Col, Expr>::make(at_c<0>(zip), at_c<1>(zip)))));
      return as_vector(join(placeholders,
        detail::binary_operation_placeholders<Col, Expr>::make(at_c<0>(zip), at_c<1>(zip))));
    }
  };

  template<class Data, class Subdialect, class Tag>
  struct insert_impl<Data, mpl::true_, mpl::false_, Subdialect, Tag> {
    //typedef core::insert_statement_tag tag;
      using tag = Tag;
    typedef typename ct::result_of::value_at_key<Data, typename Subdialect::cols>::type cols_type;
    typedef typename ct::result_of::value_at_key<Data, typename Subdialect::values>::type values_type;

    insert_impl(const Data& data) : data_(data) { }
    typedef insert_impl inherited;

    Data data_;

    const cols_type& cols() const { return ct::at_key<typename Subdialect::cols>(data_); }
    const values_type& values() const { return ct::at_key<typename Subdialect::values>(data_); }

    typedef fusion::vector<const cols_type&, const values_type&> zip;
    typedef fusion::zip_view<zip> zip_view;

    typedef typename fusion::result_of::as_vector<
      typename fusion::result_of::accumulate<zip_view, fusion::vector<>, extract_insert_values_placeholders>::type
    >::type placeholder_vector;

    placeholder_vector placeholders() const {
      using namespace fusion;
      //BOOST_AUTO(result, as_vector(accumulate(zip_view(zip(cols(), values())),
      //  make_vector(), extract_insert_values_placeholders())));
      return as_vector(
        accumulate(zip_view(zip(cols(), values())),
          make_vector(), extract_insert_values_placeholders()));
    }

  };

  template<class Data, class Subdialect, class Tag>
  struct insert_impl<Data, mpl::false_, mpl::true_, Subdialect, Tag> {
    //typedef core::insert_statement_tag tag;
      using tag = Tag;
    typedef typename result_of::placeholders_from_map<Data>::type placeholder_vector;
    insert_impl(const Data& data) : data_(data) { }
    Data data_;
    typedef insert_impl inherited;
  };

  template<class Dialect, class State, class Data, class Subdialect, class Tag>
  struct insert_statement : insert_impl<Data,
    typename ct::result_of::has_key<Data, typename Subdialect::values>::type,
    typename ct::result_of::has_key<Data, typename Subdialect::insert_select>::type,
    Subdialect, Tag
  > {



    explicit insert_statement(const Data& data) : insert_impl<Data,
      typename ct::result_of::has_key<Data, typename Subdialect::values>::type,
      typename ct::result_of::has_key<Data, typename Subdialect::insert_select>::type,
      Subdialect, Tag
    >(data) { }

    typedef void result;

    template<class K, class T, class TrTag = Tag, class D = Data>
    struct transition {
      typedef insert_statement<
        Subdialect,
        K,
        ct::map<K, T, D>,
        Subdialect,
        TrTag
      > type;
    };

    using last_insert_id_expr = expression< core::literal<int, core::integer> >;

    typename transition <
        typename Subdialect::last_insert_id,
        fusion::vector < last_insert_id_expr >,
        core::insert_tabular_statement_tag
    >::type select_last_insert_id() const
    {


        return typename transition <
            typename Subdialect::last_insert_id,
            fusion::vector<last_insert_id_expr>,
            core::insert_tabular_statement_tag
        >::type(
            ct::add_key<typename Subdialect::last_insert_id>(
                        this->data_, fusion::vector<last_insert_id_expr>( last_insert_id_expr(0) ) ) );
    }

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/insert_cols.hpp>
    #include BOOST_PP_ITERATE()

    template<class ColIter, class ColLast, class ExprIter>
    struct value_list;

    template<class ColLast, class ExprIter>
    struct value_list<ColLast, ColLast, ExprIter> {
      typedef fusion::result_of::make_vector<>::type type;

      template<class T>
      static const type make(const T&) {
          return fusion::make_vector();
      }
    };  

    template<class ColIter, class ColLast, class ExprIter>
    struct value_list {

      typedef typename remove_reference<
        typename fusion::result_of::deref<ColIter>::type
      >::type col_type;

      typedef value_list<
        typename fusion::result_of::next<ColIter>::type,
        ColLast,
        typename fusion::result_of::next<ExprIter>::type
      > next;

      typedef typename fusion::result_of::push_front<
        const typename next::type,
        typename result_of::make_expression<
          col_type,
          typename remove_reference<
            typename fusion::result_of::deref<ExprIter>::type
          >::type
        >::type
      >::type type;

      template<class T>
      static const type make(const T& iter) { // why doesn't ExprIter work ?
        return fusion::push_front(
          next::make(fusion::next(iter)),
          expression<col_type>::make_expression(*iter)
          );
      }
    };  

    template<class Exprs>
    struct with_values {

      typedef typename ct::result_of::value_at_key<Data, typename Subdialect::cols>::type cols;
      typedef typename fusion::result_of::end<cols>::type col_last;

      // If this assertion fails the insert list and the value list have different sizes
      BOOST_MPL_ASSERT((mpl::equal_to<
        fusion::result_of::size<cols>,
        fusion::result_of::size<Exprs> >));
      //BOOST_MPL_ASSERT((is_sql_compatible<cols, Exprs>));

      typedef value_list<
        typename fusion::result_of::begin<cols>::type,
        typename fusion::result_of::end<cols>::type,
        typename fusion::result_of::begin<Exprs>::type
      > final_value_list;

      typedef typename transition<
        typename Subdialect::values,
        typename fusion::result_of::as_vector<
          typename final_value_list::type
        >::type
      >::type type;

      static type make(const Data& data, const Exprs& exprs) {
        return type(ct::add_key<typename Subdialect::values>(
          data, fusion::as_vector(final_value_list::make(fusion::begin(exprs)))));
      }


    };

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/insert_values.hpp>
    #include BOOST_PP_ITERATE()

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/insert_select.hpp>
    #include BOOST_PP_ITERATE()

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/select_distinct.hpp>
    #include BOOST_PP_ITERATE()

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/select_all.hpp>
    #include BOOST_PP_ITERATE()

    #define BOOST_PP_ITERATION_LIMITS (1, BOOST_RDB_MAX_SIZE - 1)
    #define BOOST_PP_FILENAME_1       <boost/rdb/sql/detail/select_from.hpp>
    #include BOOST_PP_ITERATE()
    
    #include <boost/rdb/sql/detail/select_where.hpp>

    void str(std::ostream& os) const {
      ct::for_each(this->data_, str_clause(os));
      //this->data_.for_each(str_clause(os));
    }

  };
  
  template<class Dialect, class State, class Data, class Subdialect>
  typename ct::result_of::value_at_key<Data, sql::sql2003::last_insert_id>::type data_expr(
            const insert_statement<Dialect, State, Data, Subdialect, core::insert_tabular_statement_tag >& ins
          )
  {
      return ct::at_key<sql::sql2003::last_insert_id>( ins.data_ );
  }

  BOOST_RDB_ALLOW(sql2003, cols, insert_select);
  BOOST_RDB_ALLOW(sql2003, insert_select, from);
  BOOST_RDB_ALLOW(sql2003, insert_select, distinct);
  BOOST_RDB_ALLOW(sql2003, insert_select, all);

  template<class Table>
  insert_statement<
    sql2003,
    sql2003::insert,
    ct::map<sql2003::insert, const Table*>,
    sql2003,
    core::insert_statement_tag
  >
  insert_into(const Table& table) {
    return insert_statement<
      sql2003,
      sql2003::insert,
      ct::map<sql2003::insert, const Table*>,
      sql2003,
      core::insert_statement_tag
    >(ct::map<sql2003::insert, const Table*>(&table));
  }

} } }



namespace boost { namespace rdb { namespace core {

  template<class Dialect, class State, class Data, class Subdialect>
  struct statement_result_type< sql::insert_statement<Dialect, State, Data, Subdialect, core::insert_tabular_statement_tag> > {
    typedef typename ct::result_of::value_at_key<Data, sql::sql2003::last_insert_id>::type type;
  };

} } }


#endif
