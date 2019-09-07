#include "stdafx.h"

#ifdef UNIT_TESTS

#include "Common.h"
#include "UnitTestsManager.h"
#include "User.h"
#include "Random.hpp"

UNIT_TEST("User:HashAndComparePassword")
{
	const auto salt = User::GenerateRandomSalt();
	const auto testPassword = Cryptography::GenerateRandomBytes(sha256Length);

	const auto hashedPassword = User::HashPassword(testPassword, salt);

	CHECK(User::ComparePasswords(testPassword, hashedPassword, salt));
} UNIT_TEST_END

UNIT_TEST("User:HashAndComparePassword:WrongPassword")
{
	const auto salt = User::GenerateRandomSalt();
	const auto testPassword = Cryptography::GenerateRandomBytes(sha256Length);
	const auto randomStoredPassword = Cryptography::GenerateRandomBytes(sha512Length);

	CHECK(!User::ComparePasswords(testPassword, randomStoredPassword, salt));
} UNIT_TEST_END

UNIT_TEST("User:HashAndComparePassword:WrongSalt")
{
	const auto salt = User::GenerateRandomSalt();
	const auto salt2 = User::GenerateRandomSalt();
	const auto testPassword = Cryptography::GenerateRandomBytes(sha256Length);

	const auto hashedPassword = User::HashPassword(testPassword, salt);

	CHECK(!User::ComparePasswords(testPassword, hashedPassword, salt2));
} UNIT_TEST_END

#endif	// UNIT_TESTS