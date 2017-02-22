#pragma once
#include <boost\assert.hpp>

#define ASSERT(_exp)						BOOST_ASSERT(_exp)
#define ASSERT_MSG(_exp, _msg)				BOOST_ASSERT_MSG(_exp, _msg)

// #TODO 
#ifdef ENABLE_SEC_ADVANCED_CHECKS
#define SECURITY_ASSERT(_exp)				ASSERT(_exp)
#define SECURITY_ASSERT_MSG(_exp, _msg)		BOOST_ASSERT_MSG(_exp, _msg)
#else
#define SECURITY_ASSERT(_exp)				if (!_exp) throw std::exception("SECURITY_ASSERT");
#define SECURITY_ASSERT_MSG(_exp, _msg)		if (!_exp) throw std::exception("SECURITY_ASSERT: " + _msg);
#endif

#ifdef DEBUG
#define LOG_DEBUG	LOG << LogSeverity::debug
#else
#define LOG_DEBUG	//
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