#ifndef CONCAT_H
#define CONCAT_H

#if 0
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/for_each.hpp>

template <class ... Args>
struct concat_query
{
    void str(std::ostream& os) const {
      os << "ura! ";
      //data_.for_each(str_clause(os));

    }

    boost::fusion::vector<Args ...> queries;
};

template <class T1, class T2>
concat_query operator<< ( const T1&, const T2& )
{
    return concat_query(  );
}

#endif

#endif

