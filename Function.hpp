#ifndef FUNCTION_HPP_INCLUDED__
#define FUNCTION_HPP_INCLUDED__ 1

#include <string>
#include <vector>

#include "Expression.hpp"
#include "Op0.hpp"

class Function : public Expression
{
    std::string name_;
    std::vector<ConstExprPtr> args_;

public:
    Function(const std::string name, const std::vector<ConstExprPtr> & a)
        : name_(name), args_(a)
    { }

    virtual std::vector<ConstExprPtr> subs() const
    {
        return args_;
    }

    virtual ExprValue operator () (const Context & c) const
    {
        const std::vector<std::string> & args = c.funcArgs(name_);
        Context context(c);
        for (size_t i = 0 ; i < args_.size() ; ++i) {
            context.set(args[i], (*args_[i])(c));
        }
        ConstExprPtr func = c.getFunc(name_);
        return (*func)(context);
    }
};

#endif

