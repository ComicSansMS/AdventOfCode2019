#include <password_cracker.hpp>

#include <catch.hpp>

TEST_CASE("Password Cracker")
{
    SECTION("Parse Input")
    {
        char const sample_input[] = "123456-789012";
        auto const [lower, upper] = parseInput(sample_input);
        CHECK(lower == 123456);
        CHECK(upper == 789012);
    }

    SECTION("Get Digits")
    {
        CHECK(getDigits(123456) == Digits{ 1,2,3,4,5,6 });
        CHECK(getDigits(223319) == Digits{ 2,2,3,3,1,9 });
        CHECK(getDigits(111110) == Digits{ 1,1,1,1,1,0 });
    }

    SECTION("Has Adjacent Digits")
    {
        CHECK_FALSE(hasAdjacentDigits(getDigits(123456)));
        CHECK_FALSE(hasAdjacentDigits(getDigits(987130)));
        CHECK_FALSE(hasAdjacentDigits(getDigits(919191)));
        CHECK(hasAdjacentDigits(getDigits(919199)));
        CHECK(hasAdjacentDigits(getDigits(111234)));
        CHECK(hasAdjacentDigits(getDigits(123345)));
    }

    SECTION("Is Increasing")
    {
        CHECK(isIncreasing(getDigits(123456)));
        CHECK(isIncreasing(getDigits(111111)));
        CHECK(isIncreasing(getDigits(123345)));
        CHECK(isIncreasing(getDigits(112233)));
        CHECK(isIncreasing(getDigits(111116)));
        CHECK_FALSE(isIncreasing(getDigits(111110)));
        CHECK_FALSE(isIncreasing(getDigits(654321)));
        CHECK_FALSE(isIncreasing(getDigits(234356)));
    }

    SECTION("Is Valid Password")
    {
        CHECK(isValidPassword(111111));
        CHECK_FALSE(isValidPassword(223450));
        CHECK_FALSE(isValidPassword(123789));
    }

    SECTION("Generate Valid Passwords")
    {
        CHECK(generateValidPasswords(InputRange{ 123456, 123500 }) ==
            std::vector<int>{ 123466, 123477, 123488, 123499 });
    }

    SECTION("Has Double Which Is Not Part Of A Larger Group")
    {
        CHECK(hasDouble(getDigits(112233)));
        CHECK_FALSE(hasDouble(getDigits(321444)));
        CHECK(hasDouble(getDigits(111122)));
    }

    SECTION("Is Valid Password2")
    {
        CHECK_FALSE(isValidPassword2(111111));
        CHECK_FALSE(isValidPassword2(223450));
        CHECK_FALSE(isValidPassword2(123789));
        CHECK(isValidPassword2(112233));
        CHECK_FALSE(isValidPassword2(123444));
        CHECK(isValidPassword2(111122));
    }

    SECTION("Generate Valid Passwords2")
    {
        CHECK(generateValidPasswords2(InputRange{ 111234, 111334 }) ==
            std::vector<int>{ 111244, 111255, 111266, 111277, 111288, 111299, 111334 });
    }
}
