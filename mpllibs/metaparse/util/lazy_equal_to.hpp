#ifndef MPLLIBS_METAPARSE_UTIL_LAZY_EQUAL_TO_HPP
#define MPLLIBS_METAPARSE_UTIL_LAZY_EQUAL_TO_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mpllibs/metatest/to_stream_fwd.hpp>

#include <boost/mpl/equal_to.hpp>

#include <iostream>

namespace mpllibs
{
  namespace metaparse
  {
    namespace util
    {
      // Accepts nullary metafunctions
      template <class A, class B>
      struct lazy_equal_to :
        boost::mpl::equal_to<typename A::type, typename B::type> {};
    }
  }

  namespace metatest
  {
    template <class A, class B>
    struct to_stream<mpllibs::metaparse::util::lazy_equal_to<A, B> >
    {
      static std::ostream& run(std::ostream& o_)
      {
        return o_ << "lazy_equal_to<not_evaluated, not_evaluated>";
      }
    };
  }
}

#endif

