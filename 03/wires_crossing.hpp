#ifndef ADVENT_OF_CODE_03_WIRES_CROSSING_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_03_WIRES_CROSSING_HPP_INCLUDE_GUARD

#include <array>
#include <iosfwd>
#include <optional>
#include <string_view>
#include <tuple>
#include <vector>

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

struct PathSegment {
    Direction direction;
    int length;
};

inline bool operator==(PathSegment const& lhs, PathSegment const& rhs)
{
    return (lhs.direction == rhs.direction) && (lhs.length == rhs.length);
}

struct Coordinates {
    int x;
    int y;
};

inline bool operator==(Coordinates const& lhs, Coordinates const& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

struct Line {
    Coordinates start;
    Coordinates end;
};

inline bool operator==(Line const& lhs, Line const& rhs) {
    return (lhs.start == rhs.start) && (lhs.end == rhs.end);
}

bool isVertical(Line const& l);
int length(Line const& l);
std::optional<Coordinates> intersect(Line const& l1, Line const& l2);

bool intersect(Line const& l, Coordinates const& p);

struct Wire {
    std::vector<PathSegment> path;
    std::vector<Line> lines;
};

struct Field {
    std::array<Wire, 2> wires;
};

std::ostream& operator<<(std::ostream & os, Field const& f);

Field parseInput(std::string_view input);

void layoutWires(Field& f);

std::tuple<Coordinates, int> closestIntersection(Wire const& w1, Wire const& w2);

std::array<int, 2> walkIntersectionPoints(Field const& f);

#endif
