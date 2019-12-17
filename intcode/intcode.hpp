#ifndef ADVENT_OF_CODE_INTCODE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_INTCODE_HPP_INCLUDE_GUARD

#include <memory>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

using Word = int64_t;
using Address = int64_t;

struct IntcodeProgram {
    std::unordered_map<Address, Word> memory;
    Address pc;
    Address base;
    std::vector<Word> input;
    std::vector<Word> output;
    Address resume_point;
};

enum class Opcode {
    Add = 1,
    Multiply = 2,
    Input = 3,
    Output = 4,
    JumpIfTrue = 5,
    JumpIfFalse = 6,
    LessThan = 7,
    Equals = 8,
    AdjustRelativeBase = 9,
    Halt = 99
};

enum class Mode {
    Position = 0,
    Immediate = 1,
    Relative = 2,
    NoArgument = -1
};

struct ResultCode {
    enum ResultCodes {
        Halted = -1,
        InvalidOpcode = -2,
        InvalidAddress = -3,
        MissingInput = -4
    };
};

IntcodeProgram parseInput(std::string_view input);

std::tuple<Opcode, Mode, Mode, Mode> decode(Word instruction);

void executeOpcode(IntcodeProgram& p);

void executeProgram(IntcodeProgram& p);

class Intcode {
public:
    struct Concept {
        virtual ~Concept() = default;
        virtual std::unique_ptr<Concept> clone() = 0;
        virtual ResultCode::ResultCodes execute() = 0;
        virtual ResultCode::ResultCodes resumeExecution() = 0;
        virtual Address& pc() = 0;
        virtual Address const& pc() const = 0;
        virtual std::vector<Word>& input() = 0;
        virtual std::vector<Word> const & input() const = 0;
        virtual std::vector<Word>& output() = 0;
        virtual std::vector<Word> const & output() const = 0;
    };
private:
    std::unique_ptr<Concept> m_handle;
public:
    template<typename T>
    Intcode(T const& v)
        :m_handle(makeConceptHandle(v))
    {}

    ~Intcode() = default;

    Intcode(Intcode&& rhs) = default;
    Intcode& operator=(Intcode&& rhs) = default;

    Intcode(Intcode const& rhs)
        :m_handle(rhs.m_handle->clone())
    {}

    Intcode& operator=(Intcode const& rhs)
    {
        if (&rhs != this) {
            m_handle = rhs.m_handle->clone();
        }
        return *this;
    }

    ResultCode::ResultCodes execute() { return m_handle->execute(); }
    ResultCode::ResultCodes resumeExecution() { return m_handle->resumeExecution(); }
    Address& pc() { return m_handle->pc(); }
    Address const& pc() const { return m_handle->pc(); }
    std::vector<Word>& input() { return m_handle->input(); }
    std::vector<Word> const & input() const { return m_handle->input(); }
    std::vector<Word>& output() { return m_handle->output(); }
    std::vector<Word> const & output() const { return m_handle->output(); }
};

template<typename IntcodeProgram_T>
struct IntcodeProgramModel : Intcode::Concept {
    IntcodeProgram_T p;
    IntcodeProgramModel(IntcodeProgram_T program)
        : p(program)
    {}
    ~IntcodeProgramModel() override = default;
    std::unique_ptr<Concept> clone() {
        return std::make_unique<IntcodeProgramModel<IntcodeProgram_T>>(p);
    }
    ResultCode::ResultCodes execute() override {
        executeProgram(p);
        return static_cast<ResultCode::ResultCodes>(p.pc);
    }
    ResultCode::ResultCodes resumeExecution() override {
        p.pc = p.resume_point;
        return execute();
    }
    Address& pc() override {
        return p.pc;
    }
    Address const& pc() const override {
        return p.pc;
    }
    std::vector<Word>& input() override {
        return p.input;
    }
    std::vector<Word> const& input() const override {
        return p.input;
    }
    std::vector<Word>& output() override {
        return p.output;
    }
    std::vector<Word> const& output() const override {
        return p.output;
    }
};

template<typename IntcodeProgram_T>
inline std::unique_ptr<Intcode::Concept> makeConceptHandle(IntcodeProgram_T const& p) {
    return std::make_unique<IntcodeProgramModel<IntcodeProgram_T>>(p);
}

#endif
