#include <frequency_transmission.hpp>

#include <catch.hpp>

TEST_CASE("Frequency Transmission")
{
    SECTION("Parse Input")
    {
        CHECK(parseInput("12345\n") == Signal{ 1,2,3,4,5 });
        CHECK(parseInput("15243\n") == Signal{ 1,5,2,4,3 });
    }

    SECTION("Signal-Pattern Multiplication")
    {
        CHECK(ranges::to<Signal>(multiplyPattern(Signal{ 4, 9, 9, 8, 7, 6, 5 }, Pattern{ 1, 2, 3 })) ==
              Signal{ 8, 27, 9, 16, 21, 6, 10 });
        CHECK(ranges::to<Signal>(multiplyPattern(Signal{ 1, 9, 8, 7, 6, 5 }, Pattern{ 2, -5 })) ==
              Signal{ -5, 18, -40, 14, -30, 10 });
    }

    SECTION("Pattern Generation")
    {
        CHECK(ranges::to<Pattern>(generatePattern(1)) == Pattern{ 0, 1, 0, -1 });
        CHECK(ranges::to<Pattern>(generatePattern(3)) == Pattern{ 0, 0, 0, 1, 1, 1, 0, 0, 0, -1, -1, -1 });
    }

    SECTION("Apply Full Phase")
    {
        std::vector<Signal> s;
        s.push_back(Signal{ 1, 2, 3, 4, 5, 6, 7, 8 });
        for (int i = 0; i < 5; ++i) { s.push_back(applyPhase(s.back())); }
        // Input signal: 12345678
        // 
        CHECK(s[0] == Signal{ 1, 2, 3, 4, 5, 6, 7, 8 });
        // 1*1  + 2*0  + 3*-1 + 4*0  + 5*1  + 6*0  + 7*-1 + 8*0  = 4
        // 1*0  + 2*1  + 3*1  + 4*0  + 5*0  + 6*-1 + 7*-1 + 8*0  = 8
        // 1*0  + 2*0  + 3*1  + 4*1  + 5*1  + 6*0  + 7*0  + 8*0  = 2
        // 1*0  + 2*0  + 3*0  + 4*1  + 5*1  + 6*1  + 7*1  + 8*0  = 2
        // 1*0  + 2*0  + 3*0  + 4*0  + 5*1  + 6*1  + 7*1  + 8*1  = 6
        // 1*0  + 2*0  + 3*0  + 4*0  + 5*0  + 6*1  + 7*1  + 8*1  = 1
        // 1*0  + 2*0  + 3*0  + 4*0  + 5*0  + 6*0  + 7*1  + 8*1  = 5
        // 1*0  + 2*0  + 3*0  + 4*0  + 5*0  + 6*0  + 7*0  + 8*1  = 8
        //
        // After 1 phase: 48226158
        //
        CHECK(ranges::to<Signal>(multiplyPattern(s[0], ranges::to<Pattern>(generatePattern(1)))) ==
              Signal{ 1, 0, -3, 0, 5, 0, -7, 0 });
        CHECK(ranges::to<Signal>(multiplyPattern(s[0], ranges::to<Pattern>(generatePattern(2)))) ==
              Signal{ 0, 2, 3, 0, 0, -6, -7, 0 });
        CHECK(ranges::to<Signal>(multiplyPattern(s[0], ranges::to<Pattern>(generatePattern(3)))) ==
              Signal{ 0, 0, 3, 4, 5, 0, 0, 0 });
        CHECK(s[1] == Signal{ 4, 8, 2, 2, 6, 1, 5, 8 });
        // 4*1  + 8*0  + 2*-1 + 2*0  + 6*1  + 1*0  + 5*-1 + 8*0  = 3
        // 4*0  + 8*1  + 2*1  + 2*0  + 6*0  + 1*-1 + 5*-1 + 8*0  = 4
        // 4*0  + 8*0  + 2*1  + 2*1  + 6*1  + 1*0  + 5*0  + 8*0  = 0
        // 4*0  + 8*0  + 2*0  + 2*1  + 6*1  + 1*1  + 5*1  + 8*0  = 4
        // 4*0  + 8*0  + 2*0  + 2*0  + 6*1  + 1*1  + 5*1  + 8*1  = 0
        // 4*0  + 8*0  + 2*0  + 2*0  + 6*0  + 1*1  + 5*1  + 8*1  = 4
        // 4*0  + 8*0  + 2*0  + 2*0  + 6*0  + 1*0  + 5*1  + 8*1  = 3
        // 4*0  + 8*0  + 2*0  + 2*0  + 6*0  + 1*0  + 5*0  + 8*1  = 8
        //
        // After 2 phases: 34040438
        //
        CHECK(s[2] == Signal{ 3, 4, 0, 4, 0, 4, 3, 8 });
        // 3*1  + 4*0  + 0*-1 + 4*0  + 0*1  + 4*0  + 3*-1 + 8*0  = 0
        // 3*0  + 4*1  + 0*1  + 4*0  + 0*0  + 4*-1 + 3*-1 + 8*0  = 3
        // 3*0  + 4*0  + 0*1  + 4*1  + 0*1  + 4*0  + 3*0  + 8*0  = 4
        // 3*0  + 4*0  + 0*0  + 4*1  + 0*1  + 4*1  + 3*1  + 8*0  = 1
        // 3*0  + 4*0  + 0*0  + 4*0  + 0*1  + 4*1  + 3*1  + 8*1  = 5
        // 3*0  + 4*0  + 0*0  + 4*0  + 0*0  + 4*1  + 3*1  + 8*1  = 5
        // 3*0  + 4*0  + 0*0  + 4*0  + 0*0  + 4*0  + 3*1  + 8*1  = 1
        // 3*0  + 4*0  + 0*0  + 4*0  + 0*0  + 4*0  + 3*0  + 8*1  = 8
        //
        // After 3 phases: 03415518
        //
        CHECK(s[3] == Signal{ 0, 3, 4, 1, 5, 5, 1, 8 });
        // 0*1  + 3*0  + 4*-1 + 1*0  + 5*1  + 5*0  + 1*-1 + 8*0  = 0
        // 0*0  + 3*1  + 4*1  + 1*0  + 5*0  + 5*-1 + 1*-1 + 8*0  = 1
        // 0*0  + 3*0  + 4*1  + 1*1  + 5*1  + 5*0  + 1*0  + 8*0  = 0
        // 0*0  + 3*0  + 4*0  + 1*1  + 5*1  + 5*1  + 1*1  + 8*0  = 2
        // 0*0  + 3*0  + 4*0  + 1*0  + 5*1  + 5*1  + 1*1  + 8*1  = 9
        // 0*0  + 3*0  + 4*0  + 1*0  + 5*0  + 5*1  + 1*1  + 8*1  = 4
        // 0*0  + 3*0  + 4*0  + 1*0  + 5*0  + 5*0  + 1*1  + 8*1  = 9
        // 0*0  + 3*0  + 4*0  + 1*0  + 5*0  + 5*0  + 1*0  + 8*1  = 8
        //
        // After 4 phases: 01029498
        //
        CHECK(s[4] == Signal{ 0, 1, 0, 2, 9, 4, 9, 8 });
    }

    SECTION("Calculate Transmission")
    {
        CHECK(calculateTransmission(parseInput("12345678"), 4) == parseInput("01029498"));
        {
            Signal s = calculateTransmission(parseInput("80871224585914546619083218645595"), 100);
            s.resize(8);
            CHECK(s == parseInput("24176176"));
        }
        {
            Signal s = calculateTransmission(parseInput("19617804207202209144916044189917"), 100);
            s.resize(8);
            CHECK(s == parseInput("73745418"));
        }
        {
            Signal s = calculateTransmission(parseInput("69317163492948606335995924319873"), 100);
            s.resize(8);
            CHECK(s == parseInput("52432133"));
        }

    }
}
