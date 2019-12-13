#include <intcode_arcade.hpp>

#include <algorithm>
#include <cassert>
#include <ostream>
#include <unordered_map>

int countTiles(std::vector<Word> const& intcode_output, Tile needle)
{
    int count = 0;
    for (size_t i = 2; i < intcode_output.size(); i += 3) {
        Tile t = static_cast<Tile>(intcode_output[i]);
        if (t == needle) {
            ++count;
        }
    }
    return count;
}

Vector2::Vector2()
    :x(0), y(0)
{}

Vector2::Vector2(int xx, int yy)
    :x(xx), y(yy)
{}

bool operator==(Vector2 const& lhs, Vector2 const& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

std::ostream& operator<<(std::ostream& os, Vector2 const& v) {
    return os << '[' << v.x << ',' << v.y << ']';
}

PlayingField parseField(std::vector<Word> const& intcode_output)
{
    std::unordered_map<Vector2, Tile> tiles;
    Vector2 max_position;
    Word score = 0;
    for (size_t i = 0; i < intcode_output.size(); i += 3) {
        int const x = static_cast<int>(intcode_output[i]);
        int const y = static_cast<int>(intcode_output[i + 1]);
        if (x == -1) {
            assert(y == 0);
            score = intcode_output[i + 2];
            continue;
        }
        Tile const t = static_cast<Tile>(intcode_output[i + 2]);
        auto const it = tiles.find(Vector2(x, y));
        if (it != tiles.end()) {
            if (it->second == Tile::Empty) {
                it->second = t;
            } else {
                assert(t == Tile::Empty);
            }
        }
        assert((x >= 0) && (y >= 0));
        tiles[Vector2(x, y)] = t;
        max_position.x = std::max(max_position.x, x);
        max_position.y = std::max(max_position.y, y);
    }
    PlayingField ret;
    ret.score = score;
    ret.dimensions = max_position;
    ++ret.dimensions.x;
    ++ret.dimensions.y;
    ret.tiles.resize(ret.dimensions.x * ret.dimensions.y, Tile::Empty);
    ret.n_blocks = 0;
    for (auto const& [position, tile] : tiles) {
        ret.tiles[position.y * ret.dimensions.x + position.x] = tile;
        if (tile == Tile::Ball) {
            ret.ball_position = position;
        } else if (tile == Tile::HorizontalPaddle) {
            ret.pad_position = position;
        } else if (tile == Tile::Block) {
            ++ret.n_blocks;
        }
    }
    return ret;
}

std::ostream& operator<<(std::ostream& os, PlayingField const& f)
{
    for (int ix = 0; ix < f.dimensions.x; ++ix) { os << '*'; }
    os << '\n';
    os << " *** Score: " << f.score << " | Blocks remaining: " << f.n_blocks << " ***\n";
    for (int iy = 0; iy < f.dimensions.y; ++iy) {
        for (int ix = 0; ix < f.dimensions.x; ++ix) {
            Tile t = f.tiles[iy * f.dimensions.x + ix];
            if (t == Tile::Empty) {
                os << ' ';
            } else if (t == Tile::Wall) {
                os << '#';
            } else if (t == Tile::Block) {
                os << '0';
            } else if (t == Tile::HorizontalPaddle) {
                os << '-';
            } else {
                assert(t == Tile::Ball);
                os << 'o';
            }
        }
        os << '\n';
    }
    return os;
}

int64_t playTheGame(IntegerProgram p, std::ostream* os)
{
    // insert credits:
    p.memory[0] = 2;
    for (size_t steps = 0; ; ++steps) {
        executeProgram(p);

        PlayingField f = parseField(p.output);
        if (os) { (*os) << f; }
        if (p.pc == ResultCode::Halted) {
            /// game over!
            return (f.n_blocks == 0) ? f.score : 0;
        } else {
            assert(p.pc == ResultCode::MissingInput);
        }

        if (f.pad_position.x < f.ball_position.x) {
            p.input.push_back(1);
        } else if (f.pad_position.x > f.ball_position.x) {
            // jostick to the left
            p.input.push_back(-1);
        } else {
            assert(f.pad_position.x == f.ball_position.x);
            // joystick neutral
            p.input.push_back(0);
        }
        // resume game
        p.pc = p.resume_point;
    }
    return 0;
}
