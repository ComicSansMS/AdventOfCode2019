#ifndef ADVENT_OF_CODE_13_INTCODE_ARCADE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_13_INTCODE_ARCADE_HPP_INCLUDE_GUARD

#include <integer_program_mk3.hpp>

#include <iosfwd>
#include <functional>
#include <string_view>
#include <vector>

enum class Tile {
    Empty = 0,
    Wall = 1,
    Block = 2,
    HorizontalPaddle = 3,
    Ball = 4
};

int countTiles(std::vector<Word> const& intcode_output, Tile needle);

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

struct PlayingField {
    std::vector<Tile> tiles;
    Vector2 dimensions;
    Vector2 pad_position;
    Vector2 ball_position;
    Word score;
    int n_blocks;
};

PlayingField parseField(std::vector<Word> const& intcode_output);

std::ostream& operator<<(std::ostream& os, PlayingField const& f);

int64_t playTheGame(IntegerProgram p, std::ostream* os);

#endif
