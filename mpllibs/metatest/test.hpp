#ifndef MPLLIBS_METATEST_TEST_HPP
#define MPLLIBS_METATEST_TEST_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2009 - 2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mpllibs/metatest/test_driver.hpp>
#include <mpllibs/metatest/test_result.hpp>

#ifdef MPLLIBS_ADD_TEST
  #error MPLLIBS_ADD_TEST already defined
#endif
// We have to use a namespace not used anywhere else to avoid
// test cases being replaced by other classes in the background
#define MPLLIBS_ADD_TEST(suite, name) \
  namespace mpllibs_test_cases \
  { \
    namespace \
    { \
      struct name##_executor \
      { \
        name##_executor() \
        { \
          mpllibs::metatest::test_driver::instance().add( \
            (suite), \
            mpllibs::metatest::test_result::build<name>(MPLLIBS_HERE, #name) \
          ); \
        } \
        \
        static name##_executor instance; \
      }; \
      \
      name##_executor name##_executor::instance; \
    } \
  }

#endif

