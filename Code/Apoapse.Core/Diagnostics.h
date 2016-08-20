#pragma once
#include <boost\assert.hpp>

#define ASSERT(_exp)	BOOST_ASSERT(_exp)
//#define ASSERT_MSG(_exp, _msg)	BOOST_ASSERT_MSG(_exp, _msg)

#define ASSERT_MSG(_exp, _msg) {\
        if(!(_exp))\
        {\
            std::stringstream str;\
            str << _msg;\
            BOOST_ASSERT_MSG(_exp, str.str().c_str());\
        }\
    }