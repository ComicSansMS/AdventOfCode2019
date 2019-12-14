#include <fuel_refinement.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Fuel Refinement")
{
    char const sample_input1[] = "10 ORE => 10 A\n"
                                 "1 ORE => 1 B\n"
                                 "7 A, 1 B => 1 C\n"
                                 "7 A, 1 C => 1 D\n"
                                 "7 A, 1 D => 1 E\n"
                                 "7 A, 1 E => 1 FUEL\n";

    SECTION("Parse Input")
    {
        Factory const f = parseInput(sample_input1);
        REQUIRE(f.size() == 6);
        CHECK(f[0].ingredients == std::vector<Component>{ { 10, "ORE" } });
        CHECK(f[0].result == Component{ 10, "A" });

        CHECK(f[1].ingredients == std::vector<Component>{ { 1, "ORE" } });
        CHECK(f[1].result == Component{ 1, "B" });

        CHECK(f[2].ingredients == std::vector<Component>{ { 7, "A" }, {1, "B"} });
        CHECK(f[2].result == Component{ 1, "C" });

        CHECK(f[3].ingredients == std::vector<Component>{ { 7, "A" }, {1, "C"} });
        CHECK(f[3].result == Component{ 1, "D" });

        CHECK(f[4].ingredients == std::vector<Component>{ { 7, "A" }, {1, "D"} });
        CHECK(f[4].result == Component{ 1, "E" });

        CHECK(f[5].ingredients == std::vector<Component>{ { 7, "A" }, {1, "E"} });
        CHECK(f[5].result == Component{ 1, "FUEL" });
    }

    SECTION("Map Building")
    {
        Factory const f = parseInput(sample_input1);
        ReactionMap m = buildReactionMap(f);
        CHECK(m.size() == f.size());
        CHECK(m["FUEL"]->result == Component{ 1, "FUEL" });
        CHECK(m["FUEL"]->ingredients == std::vector<Component>{ { 7, "A" }, { 1, "E" } });

        CHECK(m["E"]->result == Component{ 1, "E" });
        CHECK(m["E"]->ingredients == std::vector<Component>{ { 7, "A" }, {1, "D"} });

        CHECK(m["D"]->result == Component{ 1, "D" });
        CHECK(m["D"]->ingredients == std::vector<Component>{ { 7, "A" }, {1, "C"} });

        CHECK(m["C"]->result == Component{ 1, "C" });
        CHECK(m["C"]->ingredients == std::vector<Component>{ { 7, "A" }, {1, "B"} });

        CHECK(m["B"]->result == Component{ 1, "B" });
        CHECK(m["B"]->ingredients == std::vector<Component>{ { 1, "ORE" } });

        CHECK(m["A"]->result == Component{ 10, "A" });
        CHECK(m["A"]->ingredients == std::vector<Component>{ { 10, "ORE" } });
    }

    char const sample_input2[] = "9 ORE => 2 A\n"
                                 "8 ORE => 3 B\n"
                                 "7 ORE => 5 C\n"
                                 "3 A, 4 B => 1 AB\n"
                                 "5 B, 7 C => 1 BC\n"
                                 "4 C, 1 A => 1 CA\n"
                                 "2 AB, 3 BC, 4 CA => 1 FUEL\n";

    char const sample_input3[] = "157 ORE => 5 NZVS\n"
                                 "165 ORE => 6 DCFZ\n"
                                 "44 XJWVT, 5 KHKGT, 1 QDVJ, 29 NZVS, 9 GPVTF, 48 HKGWZ => 1 FUEL\n"
                                 "12 HKGWZ, 1 GPVTF, 8 PSHF => 9 QDVJ\n"
                                 "179 ORE => 7 PSHF\n"
                                 "177 ORE => 5 HKGWZ\n"
                                 "7 DCFZ, 7 PSHF => 2 XJWVT\n"
                                 "165 ORE => 2 GPVTF\n"
                                 "3 DCFZ, 7 NZVS, 5 HKGWZ, 10 PSHF => 8 KHKGT\n";

    char const sample_input4[] = "2 VPVL, 7 FWMGM, 2 CXFTF, 11 MNCFX => 1 STKFG\n"
                                 "17 NVRVD, 3 JNWZP => 8 VPVL\n"
                                 "53 STKFG, 6 MNCFX, 46 VJHF, 81 HVMC, 68 CXFTF, 25 GNMV => 1 FUEL\n"
                                 "22 VJHF, 37 MNCFX => 5 FWMGM\n"
                                 "139 ORE => 4 NVRVD\n"
                                 "144 ORE => 7 JNWZP\n"
                                 "5 MNCFX, 7 RFSQX, 2 FWMGM, 2 VPVL, 19 CXFTF => 3 HVMC\n"
                                 "5 VJHF, 7 MNCFX, 9 VPVL, 37 CXFTF => 6 GNMV\n"
                                 "145 ORE => 6 MNCFX\n"
                                 "1 NVRVD => 8 CXFTF\n"
                                 "1 VJHF, 6 MNCFX => 4 RFSQX\n"
                                 "176 ORE => 6 VJHF\n";

    char const sample_input5[] = "171 ORE => 8 CNZTR\n"
                                 "7 ZLQW, 3 BMBT, 9 XCVML, 26 XMNCP, 1 WPTQ, 2 MZWV, 1 RJRHP => 4 PLWSL\n"
                                 "114 ORE => 4 BHXH\n"
                                 "14 VRPVC => 6 BMBT\n"
                                 "6 BHXH, 18 KTJDG, 12 WPTQ, 7 PLWSL, 31 FHTLT, 37 ZDVW => 1 FUEL\n"
                                 "6 WPTQ, 2 BMBT, 8 ZLQW, 18 KTJDG, 1 XMNCP, 6 MZWV, 1 RJRHP => 6 FHTLT\n"
                                 "15 XDBXC, 2 LTCX, 1 VRPVC => 6 ZLQW\n"
                                 "13 WPTQ, 10 LTCX, 3 RJRHP, 14 XMNCP, 2 MZWV, 1 ZLQW => 1 ZDVW\n"
                                 "5 BMBT => 4 WPTQ\n"
                                 "189 ORE => 9 KTJDG\n"
                                 "1 MZWV, 17 XDBXC, 3 XCVML => 2 XMNCP\n"
                                 "12 VRPVC, 27 CNZTR => 2 XDBXC\n"
                                 "15 KTJDG, 12 BHXH => 5 XCVML\n"
                                 "3 BHXH, 2 VRPVC => 7 MZWV\n"
                                 "121 ORE => 7 VRPVC\n"
                                 "7 XCVML => 6 RJRHP\n"
                                 "5 BHXH, 4 VRPVC => 5 LTCX\n";

    SECTION("Ore Required For 1 Fuel")
    {
        CHECK(oreForOneFuel(buildReactionMap(parseInput(sample_input1))) == 31);
        CHECK(oreForOneFuel(buildReactionMap(parseInput(sample_input2))) == 165);
        CHECK(oreForOneFuel(buildReactionMap(parseInput(sample_input3))) == 13312);
        CHECK(oreForOneFuel(buildReactionMap(parseInput(sample_input4))) == 180697);
        CHECK(oreForOneFuel(buildReactionMap(parseInput(sample_input5))) == 2210736);
    }

    SECTION("One Trillion Fuel")
    {
        CHECK(fuelFromOneTrillion(buildReactionMap(parseInput(sample_input3))) == 82892753);
        CHECK(fuelFromOneTrillion(buildReactionMap(parseInput(sample_input4))) == 5586022);
        CHECK(fuelFromOneTrillion(buildReactionMap(parseInput(sample_input5))) == 460664);
    }
}
