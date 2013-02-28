#ifndef NULLABLE_H
#define NULLABLE_H

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

#include <bitset>

#include <boost/rdb/sql/alias.hpp>
#include <boost/rdb/sql/common.hpp>
#include <boost/rdb/core/datetime.hpp>
#include <boost/rdb/sql/expression.hpp>

namespace boost { namespace rdb { namespace mysql {

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

    template <bool IsCopy>
    struct constr_disp;


    template <>
    struct constr_disp<true>
    {
        template <class N, class ... Args>
        static void call( N& n, Args ... args )
        {
            n.copyValues(std::forward<Args>(args)...);
        }
    };

    template <>
    struct constr_disp<false>
    {
        template <class N, class ... Args>
        static void call( N& n, Args ... args )
        {
            n.makeValues(std::forward<Args>(args)...);
        }
    };


template<class Seq, class ExprList>
struct nullable {
    using expr_list = ExprList;

    nullable()
    {}

    template <class Arg, class ... Args>
    nullable( Arg arg, Args&& ...  args )
    {
        constr_disp< std::is_same<typename std::remove_reference<Arg>::type,
                nullable<Seq, ExprList> >::value >::call(*this, arg, args...);
    }

      //template <class SeqCpy, class ExprListCpy>

    template <class ... Args>
    void makeValues(Args && ... args)
    {
        values_ = fusion::make_vector( std::forward<Args>(args)...);
    }


    void copyValues(const nullable<Seq, ExprList>& other)
    {
        values_ = other.values_;
    }


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


}}}
	

#endif
