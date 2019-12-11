#ifndef ADVENT_OF_CODE_11_INTCODE_ROBOT_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_11_INTCODE_ROBOT_HPP_INCLUDE_GUARD

#include <integer_program_mk3.hpp>

#include <iosfwd>
#include <string_view>
#include <unordered_map>

struct Vector2 {
    int x;
    int y;

    Vector2();
    Vector2(int xx, int yy);
};


bool operator==(Vector2 const& lhs, Vector2 const& rhs);
std::ostream& operator<<(std::ostream& os, Vector2 const& v);

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

namespace std
{
template<> struct hash<Vector2>
{
    std::size_t operator()(Vector2 const& v) const noexcept
    {
        std::size_t seed = std::hash<int>{}(v.x);
        hash_combine(seed, v.y);
        return seed;
    }
};
}

enum class Direction {
    Up = 0,
    Left = 1,
    Down = 2,
    Right = 3
};

struct Color {
    enum ColorT {
        Black = 0,
        White = 1
    };
};

struct Dimensions {
    Vector2 min;
    Vector2 max;
};

struct RobotState {
    Vector2 position;
    Direction direction;
    std::unordered_map<Vector2, Color::ColorT> grid;
    Dimensions grid_dimensions;

    RobotState();
};

Direction turnLeft(Direction d);

Direction turnRight(Direction d);

Vector2 moveForward(Vector2 position, Direction d);


Color::ColorT scanPanel(RobotState& s);

void processInput(RobotState& s, std::vector<Word> const& input);

std::ostream& operator<<(std::ostream& os, RobotState const& s);

struct IntcodeRobot {
    IntegerProgram program;
    RobotState robot;

    IntcodeRobot(std::string_view input);

    void run();
};

#endif
