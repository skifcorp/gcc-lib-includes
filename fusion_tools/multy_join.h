#ifndef __MULTY_JOIN_H_
#define __MULTY_JOIN_H_

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/push_back.hpp>
#include <boost/fusion/include/push_front.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/join.hpp>

#include <fusion_tools/as_vvector.h>

namespace tools { namespace result_of {



template <class Arg1, class Arg2>
struct multy_joiner_imp
{
    using type = fusion::vector<Arg1, Arg2>;

    static type imp( const Arg1& arg1, const Arg2& arg2 )
    {
        return fusion::make_vector(arg1, arg2);
    }
};

template <class ... Args1, class ... Args2>
struct multy_joiner_imp<fusion::vector<Args1...>, fusion::vector<Args2...>>
{
    using type = typename tools::result_of::as_vvector2<
                    typename fusion::result_of::join<
                            fusion::vector<Args1...>,
                            fusion::vector<Args2...>
                        >::type
                  >::type;

    static type imp(const fusion::vector<Args1...>& v1, const fusion::vector<Args2...>& v2 )
    {
        return fusion::join( v1, v2 );
    }

};


template <class Arg1, class ... Args2>
struct multy_joiner_imp<Arg1, fusion::vector<Args2...>>
{
    using type = typename tools::result_of::as_vvector2<
                    typename fusion::result_of::push_front<
                        fusion::vector<Args2...>,
                        Arg1
                    >::type
                >::type;
    static type imp(const Arg1& arg1, const fusion::vector<Args2...>& v2 )
    {
        return fusion::push_front( v2, arg1 );
    }

};

template <class ... Args1, class Arg2>
struct multy_joiner_imp<fusion::vector<Args1...>, Arg2>
{
    using type = typename tools::result_of::as_vvector2<
                    typename fusion::result_of::push_back<
                        fusion::vector<Args1...>, Arg2
                    >::type
                >::type;

    static type imp(const fusion::vector<Args1...>& v1, const Arg2& arg2)
    {
        return fusion::push_back( v1, arg2 );
    }

};



/*template <class Arg>
struct multy_join<Arg>
{
    using type = fusion::vector<Arg>;

    static type imp(const Arg& arg)
    {
        return fusion::make_vector(arg);
    }
};*/


template <class Arg, class ... Args>
struct multy_join
{
    using inner_func =  multy_join<
                            Args...
                        >;


    using func = multy_joiner_imp<
            Arg,
            typename inner_func::type
        >;

    using type = typename func::type;

    static type imp( const Arg& arg, const Args& ... args )
    {
        return func::imp(arg, inner_func::imp(args...));
    }
};

template  <class Arg1, class Arg2>
struct multy_join<Arg1, Arg2>
{
    using func = multy_joiner_imp<
                    Arg1, Arg2
                 >;


    using type = typename func::type;

    static type imp( const Arg1& arg1, const Arg2& arg2 )
    {
        return func::imp(arg1, arg2);
    }

};

template  <class Arg1>
struct multy_join<Arg1>
{

    using type = fusion::vector<Arg1>;

    static type imp( const Arg1& arg1 )
    {
        return fusion::make_vector(arg1);
    }

};

/*

template <class Arg>
struct multy_join<Arg>
{
    using type = fusion::vector<Arg>;

    static type imp(const Arg& arg)
    {
        return fusion::make_vector(arg);
    }
};
*/
}

template <class ... Args>
typename result_of::multy_join<Args ...>::type multy_join( Args ... args )
{
    return result_of::multy_join<Args ...>::imp( args ... );
}

}

#endif
