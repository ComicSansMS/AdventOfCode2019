#include <intcode_robot.hpp>

#include <algorithm>
#include <cassert>
#include <ostream>

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

RobotState::RobotState()
    :position(0, 0), direction(Direction::Up)
{}

Color::ColorT scanPanel(RobotState& s)
{
    auto const it = s.grid.find(s.position);
    return (it == s.grid.end()) ? Color::Black : it->second;
}

Direction turnLeft(Direction d)
{
    return static_cast<Direction>((static_cast<int>(d) + 1) % 4);
}

Direction turnRight(Direction d)
{
    return static_cast<Direction>((static_cast<int>(d) + 3) % 4);
}

Vector2 moveForward(Vector2 position, Direction d)
{
    if (d == Direction::Up) {
        ++position.y;
    } else if (d == Direction::Down) {
        --position.y;
    } else if (d == Direction::Left) {
        --position.x;
    } else {
        assert(d == Direction::Right);
        ++position.x;
    }
    return position;
}

void processInput(RobotState& s, std::vector<Word> const& input)
{
    for(auto it = input.begin(); it != input.end(); ++it) {
        Word const paint_instruction = *it;
        if (paint_instruction == 0) {
            s.grid[s.position] = Color::Black;
        } else {
            assert(paint_instruction == 1);
            s.grid[s.position] = Color::White;
        }
        ++it;
        assert(it != input.end());
        Word const move_instruction = *it;
        if (move_instruction == 0) {
            s.direction = turnLeft(s.direction);
        } else {
            assert(move_instruction == 1);
            s.direction = turnRight(s.direction);
        }
        s.position = moveForward(s.position, s.direction);

        s.grid_dimensions.min.x = std::min(s.grid_dimensions.min.x, s.position.x);
        s.grid_dimensions.min.y = std::min(s.grid_dimensions.min.y, s.position.y);
        s.grid_dimensions.max.x = std::max(s.grid_dimensions.max.x, s.position.x);
        s.grid_dimensions.max.y = std::max(s.grid_dimensions.max.y, s.position.y);
    }
}

IntcodeRobot::IntcodeRobot(std::string_view input)
    :program(parseInput(input))
{
}

void IntcodeRobot::run()
{
    for (;;) {
        executeProgram(program);
        if (program.pc == ResultCode::Halted) {
            return;
        } else {
            assert(program.pc == ResultCode::MissingInput);
            processInput(robot, program.output);
            program.output.clear();
            program.input.push_back(scanPanel(robot));
            program.pc = program.resume_point;
        }
    }
}

std::ostream& operator<<(std::ostream& os, RobotState const& s)
{
    for (int y = s.grid_dimensions.max.y + 1; y >= s.grid_dimensions.min.y - 1; --y) {
        for (int x = s.grid_dimensions.min.x - 1; x <= s.grid_dimensions.max.x + 1; ++x) {
            auto const it = s.grid.find(Vector2(x, y));
            os << (((it == s.grid.end()) || (it->second == Color::Black)) ? '.' : '#');
        }
        os << '\n';
    }
    return os;
}
