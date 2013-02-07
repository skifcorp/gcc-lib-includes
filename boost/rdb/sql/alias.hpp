#ifndef __ALIAS_HPP_
#define __ALIAS_HPP_

#include <boost/mpl/string.hpp>


namespace boost { namespace rdb { namespace sql {

namespace mpl = boost::mpl;

template <class Expr, class Name>
struct sql_column_alias
{        
    void str(std::ostream & os) const
    {
        os << mpl::c_str< Name >::value;
    }    

    using type = typename Expr::rdb_type;
    using expression = Expr;

    template <class ... Args >
    sql_column_alias(Args ... ) {}
};


template <class ... Expr>
struct unroll_expr;

template <>
struct unroll_expr<>
{
    using type =  void;
};


template <class Expr>
struct unroll_expr<Expr>
{
    using type =  Expr;
};


template <class Expr, class Name>
struct unroll_expr< sql_column_alias<Expr, Name> >
{
    using type = typename unroll_expr<Expr>::type;
};

template<class Name, class ... Expr >
constexpr sql_column_alias< typename unroll_expr<Expr...>::type, Name> alias( const Expr & ... e  )
{ 
    return sql_column_alias< typename unroll_expr<Expr...>::type, Name>(e...);
}

}}}

#endif
