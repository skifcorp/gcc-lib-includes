#ifndef DATABASE_HPP
#define DATABASE_HPP


#include <string>
#include <sstream>
#include <boost/rdb/core.hpp>

namespace boost { namespace rdb { namespace sql {

namespace core = boost::rdb::core;

struct create_database_statement
{
    create_database_statement( const std::string& n ) : database_name(n)
    {
    }

    std::string database_name;

    void str(std::ostream& os) const
    {
        os << "create database if not exists " << database_name;

    }

    using tag = core::create_database_statement_tag;
    using result = void;
};


inline create_database_statement create_database( const std::string& n )
{
    return create_database_statement( n );
}


} } }

#endif
