#pragma once
#include <string>
#include <functional>
#include <exception>

#define UNIT_TEST(_name)	testsManager->RegisterTest(new UnitTest(_name, []() -> void
#define UNIT_TEST_END	));

class SuccessException : public std::exception {};
class FailException : public std::exception {
	std::string m_errorMsgStr;
public:
	FailException(const char* msg = "")
	{
		m_errorMsgStr = std::string(msg);
	}

	const char* what() const
	{
		return m_errorMsgStr.c_str();
	}
};

class UnitTest
{
	class TestsManager;
	typedef std::function<void()> lamdba;

	lamdba m_testCode;
	const char* m_fullName;

public:
	UnitTest(const char* fullName, lamdba code) : m_fullName(fullName), m_testCode(code)
	{
	}

	virtual ~UnitTest()
	{

	}

	bool Execute(std::string& returnErrorMsg) const
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
			returnErrorMsg = std::string(e.what());
			return false;
		}
		catch (...)
		{
		}

		return false;
	}

	const char* GetFullName() const
	{
		return m_fullName;
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