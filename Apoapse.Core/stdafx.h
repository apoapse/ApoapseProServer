#pragma once

//	SHARED PRECOMPILED HEADERS FILE

#include <string>
#include <iostream>
//#include <map>
#include <memory>
#include <vector>

#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\json_parser.hpp>
#include <boost\any.hpp>
#include <boost\none.hpp>
#include <boost\algorithm\string.hpp>

using string = std::string;

#define ASSERT(_exp)	BOOST_ASSERT(_exp, _msg)
//#define ASSERT_MSG(_exp, _msg)	BOOST_ASSERT_MSG(_exp, _msg)

#define ASSERT_MSG(_exp, _msg) {\
        if(!(_exp))\
        {\
            std::stringstream str;\
            str << _msg;\
            BOOST_ASSERT_MSG(_exp, str.str().c_str());\
        }\
    }