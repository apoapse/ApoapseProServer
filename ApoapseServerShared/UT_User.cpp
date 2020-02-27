// ----------------------------------------------------------------------------
// Copyright (C) 2020 Apoapse
// Copyright (C) 2020 Guillaume Puyal
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// For more information visit https://github.com/apoapse/
// And https://apoapse.space/
// ----------------------------------------------------------------------------

#include "stdafx.h"

#ifdef UNIT_TESTS

#include "Common.h"
#include "UnitTest.hpp"
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