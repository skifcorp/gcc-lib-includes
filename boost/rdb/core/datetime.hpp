//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#ifndef BOOST_RDB_CORE_DATETIME_HPP
#define BOOST_RDB_CORE_DATETIME_HPP

#include <boost/rdb/core.hpp>

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <sstream>
#include <locale>
#include <string>

namespace boost { namespace rdb { namespace core {
    const std::string mysql_date_time_format = "%Y-%m-%d %H:%M:%S";

    inline std::string ptime_to_string( const ::boost::posix_time::ptime& t )
    {
        std::stringstream ss;

        std::locale l( std::locale(), new ::boost::posix_time::time_facet(mysql_date_time_format.c_str()) );

        ss.imbue(l);

        ss << t;

        return ss.str();
    }

    struct datetime {
        BOOST_STATIC_CONSTANT(int, id = 6);
        BOOST_STATIC_CONSTANT(size_t, length = 1);
        typedef boost::mpl::true_::type is_numeric;
        typedef num_comparable_type comparable_type;
        typedef numeric_type kind;
        static void str(std::ostream& os) { os << "datetime"; }
    };

    template<>
    struct make_literal<datetime, const char*> {
        typedef literal<std::string, datetime> type;
        static type value(const std::string& val) { return type(val); }
    };
  

    template<int N>
    struct make_literal<datetime, char[N]> : make_literal<datetime, const char*> { };

    template <>
    struct make_literal<datetime, ::boost::posix_time::ptime> {
        typedef literal<std::string, datetime> type;
        static type value(const ::boost::posix_time::ptime& val)
        {
            return ptime_to_string(val);
        }
    };

} } }

#endif
