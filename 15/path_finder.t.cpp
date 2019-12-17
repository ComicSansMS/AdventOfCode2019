#include <path_finder.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Path Finder")
{
    SECTION("Vector2")
    {
        CHECK(Vector2().x == 0);
        CHECK(Vector2().y == 0);
        CHECK(Vector2(1, 2).x == 1);
        CHECK(Vector2(1, 2).y == 2);
        CHECK(Vector2(1, 2) == Vector2(1, 2));
        CHECK_FALSE(Vector2(1, 2) == Vector2(0, 2));
        CHECK_FALSE(Vector2(1, 2) == Vector2(1, 0));
        CHECK_FALSE(Vector2(1, 2) == Vector2(0, 0));
        std::stringstream sstr;
        sstr << Vector2(5, -12);
        CHECK(sstr.str() == "[5,-12]");
    }

    char const test_map[] = "      \n"
                            "   ## \n"
                            "  #O.#\n"
                            " #X.# \n"
                            "  ##  \n";

    SECTION("Mock Construction")
    {
        MockProgram const m = MockProgram::fromString(test_map);
        CHECK(m.position == Vector2(0, 0));
        CHECK(m.map.find(Vector2(0, 0))->second == Tile::Empty);

        // first row
        CHECK(m.map.find(Vector2(-3,  2))->second == Tile::Empty);
        CHECK(m.map.find(Vector2(-2,  2))->second == Tile::Empty);
        CHECK(m.map.find(Vector2(-1,  2))->second == Tile::Empty);
        CHECK(m.map.find(Vector2( 0,  2))->second == Tile::Empty);
        CHECK(m.map.find(Vector2( 1,  2))->second == Tile::Empty);
        CHECK(m.map.find(Vector2( 2,  2))->second == Tile::Empty);

        // second row
        CHECK(m.map.find(Vector2(-3,  1))->second == Tile::Empty);
        CHECK(m.map.find(Vector2(-2,  1))->second == Tile::Empty);
        CHECK(m.map.find(Vector2(-1,  1))->second == Tile::Empty);
        CHECK(m.map.find(Vector2( 0,  1))->second == Tile::Wall);
        CHECK(m.map.find(Vector2( 1,  1))->second == Tile::Wall);
        CHECK(m.map.find(Vector2( 2,  1))->second == Tile::Empty);

        // third row
        CHECK(m.map.find(Vector2(-3,  0))->second == Tile::Empty);
        CHECK(m.map.find(Vector2(-2,  0))->second == Tile::Empty);
        CHECK(m.map.find(Vector2(-1,  0))->second == Tile::Wall);
        CHECK(m.map.find(Vector2( 0,  0))->second == Tile::Empty);
        CHECK(m.map.find(Vector2( 1,  0))->second == Tile::Empty);
        CHECK(m.map.find(Vector2( 2,  0))->second == Tile::Wall);

        // fourth row
        CHECK(m.map.find(Vector2(-3, -1))->second == Tile::Empty);
        CHECK(m.map.find(Vector2(-2, -1))->second == Tile::Wall);
        CHECK(m.map.find(Vector2(-1, -1))->second == Tile::Target);
        CHECK(m.map.find(Vector2( 0, -1))->second == Tile::Empty);
        CHECK(m.map.find(Vector2( 1, -1))->second == Tile::Wall);
        CHECK(m.map.find(Vector2( 2, -1))->second == Tile::Empty);

        // fifth row
        CHECK(m.map.find(Vector2(-3, -2))->second == Tile::Empty);
        CHECK(m.map.find(Vector2(-2, -2))->second == Tile::Empty);
        CHECK(m.map.find(Vector2(-1, -2))->second == Tile::Wall);
        CHECK(m.map.find(Vector2( 0, -2))->second == Tile::Wall);
        CHECK(m.map.find(Vector2( 1, -2))->second == Tile::Empty);
        CHECK(m.map.find(Vector2( 2, -2))->second == Tile::Empty);
    }

    SECTION("Mocking")
    {
        MockProgram const m = MockProgram::fromString(test_map);
        Scanner s(m);
        CHECK(s.pc() == 0);
        s.executeProgram();
        CHECK(s.pc() == ResultCode::MissingInput);
        CHECK(s.output().empty());
    
        std::vector<Word> directions =
            toInput({ Direction::North, Direction::East, Direction::North, Direction::East, Direction::South });
        s.input().insert(s.input().begin(), directions.begin(), directions.end());
        s.resumeProgram();
        CHECK(s.pc() == ResultCode::MissingInput);
        CHECK(s.input().empty());
        CHECK(s.output() == std::vector<Word>{ 0, 1, 0, 0, 0 });
        s.output().clear();
        CHECK(std::get<MockProgram>(s.vp).position == Vector2{1, 0});

        directions =
            toInput({ Direction::West, Direction::West, Direction::South, Direction::West });
        s.input().insert(s.input().begin(), directions.begin(), directions.end());
        s.resumeProgram();
        CHECK(s.pc() == ResultCode::MissingInput);
        CHECK(s.input().empty());
        CHECK(s.output() == std::vector<Word>{ 1, 0, 1, 2 });
        s.output().clear();
        CHECK(std::get<MockProgram>(s.vp).position == Vector2{-1, -1});
    }

    SECTION("Simple Maze")
    {
        MockProgram const m = MockProgram::fromString(test_map);
        Scanner s(m);

        auto const map = floodFill(s);
        CHECK(s.pc() == ResultCode::MissingInput);
        std::stringstream sstr;
        sstr << map;
        CHECK(sstr.str() == "  ## \n"
                            " #O.#\n"
                            "#X.# \n"
                            " ##  \n");

        CHECK(find_target(map)->first == Vector2(-1, -1));
    }

    char const test_map2[] = " ##   \n"
                             "#.O## \n"
                             "#.#..#\n"
                             "#.X.# \n"
                             " ###  \n";

    SECTION("Flood Fill #2")
    {
        {
            MockProgram const m = MockProgram::fromString(test_map);
            Scanner s(m);

            auto const map = floodFill(s);
            CHECK(floodFill2(map) == 3);
        }
        {
            MockProgram const m = MockProgram::fromString(test_map2);
            Scanner s(m);

            auto const map = floodFill(s);
            CHECK(floodFill2(map) == 4);
        }
    }
}
