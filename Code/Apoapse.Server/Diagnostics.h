#pragma once
#include <boost\assert.hpp>

#define ASSERT(_exp)	BOOST_ASSERT(_exp)
#define ASSERT_MSG(_exp, _msg)	BOOST_ASSERT_MSG(_exp, _msg)

// #TODO 
#ifdef ENABLE_SEC_ASSERTS
#define SECURITY_ASSERT(_exp)	ASSERT(_exp)
#define SECURITY_ASSERT_MSG(_exp, _msg)	BOOST_ASSERT_MSG(_exp, _msg)
#else
#define SECURITY_ASSERT(_exp)
#define SECURITY_ASSERT_MSG(_exp, _msg)
#endif

#ifdef DEBUG
#define LOG_DEBUG_ONLY(_params)	LOG << LogSeverity::debug << __FUNCTION__ << " (" << __FILE__ << ":" << __LINE__ << ") " << _params
#else
#define LOG_DEBUG_ONLY(_params)
#endif

#ifdef DEBUG
#define LOG_DEBUG_FUNCTION_NAME()	LOG << LogSeverity::debug << __FUNCTION__
#else
#define LOG_DEBUG_FUNCTION_NAME()
#endif

/*#define ASSERT_MSG(_exp, _msg) {\
        if(!(_exp))\
        {\
            std::stringstream str;\
            str << _msg;\
            BOOST_ASSERT_MSG(_exp, str.str().c_str());\
        }\
    }*/