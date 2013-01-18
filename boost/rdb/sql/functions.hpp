#ifndef __FUNCTIONS_H_
#define __FUNCTIONS_H_

#include <boost/mpl/string.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/bool.hpp>

namespace mpl = boost::mpl;

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/algorithm/iteration.hpp>
#include <boost/fusion/include/accumulate.hpp>

namespace fusion = boost::fusion;

#include <sstream>
using std::ostream;

#include "boost/rdb/sql/expression.hpp"
#include "boost/rdb/core_traits.hpp"
#include "boost/rdb/sql/table.hpp"
#include "boost/rdb/sql/alias.hpp"

#include "mpllibs/metaparse/string.hpp"

#include "name_of/name_of.h"


#define _S(X) MPLLIBS_STRING(X)

namespace boost { namespace rdb { namespace sql {

template<class , class,  class ...>
class sql_function;

template <class T>
struct contains_str_method
{
    template <class T2 = T>
    static auto func( T2 * t ) -> decltype( (*t).str( *static_cast<std::ostream *>(nullptr)  ), mpl::true_() );
    static mpl::false_ func( ... );

    static constexpr bool value = boost::is_same<  decltype( func(static_cast<T*>(nullptr)) ), mpl::true_ >::value;
};

template <bool contains>
struct adapt_str_method
{
    template <class T>
    static void str(T t, std::ostream &os)
    {
        t.str(os);
    }
};

template <>
struct adapt_str_method<false>
{
    template <class T>
    static void str(T t, std::ostream &os)
    {
        os << t;
    }
};

namespace {
    struct empty_struct;

    template <class func_name, class fake_class = empty_struct>
    struct lambda
    {
        template <class T>
        void operator (  ) ( T t) const
        {
            if (!first)
                str<<", ";

            adapt_str_method< contains_str_method<T>::value >::str(t, str);

            first = false;
        }

        ostream& str;
        lambda ( ostream& s ) : str(s)
        {
            str<<mpl::c_str<func_name>::value<<"(";
        }
        ~lambda()
        {
            str<<") ";
        }
        mutable bool first = true;
    };


    struct empty_struct2;

    template<>
    struct lambda<_S("count") >  : public lambda<_S("count"), empty_struct2>
    {
        using inherited = lambda<_S("count"), empty_struct2>;

        lambda<_S("count")> ( ostream& s ) :inherited(s)
        {

        }

        ~lambda<_S("count")>()
        {
            if (inherited::first)
                str<<"*";
        }
    };
}

template<int ... name, class SqlType, class ... Args>
class sql_function< mpl::string<name...>, SqlType, Args...>
{
public:
    using rdb_type = SqlType;

    using FuncName = mpl::string<name...>;



	

    void str(std::ostream& os) const {
        fusion::for_each(args, lambda<FuncName>{os});
    }

    fusion::vector<Args...> args;

    typedef fusion::vector<> placeholder_vector;
    placeholder_vector placeholders() const
    {
        return placeholder_vector() ;
    }

    static const auto precedence = precedence_level::highest;


    /*sql_function(int a = 0)
    {

    }*/


    sql_function(Args... a) : args(a...)
    {

    }
    sql_function& operator= (const sql_function& other)
    {
        args = other.args; return *this;
    }
};



template <class T>
struct impossible_numeric_type;

template <class T>
struct column_type_converter : mpl::if_<
                                boost::is_same< T, int >,
                                core::integer,
                                typename mpl::if_<
                                        mpl::or_<boost::is_same< T, float >, boost::is_same< T, double > > ,
                                        core::float_,
                                        impossible_numeric_type<T>
                                >::type
                             >

{

};

template <class T>
struct column_type_converter< expression< T > >
{
    using type = typename expression< T >::rdb_type;
};

template <class Expr, class Name>
struct column_type_converter< sql_column_alias<Expr, Name> >
{
    using type = typename sql_column_alias<Expr, Name>::type;
};

struct numeric_type_detector
{
    template <class T>
    struct result;

    template <class State, class Current>
    struct result< numeric_type_detector( const State&, Current& ) >
    {
        using current_type = typename column_type_converter<Current>::type;

        using type = typename mpl::if_<
                                         boost::is_same<State, core::float_>,
                                         core::float_,
                                         current_type
                                      >::type;
    };
};

template <class ... T>
struct detect_numeric_type
{
    using type = typename fusion::result_of::accumulate < typename fusion::result_of::make_vector<T ...>::type,
            core::integer, numeric_type_detector >::type;
};

template <class ... Expr>
using sql_function_max = expression< sql_function<_S("max"), typename detect_numeric_type<Expr ...>::type , Expr...> >;

template <class ... Expr>
sql_function_max<Expr...> max(Expr ... e)
{
    return sql_function_max<Expr...>(e...);
}

template <class ... Expr>
using sql_function_min = expression< sql_function<_S("min"), typename detect_numeric_type<Expr ...>::type, Expr...> >;

template <class ... Expr>
sql_function_min<Expr...> min(Expr ... e)
{
    return sql_function_min<Expr...>(e...);
}

template <class ... Expr>
using sql_function_sum = expression< sql_function<_S("sum"), typename detect_numeric_type<Expr ...>::type, Expr...> >;

template <class ... Expr>
sql_function_sum<Expr...> sum(Expr ... e)
{
    return sql_function_sum<Expr...>(e...);
}



template <class ... Expr>
using sql_function_count = expression< sql_function<_S("count"), core::integer, Expr...> >;

template <class ... Expr>
sql_function_count<Expr...> count(Expr ... e)
{
    return sql_function_count<Expr...>(e...);
}



template <class ... Expr>
using sql_function_floor = expression< sql_function<_S("floor"), typename detect_numeric_type<Expr ...>::type, Expr...> >;

template <class ... Expr>
sql_function_floor<Expr...> floor(Expr ... e)
{
    return sql_function_floor<Expr...>(e...);
}

template <class ... Expr>
using sql_function_avg = expression< sql_function<_S("avg"), typename detect_numeric_type<Expr ...>::type, Expr...> >;

template <class ... Expr>
sql_function_avg<Expr...> avg(Expr ... e)
{
    return sql_function_avg<Expr...>(e...);
}


#undef _S

}}}
#endif
