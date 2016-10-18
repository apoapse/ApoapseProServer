#pragma once
#include <boost\assert.hpp>

#define ASSERT(_exp)	BOOST_ASSERT(_exp)
#define ASSERT_MSG(_exp, _msg)	BOOST_ASSERT_MSG(_exp, _msg)

#ifdef DEBUG
#define LOG_DEBUG_ONLY(_text)	Log(_text, LogSeverity::debug);
#else
#define LOG_DEBUG_ONLY(_text)
#endif

/*#define ASSERT_MSG(_exp, _msg) {\
        if(!(_exp))\
        {\
            std::stringstream str;\
            str << _msg;\
            BOOST_ASSERT_MSG(_exp, str.str().c_str());\
        }\
    }*/