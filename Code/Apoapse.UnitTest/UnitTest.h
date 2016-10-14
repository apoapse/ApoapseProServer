#pragma once
#include <string>
#include <functional>
#include <exception>

#define UNIT_TEST(_name)	testsManager->RegisterTest(new UnitTest(_name, [] {
#define UNIT_TEST_END	}));

class FailException : public std::exception {
	const char* m_errorMsg;
public:
	FailException(const char* msg = "") : m_errorMsg(msg)
	{
	}

	const char* what() const
	{
		return m_errorMsg;
	}
};
class SuccessException : public std::exception {};

class UnitTest
{
	class TestsManager;
	typedef std::function<void()> lamdba;

	lamdba m_testCode;
	const char* fullName;

public:
	UnitTest(const char* fullName, lamdba code) : fullName(fullName), m_testCode(code)
	{
	}

	bool Execute(char*& returnErrorMsg) const
	{
		try
		{
			m_testCode();
			throw FailException("Unit test check expected");
		}
		catch (const SuccessException&)
		{
			return true;
		}
		catch (const FailException& e)
		{
			returnErrorMsg = const_cast<char*>(e.what());
			return false;
		}
	}

	const char* GetFullName() const
	{
		return fullName;
	}

	static void Fail(const char* msg = "")
	{
		throw FailException(msg);
	}

	static void Success()
	{
		throw SuccessException();
	}

	static void Assert(bool expression, const char* msg = "")
	{
		if (expression)
			throw SuccessException();
		else
			throw FailException(msg);
	}
};