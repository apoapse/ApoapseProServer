#pragma once
#include <boost\predef.h>

#define _STR(x) #x
#define STR(x) _STR(x)
#define PLATFORM_NOT_IMPLEMENTED(_msg) __pragma(message("PLATFORM_NOT_IMPLEMENTED: "_STR(_msg) " :: " __FILE__ "@"STR(__LINE__)))

#if BOOST_OS_LINUX
#define LINUX 1
#endif // BOOST_OS_LINUX

#if BOOST_OS_WINDOWS
#define WINDOWS 1
#endif // BOOST_OS_WINDOWS

#if BOOST_OS_UNIX
#define UNIX 1
#endif // BOOST_OS_UNIX

//	DLL
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

#ifdef COMPILE_DLL
#define DLL_API DLL_EXPORT
#else
#define DLL_API DLL_IMPORT
#endif