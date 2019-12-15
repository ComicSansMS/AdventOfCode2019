#include <path_finder.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <sstream>
#include <string>

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

MockProgram MockProgram::fromString(std::string_view map_string)
{
    MockProgram ret;
    auto it = std::find(map_string.begin(), map_string.end(), '\n');
    assert(it != map_string.end());
    int const map_width = static_cast<int>(std::distance(map_string.begin(), it));
    assert(map_string.size() % (map_width + 1) == 0);
    int const map_height = static_cast<int>(map_string.size() / (map_width + 1));
    auto const idx_origin = std::distance(map_string.begin(), std::find(map_string.begin(), map_string.end(), 'O'));
    auto const x_origin = static_cast<int>(idx_origin % (map_width + 1));
    auto const y_origin = static_cast<int>(idx_origin / (map_width + 1));
    for (int y = 0; y < map_height; ++y) {
        for (int x = 0; x < map_width; ++x) {
            char const c = map_string[y * (map_width + 1) + x];
            Vector2 ipos(x - x_origin, y_origin - y);
            if ((c == ' ') || (c == '.')) {
                ret.map[ipos] = Tile::Empty;
            } else if (c == '#') {
                ret.map[ipos] = Tile::Wall;
            } else if (c == 'X') {
                ret.map[ipos] = Tile::Target;
            } else {
                assert(c == 'O');
                assert(x == x_origin);
                assert(y == y_origin);
                ret.map[ipos] = Tile::Empty;
            }
        }
    }
    return ret;
}

void executeProgram(MockProgram& m)
{
    if (m.pc < 0) { return; }
    for (Word const& in : m.input) {
        Direction const d = static_cast<Direction>(in);
        Step s(m.position, d);
        Vector2 const target = s.getTargetPosition();
        auto it_target = m.map.find(target);
        assert(it_target != m.map.end());
        if (it_target->second == Tile::Empty) {
            m.output.push_back(1);
            m.position = target;
        } else if (it_target->second == Tile::Wall) {
            m.output.push_back(0);
        } else {
            assert(it_target->second == Tile::Target);
            m.output.push_back(2);
            m.position = target;
        }
    }
    m.input.clear();
    m.pc = -4;
    m.resume_point = 0;
}

Scanner::Scanner(IntegerProgram p)
    :vp(p)
{}

Scanner::Scanner(MockProgram m)
    :vp(m)
{}

void Scanner::executeProgram()
{
    std::visit([](auto& p) { ::executeProgram(p); }, vp);
}

void Scanner::resumeProgram()
{
    std::visit([](auto& p) {
            p.pc = p.resume_point;
            ::executeProgram(p);
        }, vp);
}

std::vector<Word>& Scanner::input()
{
    return std::visit([](auto& p) -> std::vector<Word>& { return p.input; }, vp);
}

std::vector<Word> const& Scanner::input() const
{
    return std::visit([](auto const& p) -> std::vector<Word> const& { return p.input; }, vp);
}

std::vector<Word>& Scanner::output()
{
    return std::visit([](auto& p) -> std::vector<Word>& { return p.output; }, vp);
}

std::vector<Word> const& Scanner::output() const
{
    return std::visit([](auto const& p) -> std::vector<Word> const& { return p.output; }, vp);
}

Address Scanner::pc() const
{
    return std::visit([](auto const& p) -> Address { return p.pc; }, vp);
}

MapTile::MapTile()
    :t(Tile::Empty)
{}

Step::Step(Vector2 p, Direction d)
    :initial_position(p), step(d)
{}

Vector2 Step::getTargetPosition() const
{
    if (step == Direction::North) {
        return Vector2(initial_position.x, initial_position.y + 1);
    } else if (step == Direction::South) {
        return Vector2(initial_position.x, initial_position.y - 1);
    } else if (step == Direction::West) {
        return Vector2(initial_position.x - 1, initial_position.y);
    } else {
        assert(step == Direction::East);
        return Vector2(initial_position.x + 1, initial_position.y);
    }
}

Direction reverseDirection(Direction d)
{
    if (d == Direction::North) {
        return Direction::South;
    } else if (d == Direction::South) {
        return Direction::North;
    } else if (d == Direction::West) {
        return Direction::East;
    } else {
        assert(d == Direction::East);
        return Direction::West;
    }
}

std::vector<Direction> findPath(MazeMap const& map, Vector2 start, Vector2 destination)
{
    auto it_start = map.find(start);
    assert(it_start != map.end());
    auto it_destination = map.find(destination);
    assert(it_destination != map.end());
    auto const& p1 = it_start->second.path;
    auto const& p2 = it_destination->second.path;
    auto const [it_m1, it_m2] = std::mismatch(p1.begin(), p1.end(), p2.begin(), p2.end());
    std::vector<Direction> backtrack_path(it_m1, p1.end());
    std::transform(backtrack_path.begin(), backtrack_path.end(), backtrack_path.begin(), reverseDirection);
    std::reverse(backtrack_path.begin(), backtrack_path.end());
    backtrack_path.insert(backtrack_path.end(), it_m2, p2.end());
    return backtrack_path;
}

Word toInput(Direction d)
{
    return static_cast<Word>(d);
}

std::vector<Word> toInput(std::vector<Direction> const& v)
{
    std::vector<Word> ret;
    ret.reserve(v.size());
    std::transform(v.begin(), v.end(), std::back_inserter(ret),
        [](Direction dd) -> Word { return toInput(dd); });
    return ret;
}

void walkToKnownPosition(Scanner& s, MazeMap const& map, Vector2 destination)
{
    assert(s.input().empty());
    assert(s.output().empty());
    std::vector<Direction> const path = findPath(map, s.position, destination);
    Scanner const before = s;
    MazeMap m_before = map;
    s.input() = toInput(path);
    s.resumeProgram();
    assert(s.pc() == ResultCode::MissingInput);
    assert(s.input().empty());
    assert(s.output().size() == path.size());
    assert(std::all_of(s.output().begin(), s.output().end(), [](Word o) { return (o == 1) || (o == 2); }));
    s.output().clear();
    s.position = destination;
}

Tile scanField(Scanner& s, Step next)
{
    assert(s.position == next.initial_position);
    assert(s.input().empty());
    s.input().push_back(toInput(next.step));
    s.resumeProgram();
    assert(s.pc() == ResultCode::MissingInput);
    assert(s.input().empty());
    assert(s.output().size() == 1);
    Word scan = s.output().front();
    Tile ret;
    if (scan == 0) {
        ret = Tile::Wall;
    } else if (scan == 1) {
        ret = Tile::Empty;
    } else {
        assert(scan == 2);
        ret = Tile::Target;
    }
    s.output().clear();
    return ret;
}

MazeMap floodFill(Scanner& s, std::ostream* os)
{
    MazeMap map;
    map[Vector2(0, 0)] = MapTile{};
    std::deque<Step> queue;
    queue.emplace_back(Vector2(), Direction::East);
    queue.emplace_back(Vector2(), Direction::North);
    queue.emplace_back(Vector2(), Direction::West);
    queue.emplace_back(Vector2(), Direction::South);
    while (!queue.empty())
    {
        Step next = queue.front();
        queue.pop_front();
        if (map.find(next.getTargetPosition()) != map.end()) { continue; }
        walkToKnownPosition(s, map, next.initial_position);
        Tile t = scanField(s, next);
        if (t != Tile::Wall) {
            s.position = next.getTargetPosition();
            assert(map.find(s.position) == map.end());
            auto& map_tile = map[s.position];
            map_tile.t = t;
            map_tile.path = map[next.initial_position].path;
            map_tile.path.push_back(next.step);
            queue.emplace_back(s.position, Direction::East);
            queue.emplace_back(s.position, Direction::North);
            queue.emplace_back(s.position, Direction::West);
            queue.emplace_back(s.position, Direction::South);
        } else {
            assert(map.find(next.getTargetPosition()) == map.end());
            auto& map_tile = map[next.getTargetPosition()];
            map_tile.t = Tile::Wall;
        }
        if (os) { (*os) << map << "\n\n"; }
    }
    return map;
}

MazeMap::const_iterator find_target(MazeMap const& map)
{
    return std::find_if(map.begin(), map.end(), [](auto const& p) { return p.second.t == Tile::Target; });
}

std::ostream& operator<<(std::ostream& os, MazeMap const& m)
{
    auto const [it_min_x, it_max_x] = std::minmax_element(m.begin(), m.end(),
        [](auto const& p1, auto const& p2) { return p1.first.x < p2.first.x; });
    auto const [it_min_y, it_max_y] = std::minmax_element(m.begin(), m.end(),
        [](auto const& p1, auto const& p2) { return p1.first.y < p2.first.y; });
    int const min_x = it_min_x->first.x;
    int const max_x = it_max_x->first.x;
    int const min_y = it_min_y->first.y;
    int const max_y = it_max_y->first.y;
    int const width = max_x - min_x;
    int const height = max_y - min_y;

    for (int y = max_y; y > min_y - 1; --y) {
        for (int x = min_x; x < max_x + 1; ++x) {
            auto map_it = m.find(Vector2(x, y));
            if ((x == 0) && (y == 0)) { os << 'O'; continue; }
            if (map_it == m.end()) {
                os << ' ';
            } else {
                MapTile const& t = map_it->second;
                if (t.t == Tile::Empty) {
                    os << '.';
                } else if (t.t == Tile::Wall) {
                    os << '#';
                } else {
                    assert(t.t == Tile::Target);
                    os << 'X';
                }
            }
        }
        os << '\n';
    }
    std::flush(os);
    return os;
}

int floodFill2(MazeMap map, std::ostream* os)
{
    for (auto& m : map) { m.second.path.clear(); }
    auto it_target = find_target(map);
    assert(it_target != map.end());
    Vector2 const starting_point = it_target->first;

    int ret = 0;
    std::deque<Step> queue;
    queue.emplace_back(starting_point, Direction::East);
    queue.emplace_back(starting_point, Direction::North);
    queue.emplace_back(starting_point, Direction::West);
    queue.emplace_back(starting_point, Direction::South);
    while (!queue.empty())
    {
        Step next = queue.front();
        queue.pop_front();
        Vector2 position = next.getTargetPosition();
        assert(map.find(position) != map.end());
        MapTile& map_tile = map[position];
        if ((map_tile.t == Tile::Target) || (map_tile.t == Tile::Wall)) { continue; }
        assert(map_tile.path.empty());
        map_tile.t = Tile::Target;
        map_tile.path = map[next.initial_position].path;
        map_tile.path.push_back(next.step);
        ret = std::max(ret, static_cast<int>(map_tile.path.size()));
        queue.emplace_back(position, Direction::East);
        queue.emplace_back(position, Direction::North);
        queue.emplace_back(position, Direction::West);
        queue.emplace_back(position, Direction::South);
        if (os) { (*os) << map << "\n\n"; }
    }
    return ret;
}

