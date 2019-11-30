#include <test.hpp>

#include <string>

int test1(std::string_view input)
{
    return std::atoi(input.data());
}

int test2(std::string_view input)
{
    return std::atoi(input.data());
}


