#ifndef MAKE_VVECTOR_H
#define MAKE_VVECTOR_H


#include <boost/fusion/include/vector.hpp>

namespace tools {
    namespace fusion = boost::fusion;
    namespace result_of {
        template <class ... Args>
        struct make_vector
        {
            using type = fusion::vector<Args ...>;
        };
    }


    template <class ... Args>
    typename result_of::make_vector<Args ...>::type make_vvector( Args && ... args )
    {
        return fusion::vector<Args ...>( std::forward<Args>(args)... );
    }
}



#endif
