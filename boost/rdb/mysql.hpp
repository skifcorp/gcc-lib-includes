#ifndef __MYSQL_HPP_
#define __MYSQL_HPP_


#include <boost/rdb/sql.hpp>
#include <boost/rdb/mysql/select.hpp>
#include <boost/rdb/sql/update.hpp>

namespace boost { namespace rdb { namespace mysql {

namespace sql = boost::rdb::sql;


using mysql_select_statement_type = sql::mysql_select_statement_type;

extern mysql_select_statement_type select;

using sql::update;

using sql::max;
using sql::sum;
using sql::count;
using sql::floor;
using sql::avg;
using sql::greatest;

using sql::alias;
using sql::exprs;
using sql::sql_column_alias;

}}}

#endif 
