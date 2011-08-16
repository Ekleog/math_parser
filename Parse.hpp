#ifndef PARSE_HPP_INCLUDED__
#define PARSE_HPP_INCLUDED__ 1

#include <stdexcept>
#include <string>

#include "Context.hpp"
#include "Expression.hpp"

struct EpicFail : public std::logic_error {
    EpicFail() : std::logic_error("Epic fail")
    { }
    EpicFail(const std::string & s) : std::logic_error("Epic fail : " + s)
    { }
};

ExprPtr Parse(std::string expr, const Context & c);

#endif

