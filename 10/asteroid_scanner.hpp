#ifndef ADVENT_OF_CODE_10_ASTEROID_SCANNER_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_10_ASTEROID_SCANNER_HPP_INCLUDE_GUARD

#include <ostream>
#include <string_view>
#include <tuple>
#include <vector>

struct Vector2 {
    int x;
    int y;

    Vector2()
        :x(0), y(0)
    {}

    Vector2(int xx, int yy)
        :x(xx), y(yy)
    {}
};

inline bool operator==(Vector2 const& lhs, Vector2 const& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

inline std::ostream& operator<<(std::ostream& os, Vector2 const& v) {
    return os << '[' << v.x << ',' << v.y << ']';
}

struct Line {
    Line(Vector2 const& origin, Vector2 const& target);

    Vector2 angle;
    int distance;
};

struct Map {
    int width;
    int height;
    std::vector<Vector2> asteroid_positions;
    std::vector<std::vector<Line>> lines_of_sight;
};

Map parseInput(std::string_view input);

void determineLinesOfSight(Map& m);

int asteroidsVisibleFrom(Map& m, std::size_t origin_index);

std::tuple<int, std::size_t> bestVantagePoint(Map& m);

int dot(Vector2 const& v1, Vector2 const& v2);

struct Target {
    std::size_t index;
    Vector2 position;
    Line line_of_sight;
    float angle;
};

std::vector<Target> determineTargets(Map& m, std::size_t origin);

std::vector<Vector2> vaporize(std::vector<Target> targets);

#endif
