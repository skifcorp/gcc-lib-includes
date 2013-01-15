#ifndef __MYSQL_SELECT_HPP_
#define __MYSQL_SELECT_HPP_

#include <boost/rdb/sql.hpp>
#include <name_of/name_of.h>


#include <boost/mpl/replace_if.hpp>

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/invoke.hpp>
#include <boost/fusion/include/transform.hpp>

#include <type_traits>

namespace boost { namespace rdb { namespace sql {


namespace ct  = boost::rdb::ct;
namespace fusion = boost::fusion;
namespace mpl = boost::mpl;

struct mysql :  sql2003
{
    struct limit;
    struct as;
    struct inner_join;
    struct on;
    struct group_by;
    struct select_all;
};

template <class State, class New>
struct allow<mysql, State, New> : public allow<sql2003, State, New>
{
};

struct select_all_placeholder
{
    typedef fusion::vector<> placeholder_vector;
    placeholder_vector placeholders() const
    {
        return placeholder_vector() ;
    }

};


template <bool isSelectAll>
struct query_str
{
    template <class ThisClass>
    static void str(ThisClass& this_class, std::ostream& os)
    {
        this_class.data_.for_each(str_clause(os));
    }
};


struct str_clase_adapted : public str_clause
{
    str_clase_adapted(std::ostream& o) : str_clause(o){}

    template <class MapEntry>
    void operator()(const MapEntry& e) const
    {
        str_clause::operator ()(e.value);
    }
};

struct select_all_replacer
{
    template <class T>
    struct apply : std::conditional<
                            std::is_same<
                                typename T::Key,
                                mysql::select_all
                            >::value,
                            mpl::true_,
                            mpl::false_
                        >
    {

    };

};

struct correct_map_accumulator
{
    template <class T>
    struct result;

    template <class Self, class Cont, class Arg>
    struct result<Self(const Cont&, Arg)>
    {
        using ArgType = typename std::remove_reference<Arg>::type;
        using type = typename ct::result_of::add_key<typename ArgType::Key, Arg, Cont>::type;
    };

//    template <class Cont, class Arg>
//    typename result<Cont(Arg)>::type operator() (const Cont& cont, Arg arg) const
//    {
//        return ct::add_key<Arg::Key, Arg, Cont>(arg, cont);
//    }
};

template <>
struct query_str<true>
{
    struct empty_func
    {
        template <class T>
        struct result
        {
            using type = T;
        };
    };

    template <class ThisClass>
    static void str(ThisClass& this_class, std::ostream& os)
    {
        using DataMap =  decltype(this_class.data_);

        using TableType = typename std::remove_const <
                            typename std::remove_reference <
                                typename fusion::result_of::at_c<
                                    typename ct::result_of::entry_at_key<DataMap, mysql::from>::type::value_type,
                                    0
                                >::type
                              >::type
                           >::type;

        using TableColumns = typename table_columns<TableType>::type;

        using ExtractedMapData = typename ct::result_of::transform<DataMap, empty_func, ct::map_tag>::type;


        using ReplacedTypes = typename mpl::replace_if<                                            
                                            ExtractedMapData,
                                            select_all_replacer,
                                            ct::map_entry<
                                                mysql::exprs,
                                                TableColumns
                                            >
                                        >::type;

        using CorrectMapType = typename fusion::result_of::accumulate<
                    ReplacedTypes,
                    ct::map0,
                    correct_map_accumulator
                >::type;

    }
};

template <class Statement>
struct select_invoker
{
    Statement & statement_;

    using Self = select_invoker<Statement>;

    template <class T>
    struct result;

    template <class ... Args>
    struct result<Self(Args ...)> {
        using type = typename Statement::template transition<
                    mysql::exprs,
                    fusion::vector<
                        typename std::remove_pointer<
                            typename std::remove_reference<
                                Args
                            >::type
                        >::type ...
                    >
                >::type;
    };

    select_invoker(Statement & s) : statement_(s) {}

    template <class ... Args>
    typename result<Self(Args...)>::type operator()(Args ... args)
    {
        return statement_(*args...);
    }
};

struct deref_pointer
{
    template <class T>
    struct result;

    template <class Self, class Arg>
    struct result < Self(Arg) > {
        using type = typename std::remove_pointer <
                        typename std::remove_reference <
                            Arg
                        >::type
                    >::type;
    };



    template <class Arg>
    typename result<deref_pointer(Arg)>::type operator()(Arg arg) const
    {
        return *arg;
    }
};

template <class State, class Data, class Subdialect>
struct select_statement<mysql, State, Data, Subdialect> : public select_statement<sql2003, State, Data, Subdialect>
{
    using inherited = select_statement<sql2003, State, Data, Subdialect>;
    using Self      = select_statement<mysql, State, Data, Subdialect>;

    select_statement( const Data& d) : inherited(d){}


    template <class Alias>
    auto as ( const Alias& a ) ->decltype (typename inherited::template transition <
                                                mysql::as, Alias
                                                >::type( ct::add_key<mysql::as>(this->data_, a) ))
    {
        //BOOST_MPL_ASSERT((allow<Subdialect, State, typename Subdialect::as>));
        return typename inherited::template transition <
                mysql::as, Alias
                >::type( ct::add_key<mysql::as>(this->data_, a) );
    }

    template <class Table>
    auto inner_join ( const Table& t ) ->decltype(typename inherited::template transition <
                                                  mysql::inner_join, const Table*
                                                  >::type( ct::add_key<mysql::inner_join>(this->data_, &t) ))
    {
        //BOOST_MPL_ASSERT((allow<Subdialect, State, typename Subdialect::as>));
        return typename inherited::template transition <
                mysql::inner_join, const Table*
                >::type( ct::add_key<mysql::inner_join>(this->data_, &t) );
    }

    template <class Statement>
    auto on ( const Statement& st ) -> decltype(typename inherited::template transition <
                                                mysql::on, Statement
                                                >::type( ct::add_key<mysql::on>(this->data_, st) ))
    {
        return typename inherited::template transition <
                mysql::on, Statement
                >::type( ct::add_key<mysql::on>(this->data_, st) );
    }

    template <class Column>
    auto group_by ( const Column& c ) -> decltype(typename inherited::template transition <
                                                mysql::group_by, Column
                                                >::type( ct::add_key<mysql::group_by>(this->data_, c) ))
    {
        return typename inherited::template transition <
                mysql::group_by, Column
                >::type( ct::add_key<mysql::group_by>(this->data_, c) );
    }

/*    typename inherited::template transition<typename Subdialect::select_all, fusion::vector<select_all_placeholder>>::type
    operator()()
    {
        return typename inherited::template
                transition<
                        typename Subdialect::select_all,
                        fusion::vector<select_all_placeholder>
                >::type( ct::map<
                            typename Subdialect::select_all,
                            fusion::vector<select_all_placeholder>
                         >(fusion::vector<select_all_placeholder>(select_all_placeholder())) );
    } */

    template <class ... VectorArgs>
    typename inherited::template transition<
        typename Subdialect::exprs,
        fusion::vector<
            typename std::remove_pointer<
                typename std::remove_reference<
                    VectorArgs
                >::type
            >::type ...
        >
    >::type operator () (const fusion::vector<VectorArgs...>& columns)
    {
        return
            typename inherited::template transition<
                typename Subdialect::exprs,
                fusion::vector<
                    typename std::remove_pointer<
                        typename std::remove_reference<
                            VectorArgs
                        >::type
                    >::type ...
                >
             >::type(
                    ct::map<
                        typename Subdialect::exprs,
                        fusion::vector<
                            typename std::remove_pointer<
                                typename std::remove_reference<
                                    VectorArgs
                                >::type
                            >::type ...
                        >
                    >( boost::fusion::transform(columns, deref_pointer() ) ) );
    }

#if 0
    template <class ... VectorArgs>
    typename fusion::result_of::invoke<
            select_invoker<Self>,
            fusion::vector<VectorArgs...>
    >::type
    operator () (const fusion::vector<VectorArgs...>& columns)
    {
        return fusion::invoke( select_invoker<Self>(*this), columns );
    }
#endif


    using inherited::operator();
};

BOOST_RDB_ALLOW(mysql, mysql::as, mysql::from);
BOOST_RDB_ALLOW(mysql, mysql::from, mysql::inner_join);
BOOST_RDB_ALLOW(mysql, mysql::inner_join, mysql::on);
BOOST_RDB_ALLOW(mysql, mysql::on, mysql::where);
BOOST_RDB_ALLOW(mysql, mysql::select_all, mysql::from);

template <class T>
inline void str(std::ostream& os, const ct::map_entry<mysql::as, T>& p) {
   os << " as ";
   p.value.str(os);
 }

template <class T>
inline void str(std::ostream& os, const ct::map_entry<mysql::inner_join, T>& p) {
   os << " inner join ";
   p.value->str(os);
 }

template <class T>
inline void str(std::ostream& os, const ct::map_entry<mysql::on, T>& p) {
   os << " on ";
   p.value.str(os);
 }


template <class T>
inline void str(std::ostream& os, const ct::map_entry<mysql::group_by, T>& p) {
   os << " group by ";
   p.value.str(os);
 }

using mysql_select_statement_type = select_statement<mysql, mysql::select, ct::map0, mysql>;

} } }

#endif
