//  Copyright Jean-Louis Leroy 2009.
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.

#define n BOOST_PP_ITERATION()

template<BOOST_PP_ENUM_PARAMS(n, class Col)>
typename transition<
  typename Subdialect::cols,
  /*fusion::vector<
    BOOST_PP_ENUM_PARAMS(n, Col)
  >*/
    /*decltype(fusion::make_vector<
             BOOST_PP_ENUM_PARAMS(n, Col)
    >)*/
    typename fusion::result_of::make_vector<
        BOOST_PP_ENUM_PARAMS(n, Col)
    >::type
>::type
operator ()(BOOST_PP_ENUM_BINARY_PARAMS(n, const Col, &col)) {
  // failure here probably indicates that you are trying to pass something else than
  // columns or dynamic_columns here - note : expressions are not allowed
  BOOST_PP_REPEAT(n, BOOST_PP_RDB_MPL_ASSERT, (is_column_container, Col))
  return typename transition<
    typename Subdialect::cols,
    /*fusion::vector<
      BOOST_PP_ENUM_PARAMS(n, Col)
    >*/
/*    decltype(fusion::make_vector<
             BOOST_PP_ENUM_PARAMS(n, Col)
    >)*/
    typename fusion::result_of::make_vector<
        BOOST_PP_ENUM_PARAMS(n, Col)
    >::type
  >::type(ct::add_key<typename Subdialect::cols>(this->data_, fusion::make_vector(BOOST_PP_ENUM_PARAMS(n, col))));
}
