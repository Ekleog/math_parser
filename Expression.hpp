#ifndef EXPRESSION_HPP_INCLUDED__
#define EXPRESSION_HPP_INCLUDED__ 1

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ExprFwd.hpp"

#include "Context.hpp"

struct Expression
{
    virtual ~Expression();

    virtual ConstExprList subs() const = 0;

    virtual ExprValue operator () (const Context & c) const = 0;
};

template <typename ExprType, typename... Args>
ExprPtr New(Args&&... args)
{
    return ExprPtr(new ExprType(std::forward<Args>(args)...));
}

#endif

