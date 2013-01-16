#ifndef AS_VVECTOR_H
#define AS_VVECTOR_H

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/invoke.hpp>
#include <type_traits>

namespace tools {
    namespace fusion = boost::fusion;

    struct as_vvector_func
    {
        template <class T>
        struct result;

        template <class Self, class ... Args>
        struct result<Self(Args...)> {
            using type = fusion::vector<typename std::remove_reference<Args>::type... >;
        };

        template <class ... Args>
        typename result<as_vvector_func(Args...)>::type operator()( Args ... args )
        {
            return fusion::vector<Args...>(args...);
        }

    };

    namespace result_of {
        template <class Vector>
        struct as_vvector {
            using type = typename fusion::result_of::invoke<as_vvector_func, Vector>::type;
        };		
    }

    template <class Vector>
    typename result_of::as_vvector<Vector>::type as_vvector(const Vector& v)
    {
        return fusion::invoke( as_vvector_func(), v );
    }
	

    namespace result_of {
        namespace detail {
            template <class ... T>
            struct null_vector;

            template <class T>
            struct to_fusion_vector;

            template <class ... Args>
            struct to_fusion_vector < null_vector<Args...> >
            {
                using type = fusion::vector<Args ...>;
            };
        }

        struct push_back_to_null_vector {
            template <class T>
            struct result;

            template <class Self, class ... Args, class NewArg>
            struct result<Self(const detail::null_vector<Args ...>&, NewArg)> {
                using type = detail::null_vector<Args ... , NewArg>;
            };
        };

        template <class Vector>
        struct as_vvector2 {
		    using null_vector = typename fusion::result_of::accumulate <
                                Vector,
                                detail::null_vector<>,
                                push_back_to_null_vector
                            >::type;

            using type = typename detail::to_fusion_vector <null_vector>::type;
        };
    }

    template <class Vector>
    typename result_of::as_vvector2<Vector>::type as_vvector2( Vector&& v )
    {
        return std::forward<Vector>(v);
    }
}

#endif // AS_VVECTOR_H
